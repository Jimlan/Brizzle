#include "ClassicScene.h"
#include "base/BaseSprite.h"
#include "components/PauseMenu.h"
#include "components/Bird.h"
#include "managers/ShareManager.h"
#include "managers/AnimationManager.h"
#include "util/PuzzleUtil.h"
#include "components/ForbiddenLayer.h"

bool ClassicScene::init()
{
    if(!BaseScene::init())
    {
        return false;
    }
    __initBackground();
    __initPauseSprite();
    SoundManager::shareSoundManager()->playBackground("sounds/BGM/Play_bgm_long",true);
    return true;
}

void ClassicScene::loadAssets()
{
    m_pFrameCache->addSpriteFramesWithFile("images/character/Character_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/stage_classic/stage_RETINA.plist");
    //小鸟特效纹理
    m_pFrameCache->addSpriteFramesWithFile("images/item/itemblackhole_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/item/itembomb_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/item/itemfirebird_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/item/itemlightning_RETINA.plist");
    //执行特效时候的纹理
    m_pFrameCache->addSpriteFramesWithFile("images/item_effect/effectblackhole_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/item_effect/effectbomb_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/item_effect/effectfirebird_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/item_effect/cloud_RETINA.plist");
    //小鸟特效动画
    AnimationManager::shareManager()->addAnimation("itemBlackhole","BlackHole",0,11,12);
    AnimationManager::shareManager()->addAnimation("itemBomb","bomb",0,11,12);
    AnimationManager::shareManager()->addAnimation("itemFirebird","FireBird",0,11,12);
    AnimationManager::shareManager()->addAnimation("itemLightning","lightning",0,11,12);
    //特效执行时候的动画
    AnimationManager::shareManager()->addAnimation("Blackhole","BlackHoleEff",0,23,24);
    AnimationManager::shareManager()->addAnimation("bomb","BombEff",1,16,50);
    AnimationManager::shareManager()->addAnimation("firebird","FireBirdEff",0,11,24);

}

void ClassicScene::__initBackground()
{
    CCNode *contentNode = CCNode::create();
    CCSprite *bg = CCSprite::create("images/stage_classic/stage_bg_RETINA.png");
    bg->setPosition(VisibleRect::center());
    addChild(bg);

    CCSprite *wood = CCSprite::create("images/stage_classic/stage_tree_RETINA.png");
    wood->setAnchorPoint(ccp(0.5,0));
    wood->setPosition(ccp(m_winSize.width/2,-882));

    CCDelayTime *delayRun = CCDelayTime::create(1);
    CCMoveBy *moveAct = CCMoveBy::create(0.5,ccp(0,882));
    CCCallFunc *moveCall = CCCallFunc::create(this,callfunc_selector(ClassicScene::__woodMoveCall));
    CCCallFuncN *delayCall = CCCallFuncN::create(this,callfuncN_selector(ClassicScene::__delayCall));
    wood->setOpacity(0);
    contentNode->addChild(wood,0,0);

    progressNode = CCNode::create();
    CCSprite *progress = SPRITE("stage_tree_gauge_bar@2x.png");
    CCSprite *progressHead = SPRITE("stage_tree_gauge_head@2x.png");
    progressNode->addChild(progress);
    progressNode->addChild(progressHead);
    progressHead->setAnchorPoint(CCPointZero);
    progress->setAnchorPoint(CCPointZero);
    progressHead->setPosition(ccp(progress->getPositionX()+progress->getContentSize().width,0));

    /* 进度条的遮罩 */
    CCClippingNode *progressClipNode  = CCClippingNode::create();
    progressClipNode->setContentSize(CCSizeMake(progress->getContentSize().width+progressHead->getContentSize().width,progress->getContentSize().height));
    progressNode->setContentSize(progressClipNode->getContentSize());
    CCDrawNode *stencilNode  = CCDrawNode::create();

    CCPoint points[4];
    points[0] = ccp(0, 0);
    points[1] = ccp(progressClipNode->getContentSize().width, 0);
    points[2] = ccp(progressClipNode->getContentSize().width, progressClipNode->getContentSize().height);
    points[3] = ccp(0, progressClipNode->getContentSize().height);
    ccColor4F white = {1, 1, 1, 1};
    stencilNode->drawPolygon(points, 4, white, 1, white);
    progressClipNode->setStencil(stencilNode);
    progressClipNode->setPosition(ccp(76,-128));
    contentNode->addChild(progressClipNode);
    progressClipNode->addChild(progressNode);
    contentNode->runAction(CCSequence::create(delayRun,delayCall,moveAct,moveCall,NULL));
    addChild(contentNode);
    CCSprite *grass = CCSprite::create("images/stage_classic/stage_tree_grass_RETINA.png");
    grass->setAnchorPoint(CCPointZero);
    addChild(grass);
    grass->setZOrder(1);
}

void ClassicScene::__woodMoveCall()
{
    __createBird();
    __ready();
}


