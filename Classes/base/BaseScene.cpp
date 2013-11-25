#include "BaseScene.h"

bool BaseScene::init()
{
    if(!CCScene::init())
    {
        return false;
    }
    loadAssets();
    return true;
}

BaseScene::BaseScene()
{
    m_pDirector = CCDirector::sharedDirector();
    m_winSize = m_pDirector->getWinSize();
    m_pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    m_pAnimationCache = CCAnimationCache::sharedAnimationCache();
    m_pFrameCache->removeUnusedSpriteFrames();
    m_pDirector->purgeCachedData();
}

