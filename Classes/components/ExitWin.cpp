#include "ExitWin.h"

#define BB_WIN_EXIT_TIME 0.3f

bool ExitWin::init()
{
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initBgLayer();
        winBg = CCSprite::create("images/exit/exit_bg.png");
        winBg->setScale(0);
        CCActionInterval *scaleBig = CCScaleTo::create(BB_WIN_EXIT_TIME,1);
        CCActionInterval *fadeIn = CCFadeIn::create(BB_WIN_EXIT_TIME);
        winBg->runAction(CCEaseBackOut::create(scaleBig));
        winBg->setPosition(VisibleRect::center());
        yes = CCMenuItemSprite::create(CCSprite::create("images/exit/exit_yes_normal.png"),CCSprite::create("images/exit/exit_yes_push.png"),this,menu_selector(ExitWin::exitGame));
        no = CCMenuItemSprite::create(CCSprite::create("images/exit/exit_no_normal.png"),CCSprite::create("images/exit/exit_no_push.png"),this,menu_selector(ExitWin::resumeGame));
		CCMenu *exitMenu = CCMenu::create(yes,no,NULL);
        exitMenu->alignItemsHorizontally();
        yesX = yes->getPositionX();
        noX = no->getPositionX();
        yes->setPosition(ccp(-400,yes->getPositionY()));
        no->setPosition(ccp(400,no->getPositionY()));
        addChild(winBg);
        exitMenu->setPosition(ccp(VisibleRect::center().x,VisibleRect::center().y-70));
        addChild(exitMenu);
        CCActionInterval *yesMove = CCMoveTo::create(BB_WIN_EXIT_TIME,ccp(yesX,yes->getPositionY()));
        CCActionInterval *noMove = CCMoveTo::create(BB_WIN_EXIT_TIME,ccp(noX,no->getPositionY()));
        yes->runAction(CCSpawn::create(yesMove,fadeIn->copy(),NULL));
        no->runAction(CCSpawn::create(noMove,fadeIn->copy(),NULL));
		exitMenu->setTouchPriority(INT_MIN);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_SHOW_EXIT_WIN);
        return true;
    }
    while (0);
    return false;
}

void ExitWin::initBgLayer()
{
    bgLayer = MaskLayer::create();
	bgLayer->setColor(ccc3(0,0,0));
    bgLayer->setOpacity(0);
    CCActionInterval *fadeIn = CCFadeTo::create(0.5,100);
    bgLayer->runAction(fadeIn);
    addChild(bgLayer);
}

void ExitWin::exitGame( CCObject *pSender )
{
    CCDirector::sharedDirector()->end();
#if (CC_PLATFORM_IOS==CC_TARGET_PLATFORM)
    exit(0);
#endif
}

void ExitWin::resumeGame( CCObject *pSender )
{
    CCActionInterval *yesMove = CCMoveTo::create(BB_WIN_EXIT_TIME,ccp(-400,yes->getPositionY()));
    CCActionInterval *noMove = CCMoveTo::create(BB_WIN_EXIT_TIME,ccp(400,no->getPositionY()));
    CCActionInterval *fadeOut = CCFadeOut::create(BB_WIN_EXIT_TIME);
    yes->runAction(CCSpawn::create(yesMove,fadeOut,NULL));
    no->runAction(CCSpawn::create(noMove,fadeOut->copy(),NULL));
    CCActionInterval *scaleHide = CCScaleTo::create(BB_WIN_EXIT_TIME,0);
    CCCallFunc *destroyHandler = CCCallFunc::create(this,callfunc_selector(ExitWin::destroy));
    CCSequence *hideSeq = CCSequence::create(CCSpawn::create((CCActionInterval*)fadeOut->copy(),CCEaseBackIn::create(scaleHide),NULL),destroyHandler,NULL);
    winBg->runAction(hideSeq);
    bgLayer->runAction(CCFadeTo::create(BB_WIN_EXIT_TIME,0));
}

void ExitWin::destroy()
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_RESUME_GAME);
    removeFromParentAndCleanup(true);
}

void ExitWin::onEnter()
{
	CCLayer::onEnter();
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ExitWin::resumeGame),NOTI_HIDE_EXIT_WIN,NULL);
}

void ExitWin::onExit()
{
	CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTI_HIDE_EXIT_WIN);
}

