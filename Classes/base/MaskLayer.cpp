#include "MaskLayer.h"

void MaskLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,INT_MIN,true);
}

bool MaskLayer::init()
{
    if(!CCLayerColor::init())
    {
        return false;
    }
    setTouchEnabled(true);
    return true;
}

bool MaskLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCLog("touch mask layer");
    return true;
}
