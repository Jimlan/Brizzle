#include "IcebreakScene.h"
void IcebreakScene::loadAssets()
{

}

bool IcebreakScene::init()
{
	if(!BaseScene::init())
	{
		return false;
	}
	__initBackground();
	SoundManager::shareSoundManager()->playBackground("sounds/BGM/Play_bgm_long",true);
	return true;
}

void IcebreakScene::__initBackground()
{
	CCSprite *bg = CCSprite::create("images/stage_ice/ice_bg_RETINA.png");
	bg->setPosition(VisibleRect::center());
	addChild(bg);
}

