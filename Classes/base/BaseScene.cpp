#include "BaseScene.h"
#include "components/ExitWin.h"
#include "managers/ShareManager.h"


#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
#include "PluginManager.h"
#include "jni.h"
#include "platform/android/jni/JniHelper.h"
void MyAdsListener::onAdsResult(AdsResultCode code, const char* msg)
{
    CCLog("OnAdsResult, code : %d, msg : %s", code, msg);
	if(code==kAdsReceived)
	{
		//CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_ADS_RECEIVED);
	}
}

void MyAdsListener::onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points)
{
    CCLog("Player get points : %d", points);
    /* 用户在这里点击了广告 应该给予用户奖励 给玩家增加20秒的时间 */
    if (pAdsPlugin != NULL)
    {
        pAdsPlugin->spendPoints(points);
        pAdsPlugin->hideAds(ProtocolAds::kBannerAd);
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_ADS_SPEND);
    }
}

#endif


bool BaseScene::init()
{
    if(!CCScene::init())
    {
        return false;
    }
    loadAssets();
    return true;
}

BaseScene::BaseScene()
{
    m_pDirector = CCDirector::sharedDirector();
    m_winSize = m_pDirector->getWinSize();
    m_pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    m_pAnimationCache = CCAnimationCache::sharedAnimationCache();
    m_pFrameCache->removeUnusedSpriteFrames();
    m_pDirector->purgeCachedData();
    isShowExit = false;
}

void BaseScene::onEnter()
{
    CCScene::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(BaseScene::resumeGame),NOTI_RESUME_GAME,NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(BaseScene::exitGameDelegate),NOTI_EXIT_GAME,NULL);
}

void BaseScene::onExit()
{
    CCScene::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTI_EXIT_GAME);
}

void BaseScene::exitGame()
{
    if(isShowExit==false)
    {
        isShowExit = true;
        addChild(ExitWin::create());
        ShareManager::shareManager()->isGamePause = true;
    }
    else
    {
        ShareManager::shareManager()->isGamePause = false;
        isShowExit = false;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_HIDE_EXIT_WIN);
    }
}

void BaseScene::exitGameDelegate( float del )
{
    scheduleOnce(schedule_selector(BaseScene::exitGame),0);
}

void BaseScene::resumeGame( CCObject *obj )
{
    isShowExit = false;
    ShareManager::shareManager()->isGamePause = false;
}

void BaseScene::showAds()
{
#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
    m_pAdsListener = new MyAdsListener();
    m_pAdmob = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdsAdmob"));
    TAdsDeveloperInfo devInfo;
    devInfo["AdmobID"] = "a152c0e2117dbe6";
    m_pAdmob->configDeveloperInfo(devInfo);
    m_pAdmob->setAdsListener(m_pAdsListener);
    m_pAdmob->setDebugMode(false);
    m_pAdmob->showAds(ProtocolAds::kBannerAd,0,ProtocolAds::kPosTop);
#endif
}

void BaseScene::toast( const char *msg )
{
#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
	JniMethodInfo methodInfo;
	bool hasToast = JniHelper::getStaticMethodInfo(methodInfo,"com/giant/brizzle91/JNIBrige","toast","(Ljava/lang/String;)V");
	if(hasToast)
	{
		jstring jmsg = methodInfo.env->NewStringUTF(msg);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID,methodInfo.methodID,jmsg);
	}
#endif
}

