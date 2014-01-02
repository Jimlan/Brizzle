#include "GameOver.h"
#include "base/MaskLayer.h"
#include "base/BaseSprite.h"
#include "managers/ShareManager.h"

bool GameOver::init()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameOver/gameover_CN_RETINA.plist");
    CCNode *contentNode = CCNode::create();
    MaskLayer *maskLayer = MaskLayer::create();
    maskLayer->setTouchEnabled(false);
    maskLayer->touchPriority = -130;
    maskLayer->setTouchEnabled(true);
    addChild(maskLayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("stage_gameover_bg_china@2x.png");
    contentNode->addChild(bg);
    addChild(contentNode);
    contentNode->setPosition(VisibleRect::center());
    CCSprite *normalQuit = SPRITE("stage_gameover_quit_china@2x.png");
    CCSprite *pressedQuit = SPRITE("stage_gameover_quit_china@2x.png");
    CCSprite *restartNormal = SPRITE("stage_gameover_restart_china@2x.png");
    CCSprite *restartPressed = SPRITE("stage_gameover_restart_china@2x.png");
    normalQuit->setOpacity(0);
    restartNormal->setOpacity(0);
    CCMenuItemSprite *quit = CCMenuItemSprite::create(normalQuit,pressedQuit,this,menu_selector(GameOver::quitHandler));
    CCMenuItemSprite *restart = CCMenuItemSprite::create(restartNormal,restartPressed,this,menu_selector(GameOver::restartHandler));
    CCMenu *menu = CCMenu::create(quit,restart,NULL);
    menu->alignItemsHorizontallyWithPadding(20);
    menu->setPositionY(VisibleRect::center().y - 245);
    menu->setTouchPriority(-131);
    menu->ignoreAnchorPointForPosition(false);
    contentNode->addChild(menu);
	BaseSprite *weiboSpr = BaseSprite::create("GameOver/gameover_skynet_RETINA.png");
	contentNode->addChild(weiboSpr);
	weiboSpr->setPosition(ccp(-130,-150));

	CCLabelAtlas *level = CCLabelAtlas::create("./0123456789","images/stage_classic/numwhite-hd.png",20,40,'.');
	level->setString(CCString::createWithFormat("%d",ShareManager::shareManager()->topLevel)->getCString());
	contentNode->addChild(level);
	level->setAnchorPoint(ccp(0,0.5));
	level->setPosition(ccp(10,190));

	CCLabelAtlas *playTime = CCLabelAtlas::create("./0123456789","images/stage_classic/numwhite-hd.png",20,40,'.');
	playTime->setString(CCString::createWithFormat("%d",ShareManager::shareManager()->playTime)->getCString());
	contentNode->addChild(playTime);
	playTime->setAnchorPoint(ccp(0,0.5));
	playTime->setPosition(ccp(10,135));

	CCLabelAtlas *dashCount = CCLabelAtlas::create("./0123456789","images/stage_classic/numwhite-hd.png",20,40,'.');
	dashCount->setString(CCString::createWithFormat("%d",ShareManager::shareManager()->dashCount)->getCString());
	contentNode->addChild(dashCount);
	dashCount->setAnchorPoint(ccp(0,0.5));
	dashCount->setPosition(ccp(10,68));

	CCLabelBMFont *score = CCLabelBMFont::create(CCString::createWithFormat("%d",ShareManager::shareManager()->gameScore)->getCString(),"images/common/alphanum.fnt");
	score->setPosition(ccp(0,-30));
	contentNode->addChild(score);
    return true;
}

void GameOver::quitHandler( CCObject *pSender )
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_GAMEOVER_QUIT);
}

void GameOver::restartHandler( CCObject *pSender )
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_GAMEOVER_RESTART);
}

void GameOver::onExit()
{
	CCNode::onExit();
}


