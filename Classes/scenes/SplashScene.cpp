#include "SplashScene.h"
#include "HomeScene.h"

bool SplashScene::init()
{
    if(!BaseScene::init())
    {
        return false;
    }

    CCSprite *splashSpr = CCSprite::create("images/loading/splash.png");
    splashSpr->setPosition(VisibleRect::center());
    addChild(splashSpr);
    CCCallFunc *delayCall = CCCallFunc::create(this,callfunc_selector(SplashScene::__gotoHome));
    runAction(CCSequence::create(CCDelayTime::create(1.5f),delayCall,NULL));
    return true;
}

void SplashScene::loadAssets()
{

}

void SplashScene::__gotoHome()
{
    CCDirector::sharedDirector()->replaceScene(HomeScene::create());
}

