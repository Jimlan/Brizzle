#include "ClassicScene.h"

bool ClassicScene::init()
{
    if(!BaseScene::init())
    {
        return false;
    }
    __initBackground();
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
}

