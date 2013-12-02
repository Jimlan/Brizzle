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
	AnimationManager::shareManager()->addAnimation("Blackhole","BlackHoleEff",0,23,12);
	AnimationManager::shareManager()->addAnimation("bomb","BombEff",1,16,12);
	AnimationManager::shareManager()->addAnimation("firebird","FireBirdEff",0,11,12);

}

void ClassicScene::__initBackground()
{
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
    wood->runAction(CCSequence::create(delayRun,delayCall,moveAct,moveCall,NULL));
    addChild(wood);
    CCSprite *grass = CCSprite::create("images/stage_classic/stage_tree_grass_RETINA.png");
    grass->setAnchorPoint(CCPointZero);
    addChild(grass);
}

void ClassicScene::__woodMoveCall()
{
    __createBird();
}


void ClassicScene::__delayCall(CCNode *node)
{
    CCSprite *sprite = (CCSprite*)node;
    sprite->setOpacity(255);
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
    addChild(m_pPauseMenu);
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
	//effectLayer->setPosition(VisibleRect::center());
	effectLayer->setSwallow(false);
	addChild(effectLayer);
	ShareManager::shareManager()->effectLayer = effectLayer;
	//PuzzleUtil::instance()->dash4Bird();
}

