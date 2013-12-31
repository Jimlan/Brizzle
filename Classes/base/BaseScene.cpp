#include "BaseScene.h"
#include "components/ExitWin.h"


#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
#include "PluginManager.h"
void MyAdsListener::onAdsResult(AdsResultCode code, const char* msg)
{
    CCLog("OnAdsResult, code : %d, msg : %s", code, msg);
}

void MyAdsListener::onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points)
{
    CCLog("Player get points : %d", points);

    // @warning should add code to give game-money to player here

    // spend the points of player
    if (pAdsPlugin != NULL)
    {
        pAdsPlugin->spendPoints(points);
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

#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
    m_pAdsListener = new MyAdsListener();
    m_pAdmob = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdsAdmob"));
    TAdsDeveloperInfo devInfo;
    devInfo["AdmobID"] = "a152c0e2117dbe6";
    m_pAdmob->configDeveloperInfo(devInfo);
    m_pAdmob->setAdsListener(m_pAdsListener);
    m_pAdmob->setDebugMode(true);
    m_pAdmob->showAds(ProtocolAds::kBannerAd,0,ProtocolAds::kPosTop);

#endif

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
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(BaseScene::resumeGame),NOTI_HIDE_EXIT_WIN,NULL);
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
    }
    else
    {
        isShowExit = false;
        CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_CLOSE_EXIT_WIN);
    }

}

void BaseScene::exitGameDelegate( float del )
{
    scheduleOnce(schedule_selector(BaseScene::exitGame),0);
}

void BaseScene::resumeGame( CCObject *obj )
{
    isShowExit = false;
}

