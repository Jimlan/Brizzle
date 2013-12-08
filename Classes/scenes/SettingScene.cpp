#include "SettingScene.h"
#include "base/BaseSprite.h"
#include "HomeScene.h"

void SettingScene::onEnter()
{
	BaseScene::onEnter();
	//背景
	CCSprite *settingBg = CCSprite::create("images/options/option_bg_RETINA.png");
	settingBg->setPosition(VisibleRect::center());
	addChild(settingBg);
	//title
	CCSprite *title = SPRITE("option_soundandvibration_china@2x.png");
	title->setPosition(ccp(VisibleRect::center().x,VisibleRect::top().y-120));
	addChild(title);
	//credits
	CCSprite *credits = SPRITE("option_credit_china@2x.png");
	credits->setPosition(ccp(VisibleRect::center().x,VisibleRect::top().y-530));
	addChild(credits);
	//home
	BaseSprite *homeSpr = BaseSprite::createSpriteWithFrameName("scores_home@2x.png");
	homeSpr->setPosition(ccp(VisibleRect::right().x-68,68));
	homeSpr->setTargetEnded(this,menu_selector(SettingScene::__backToHome));
	addChild(homeSpr);
	//声音
	CCSprite *soundIcon = SPRITE("option_icon_sound@2x.png");
	CCSprite *soundText = SPRITE("option_sound_china@2x.png");
	soundIcon->setPosition(ccp(100,VisibleRect::top().y-200));
	soundText->setPosition(ccp(150,VisibleRect::top().y-200));
	addChild(soundIcon);
	addChild(soundText);
	//声音开关
	CCMenuItemSprite *soundOn = CCMenuItemSprite::create(SPRITE("option_button_on@2x.png"),SPRITE("option_button_on@2x.png"));
	CCMenuItemSprite *soundOff = CCMenuItemSprite::create(SPRITE("option_button_off@2x.png"),SPRITE("option_button_off@2x.png"));
	CCMenuItemToggle *sound = CCMenuItemToggle::createWithTarget(this,menu_selector(SettingScene::__soundHandler),soundOn,soundOff,NULL);
	CCMenu *soundMenu = CCMenu::create(sound,NULL);
	soundMenu->alignItemsVertically();
	soundMenu->setPosition(ccp(500,VisibleRect::top().y-200));
	addChild(soundMenu);
	if(UD_GET_BOOL(KEY_SOUND_ENABLE))
	{
		sound->setSelectedIndex(0);
	}else{
		sound->setSelectedIndex(1);
	}
}

bool SettingScene::init()
{
	if(!BaseScene::init())
	{
		return false;
	}

	return true;
}

void SettingScene::loadAssets()
{
	m_pFrameCache->addSpriteFramesWithFile("images/options/option_CN_RETINA.plist");
	m_pFrameCache->addSpriteFramesWithFile("images/options/option_RETINA.plist");
}

void SettingScene::__soundHandler(CCObject *pSender)
{
	CCMenuItemToggle *sound = (CCMenuItemToggle*)pSender;
	if(sound->getSelectedIndex()==0)
	{
		UD_SET_BOOL(KEY_SOUND_ENABLE,true);
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	else
	{
		UD_SET_BOOL(KEY_SOUND_ENABLE,false);
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void SettingScene::__backToHome( CCObject *pSender )
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(0.5f,HomeScene::create()));
}

