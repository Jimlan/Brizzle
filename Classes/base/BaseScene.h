#ifndef _BASESCENE_H_
#define _BASESCENE_H_

#include "common/common_header.h"
#include "managers/SoundManager.h"

class BaseScene:public CCScene
{
protected:
    CCSize m_winSize;
    CCSpriteFrameCache *m_pFrameCache;
    CCAnimationCache *m_pAnimationCache;
    CCDirector *m_pDirector;
public:
    virtual bool init();
    virtual void loadAssets() = 0;
    BaseScene();
};

#endif // !1
