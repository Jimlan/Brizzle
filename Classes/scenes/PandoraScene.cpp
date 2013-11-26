#include "PandoraScene.h"
void PandoraScene::loadAssets()
{

}

bool PandoraScene::init()
{
	if(!BaseScene::init())
	{
		return false;
	}
	__initBackground();
	SoundManager::shareSoundManager()->playBackground("sounds/BGM/pandora_bgm.mp3",true);
	return true;
}

void PandoraScene::__initBackground()
{
	CCSprite *bg = CCSprite::create("images/stage_pandora/pandora_bg_B_RETINA.png");
	bg->setPosition(VisibleRect::center());
	addChild(bg);
}