void ClassicScene::__delayCall(CCNode *node)
{
    CCArray *children = node->getChildren();
    CCObject *child = NULL;
    CCARRAY_FOREACH(children,child)
    {
        CCNodeRGBA *sprite = dynamic_cast<CCNodeRGBA*>(child);
        if(sprite)
            sprite->setOpacity(255);
    }
}

void ClassicScene::__initPauseSprite()
{
    pauseSpr = BaseSprite::create("images/stage_classic/stage_dimm2_RETINA.png");
    pauseSpr->setAnchorPoint(ccp(0,1));
    pauseSpr->setPosition(ccp(15,m_winSize.height-15));
    pauseSpr->setTargetEnded(this,menu_selector(ClassicScene::__showPauseMenu));
    pauseSpr->setOpacity(0);
    addChild(pauseSpr);
}

void ClassicScene::__showPauseMenu( CCObject *pSender )
{
    pauseSpr->setTouchEnabled(false);
    SoundManager::shareSoundManager()->playEffect("sounds/SFX/pausebuttonclick");
    m_pPauseMenu = PauseMenu::create();
    addChild(m_pPauseMenu,3);
    ShareManager::shareManager()->isGamePause = true;
}

void ClassicScene::onEnter()
{
    BaseScene::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicScene::__resumeGame),NOTI_CLOSE_PAUSE_MENU,NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicScene::__showScore),NOTI_SHOW_SCORE,NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicScene::__onExitShow),NOTI_SHOW_EXIT_WIN,NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicScene::__timeReward),NOTI_TIME_REWARD,NULL);
}

void ClassicScene::onExit()
{
    BaseScene::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTI_CLOSE_PAUSE_MENU);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTI_SHOW_SCORE);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTI_SHOW_EXIT_WIN);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTI_TIME_REWARD);
    ShareManager::shareManager()->fstBird = NULL;
    ShareManager::shareManager()->sedBird = NULL;
}

void ClassicScene::__resumeGame( CCObject *pSender )
{
    pauseSpr->setTouchEnabled(true);
    ShareManager::shareManager()->isGamePause = false;
}

void ClassicScene::__createBird()
{
    m_pBirdBatchNode = CCSpriteBatchNode::createWithTexture(SPRITE("box00_burn@2x.png")->getTexture());
    ShareManager *sm = ShareManager::shareManager();
    PuzzleUtil::instance()->createBirds();
    for(short row = 0; row<9; row++)
    {
        for(short col=0; col<7; col++)
        {
            Bird *bird = ShareManager::shareManager()->birds[row][col];
            bird->setScale(0.9f);
            bird->setPosition(ccp(col*ShareManager::boxWidth,row*ShareManager::boxHeight));
            m_pBirdBatchNode->addChild(bird);
        }
    }
    m_pBirdBatchNode->setPosition(ccp(ShareManager::boxWidth/2,ShareManager::boxHeight/2));
    sm->birdBatchNode = m_pBirdBatchNode;
    effectLayer = ForbiddenLayer::create();
    m_pScoreNode = CCNode::create();

    sm->effectLayer = effectLayer;

    scoreLabel = CCLabelAtlas::create("./0123456789","images/stage_classic/numwhite-hd.png",20,40,'.');
    scoreLabel->setString("000000");
    scoreLabel->setAnchorPoint(ccp(0.5,0.5));
    scoreLabel->setPosition(ccp(VisibleRect::center().x,VisibleRect::top().y-140));
    m_pScoreNode->addChild(scoreLabel);

    /* 此处添加遮罩层 */
    CCDrawNode *stencil = CCDrawNode::create();
    CCClippingNode *clippingNode = CCClippingNode::create();
    clippingNode->setContentSize(CCSizeMake(ShareManager::boxWidth*7,ShareManager::boxHeight*9+10));
    clippingNode->setPosition(ccp(55,47));

    CCPoint rectangle[4];
    rectangle[0] = ccp(0, 0);
    rectangle[1] = ccp(clippingNode->getContentSize().width, 0);
    rectangle[2] = ccp(clippingNode->getContentSize().width, clippingNode->getContentSize().height);
    rectangle[3] = ccp(0, clippingNode->getContentSize().height);
    ccColor4F white = {1, 1, 1, 1};
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clippingNode->setStencil(stencil);
    clippingNode->addChild(m_pBirdBatchNode);
    addChild(clippingNode);
    addChild(effectLayer);
    addChild(m_pScoreNode);
}

void ClassicScene::__ready()
{
    CCSprite *ready = SPRITE("stage_level_ready@2x.png");
    ready->setPosition(ccp(VisibleRect::top().x,VisibleRect::top().y+100));
    CCMoveTo *moveTo = CCMoveTo::create(0.8f,VisibleRect::center());
    CCEaseBackOut *backOut = CCEaseBackOut::create(moveTo);
    CCDelayTime *delay = CCDelayTime::create(0.5f);
    CCMoveBy *moveOut = CCMoveBy::create(0.2f,ccp(-500,0));
    CCCallFunc *endCall = CCCallFunc::create(ready,callfunc_selector(CCSprite::removeFromParent));
    CCCallFunc *startCall = CCCallFunc::create(this,callfunc_selector(ClassicScene::__start));
    CCDelayTime *delayStart = CCDelayTime::create(0.3f);
    ready->runAction(CCSequence::create(backOut,delay,moveOut,endCall,startCall,NULL));
    addChild(ready);
}

