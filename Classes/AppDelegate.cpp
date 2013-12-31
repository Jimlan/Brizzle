#include "AppDelegate.h"
#include "scenes/HomeScene.h"
#include "config/AppStartup.h"
#include "common/notifications_names.h"
USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    pEGLView->setDesignResolutionSize(640,960,kResolutionNoBorder);
    // pDirector->setDisplayStats(true);
    pDirector->setAnimationInterval(1.0 / 60);
    CCScene *pScene = HomeScene::create();
    pDirector->runWithScene(pScene);
    AppStartup::create();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
#include "jni.h"
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

extern "C"
{
    JNIEXPORT void JNICALL Java_com_giant_brizzle_Brizzle_exitGame(JNIEnv *env,jobject thiz)
    {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_EXIT_GAME,NULL);
    }
};

#endif