#include "HomeScene.h"
#include "base/BaseMenuItemSprite.h"
#include "ClassicScene.h"
#include "PandoraScene.h"
#include "IcebreakScene.h"
#include "SettingScene.h"

void HomeScene::loadAssets()
{
    m_pFrameCache->addSpriteFramesWithFile("images/main_menu/main_bg_a_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/main_menu/main_bg_b_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/main_menu/mainmenu_a_RETINA.plist");
    m_pFrameCache->addSpriteFramesWithFile("images/main_menu/mainmenu_b_CN_RETINA.plist");
	m_pFrameCache->addSpriteFramesWithFile("images/main_menu/mainmenu_a_CN_RETINA.plist");
}

void HomeScene::__initBackground()
{
    CCSprite *bgSpr = SPRITE("main_bg_c@2x.png");
    bgSpr->setAnchorPoint(ccp(0.5,1));
    bgSpr->setPosition(VisibleRect::top());
    addChild(bgSpr);

    CCSprite *grassB = SPRITE("main_bg_b@2x.png");
    grassB->setAnchorPoint(CCPointZero);
    grassB->setPosition(ccp(0,90));
    addChild(grassB);

    CCSprite *grassA = SPRITE("main_bg_a@2x.png");
    grassA->setAnchorPoint(CCPointZero);
    addChild(grassA);

    CCSprite *treeA = SPRITE("main_trees_a@2x.png");
    treeA->setAnchorPoint(ccp(0,1));
    treeA->setPosition(VisibleRect::leftTop());
    addChild(treeA);

    CCSprite *treeB = SPRITE("main_trees_b@2x.png");
    treeB->setAnchorPoint(ccp(1,1));
    treeB->setPosition(VisibleRect::rightTop());
    addChild(treeB);

}

bool HomeScene::init()
{
    if(!BaseScene::init())
    {
        return false;
    }
    __initBackground();
    __initLighting();
    __initMainMenu();
	SoundManager::shareSoundManager()->playBackground("sounds/BGM/Main_bgm",true);
    return true;
}

void HomeScene::__initLighting()
{
    const int deg = 30;
    CCSpriteBatchNode *pLightingNode = CCSpriteBatchNode::createWithTexture(SPRITE("main_bg_light@2x.png")->getTexture());
    for(int i=0; i<6; i++)
    {
        CCSprite *light = SPRITE("main_bg_light@2x.png");
        light->setRotation(i*deg);
        light->setScale(2);
        pLightingNode->addChild(light);
    }

    ccBlendFunc cbl = {GL_DST_COLOR, GL_ONE};
    pLightingNode->setBlendFunc(cbl);
    pLightingNode->setPosition(VisibleRect::center());
    addChild(pLightingNode);
    //使光芒开始旋转
    CCRotateBy *rotateAct = CCRotateBy::create(1,30);
    pLightingNode->runAction(CCRepeatForever::create(rotateAct));
}

void HomeScene::__initMainMenu()
{
    CCMenuItemSprite *classic = BaseMenuItemSprite::create(SPRITE("main_menu_classic_china@2x.png"),SPRITE("main_menu_classic_push_china@2x.png"),this,menu_selector(HomeScene::__gotoClassic));
    CCMenuItemSprite *icebreak = BaseMenuItemSprite::create(SPRITE("main_menu_icebreak_china@2x.png"),SPRITE("main_menu_icebreak_push_china@2x.png"),this,menu_selector(HomeScene::__gotoIcebreak));
    CCMenuItemSprite *pandora = BaseMenuItemSprite::create(SPRITE("main_menu_pandora_china@2x.png"),SPRITE("main_menu_pandora_push_china@2x.png"),this,menu_selector(HomeScene::__gotoPandora));
    CCMenuItemSprite *setting = BaseMenuItemSprite::create(SPRITE("main_option_china@2x.png"),SPRITE("main_option_push_china@2x.png"),this,menu_selector(HomeScene::__gotoSettingScene));
	CCMenuItemSprite *rank = BaseMenuItemSprite::create(SPRITE("main_rank_china@2x.png"),SPRITE("main_rank_push_china@2x.png"),this,NULL);
	CCMenu *mainMenu = CCMenu::create(classic,icebreak,pandora,setting,rank,NULL);
    addChild(mainMenu);
	rank->setPosition(ccp(-130,-365));
	setting->setPosition(ccp(-250,-365));
	classic->setPosition(ccp(0,130));
	pandora->setPosition(ccp(0,-130));
}

void HomeScene::__gotoClassic( CCObject *pSender )
{
	m_pDirector->replaceScene(CCTransitionFlipAngular::create(1,ClassicScene::create()));
}

void HomeScene::__gotoPandora( CCObject *pSender )
{
	m_pDirector->replaceScene(CCTransitionSplitRows::create(0.5,PandoraScene::create()));
}

void HomeScene::__gotoIcebreak( CCObject *pSender )
{
	m_pDirector->replaceScene(CCTransitionTurnOffTiles::create(0.5,IcebreakScene::create()));
}

void HomeScene::__gotoSettingScene( CCObject *pSender )
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.5f,SettingScene::create()));
}

