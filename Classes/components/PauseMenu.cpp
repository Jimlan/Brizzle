#include "PauseMenu.h"
#include "base/BaseMenuItemSprite.h"
#include "scenes/HomeScene.h"


bool PauseMenu::init()
{
    const float animateTime = 0.3f;
    if(!CCNode::init())
    {
        return false;
    }

    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("images/pause/paused_RETINA.plist");
    frameCache->addSpriteFramesWithFile("images/pause/paused_CN_RETINA.plist");

    CCLayerColor *bgLayer = CCLayerColor::create(ccc4(0,0,0,0));
    CCFadeTo *fadeTo = CCFadeTo::create(animateTime,128);
    addChild(bgLayer);
    bgLayer->runAction(fadeTo);

    m_pMenuNode = CCNode::create();
    addChild(m_pMenuNode);

    CCSprite *bg = SPRITE("paused_bg.png");
    bg->setAnchorPoint(CCPointZero);
    m_pMenuNode->addChild(bg);
    m_pMenuNode->setPositionY(-463);

    CCMenuItemSprite *quit = BaseMenuItemSprite::create(SPRITE("paused_quit_normal_china.png"),SPRITE("paused_quit_push_china.png"),this,menu_selector(PauseMenu::__quitGame));
    CCMenuItemSprite *restart = BaseMenuItemSprite::create(SPRITE("paused_restart_normal_china.png"),SPRITE("paused_restart_push_china.png"),this,menu_selector(PauseMenu::__restartGame));
    CCMenuItemSprite *resume = BaseMenuItemSprite::create(SPRITE("paused_resume_normal_china.png"),SPRITE("paused_resume_push_china.png"),this,menu_selector(PauseMenu::__resumeGame));

    CCMenu *menu = CCMenu::create(resume,quit,restart,NULL);
    menu->alignItemsVerticallyWithPadding(10);
    m_pMenuNode->addChild(menu);
    menu->setPosition(ccp(380,160));
    CCMoveTo *moveAct = CCMoveTo::create(animateTime,CCPointZero);
    m_pMenuNode->runAction(moveAct);

    return true;
}

void PauseMenu::__resumeGame( CCObject * )
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_CLOSE_PAUSE_MENU);
	removeFromParent();
}

void PauseMenu::__quitGame( CCObject * )
{
    CCDirector::sharedDirector()->replaceScene(HomeScene::create());
}

void PauseMenu::__restartGame( CCObject * )
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_RESTART_GAME);
}
