#include "ClassicScene.h"
#include "base/BaseSprite.h"
#include "components/PauseMenu.h"

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
    //m_pFrameCache->addSpriteFramesWithFile("");
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

}

void ClassicScene::__resumeGame( CCObject *pSender )
{
	m_pPauseMenu->removeFromParent();
	m_pPauseMenu = NULL;
}