void ClassicScene::__start()
{
    CCSprite *start = SPRITE("stage_level_start@2x.png");
    start->setPosition(VisibleRect::center());
    start->setScale(0);
    CCScaleTo *scaleTo = CCScaleTo::create(0.5f,1);
    CCEaseBackOut *easeOut = CCEaseBackOut::create(scaleTo);
    CCDelayTime *delay = CCDelayTime::create(0.5f);
    CCScaleTo *scaleOut = CCScaleTo::create(0.5f,3);
    CCFadeOut *fadeOut = CCFadeOut::create(0.5f);
    CCEaseBackIn *backin = CCEaseBackIn::create(scaleOut);
    CCSpawn *out = CCSpawn::create(backin,fadeOut,NULL);
    CCCallFuncN *endCall = CCCallFuncN::create(this,callfuncN_selector(ClassicScene::__startHandler));
    CCSequence *startSeq = CCSequence::create(easeOut,delay,out,endCall,NULL);
    addChild(start);
    start->runAction(startSeq);
    schedule(schedule_selector(ClassicScene::progressUpdate),0.05f);
}

void ClassicScene::__startHandler(CCNode *node)
{
    node->removeFromParent();
    effectLayer->setSwallow(false);
}
/* 显示单次消除得分的动画 */
void ClassicScene::__showScore( CCObject *data )
{
    int currentScore = CCString::createWithFormat(scoreLabel->getString())->intValue();
    int singleScore = ShareManager::shareManager()->dashBirdsNum*5;
    currentScore += singleScore;
    CCString *scoreStr = CCString::createWithFormat("%06d",currentScore);
    scoreLabel->setString(scoreStr->getCString());
    CCString *scoreNum = CCString::createWithFormat("%d",singleScore);
    CCLabelBMFont *score = CCLabelBMFont::create(scoreNum->getCString(),"images/common/alphanum.fnt");
    score->setPosition(VisibleRect::center());
    m_pScoreNode->addChild(score);
    score->setScale(0);
    CCScaleTo *scale = CCScaleTo::create(0.5f,1);
    CCEaseBackOut *scaleOut = CCEaseBackOut::create(scale);
    CCScaleTo *scaleTo = CCScaleTo::create(0.5f,0.2f);
    CCFadeOut *fadeOut = CCFadeOut::create(0.5f);
    CCMoveTo *moveAct = CCMoveTo::create(0.5f,ccp(VisibleRect::center().x,VisibleRect::top().y-140));
    CCSpawn *spawn = CCSpawn::create(scaleTo,fadeOut,moveAct,NULL);
    CCCallFunc *moveCall = CCCallFunc::create(score,callfunc_selector(CCLabelBMFont::removeFromParent));
    score->runAction(CCSequence::create(scaleOut,CCDelayTime::create(0.5f),spawn,moveCall,NULL));
}

void ClassicScene::progressUpdate( float del )
{
    if(ShareManager::shareManager()->isGamePause)
    {
        return;
    }
    passSeconds += del;
    float percent = passSeconds/totalSeconds;
    float offset = percent*progressNode->getContentSize().width+20;
    progressNode->setPositionX(-offset);
    if(passSeconds>=totalSeconds)
    {
        ShareManager::shareManager()->isGameOver = true;
        CCMessageBox("GameOver","TIPS");
        effectLayer->setSwallow(true);
        unschedule(schedule_selector(ClassicScene::progressUpdate));
    }
}

void ClassicScene::__onExitShow( CCObject *pSender )
{
    ShareManager::shareManager()->isGamePause = true;
}

void ClassicScene::__timeReward( CCObject *obj )
{
	CCFloat *time = dynamic_cast<CCFloat*>(obj);
	if(time)
	{
		CCString *timeStr = CCString::createWithFormat("+%dS",int(time));
		CCLabelBMFont *rewardTip = CCLabelBMFont::create(timeStr->getCString(),"images/common/alphanum.fnt");
		effectLayer->addChild(rewardTip);
		rewardTip->setScale(0);
		rewardTip->setPosition(ccp(700,800));
		CCScaleTo *scaleAct = CCScaleTo::create(0.5f,1);
		CCEaseElasticOut *easeAct = CCEaseElasticOut::create(scaleAct);
		CCDelayTime *delayAct = CCDelayTime::create(0.5f);
		CCCallFunc *delayCall = CCCallFunc::create(rewardTip,callfunc_selector(CCLabelBMFont::removeFromParent));
		rewardTip->runAction(CCSequence::create(easeAct,delayAct,delayCall,NULL));
		passSeconds -= time->getValue();
		if(passSeconds<0)
		{
			passSeconds = 0;
		}
	}
}


