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
    SoundManager::shareSoundManager()->playBackground("sounds/BGM/Play_bgm_long.mp3",true);
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
    CCSprite *progress = SPRITE("stage_tree_gauge_bar@2x.png");
    CCSprite *progressHead = SPRITE("stage_tree_gauge_head@2x.png");
    progressHead->setOpacity(0);
    progress->setOpacity(0);
    contentNode->addChild(progress);
    contentNode->addChild(progressHead);
    CCLabelAtlas *score = CCLabelAtlas::create("./0123456789","images/stage_classic/numwhite-hd.png",20,40,'.');
    score->setString("000000");
	score->setAnchorPoint(ccp(0.5,0.5));
	score->setPosition(ccp(VisibleRect::center().x,-70));
	score->setOpacity(0);
    contentNode->addChild(score);
    progress->setAnchorPoint(ccp(0,0.5));
    progress->setPosition(ccp(76,-117));
    progressHead->setPosition(ccp(progress->getPositionX()+progress->getContentSize().width,-117));
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
        CCNodeRGBA *sprite = (CCNodeRGBA*)child;
        if(sprite)
            sprite->setOpacity(255);
    }
}

void ClassicScene::__initPauseSprite()
{
    BaseSprite *pauseSpr = BaseSprite::create("images/stage_classic/stage_dimm2_RETINA.png");
    pauseSpr->setAnchorPoint(ccp(0,1));
    pauseSpr->setPosition(ccp(15,m_winSize.height-15));
    pauseSpr->setTargetEnded(this,menu_selector(ClassicScene::__showPauseMenu));
    pauseSpr->setOpacity(0);
    addChild(pauseSpr);
}

void ClassicScene::__showPauseMenu( CCObject *pSender )
{
    if(m_pPauseMenu&&m_pPauseMenu->getParent())
    {
        return ;
    }
    SoundManager::shareSoundManager()->playEffect("sounds/SFX/pausebuttonclick.mp3");
    m_pPauseMenu = PauseMenu::create();
    addChild(m_pPauseMenu,3);
}

void ClassicScene::onEnter()
{
    BaseScene::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ClassicScene::__resumeGame),NOTI_RESUME_GAME,NULL);
}

void ClassicScene::onExit()
{
    BaseScene::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,NOTI_RESUME_GAME);
    ShareManager::shareManager()->fstBird = NULL;
    ShareManager::shareManager()->sedBird = NULL;
}

void ClassicScene::__resumeGame( CCObject *pSender )
{
    m_pPauseMenu->removeFromParent();
    m_pPauseMenu = NULL;
}

void ClassicScene::__createBird()
{
    m_pBirdBatchNode = CCSpriteBatchNode::createWithTexture(SPRITE("box00_burn@2x.png")->getTexture());
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
    m_pBirdBatchNode->setPosition(ccp(93,85));
    addChild(m_pBirdBatchNode);
    ShareManager::shareManager()->birdBatchNode = m_pBirdBatchNode;
    effectLayer = ForbiddenLayer::create();
    effectLayer->setSwallow(true);
    addChild(effectLayer);
    ShareManager::shareManager()->effectLayer = effectLayer;
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
	
}

void ClassicScene::__startHandler(CCNode *node)
{
	effectLayer->setSwallow(false);
	node->removeFromParent();
}

