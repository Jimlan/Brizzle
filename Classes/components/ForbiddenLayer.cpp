#include "ForbiddenLayer.h"

bool ForbiddenLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
	setTouchEnabled(true);
    return true;
}


bool ForbiddenLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    return true;
}

void ForbiddenLayer::setPriority( int priority )
{
    if(m_iPriority!=priority)
    {
        m_iPriority = priority;
        setTouchEnabled(false);
        setTouchEnabled(true);
    }
}

void ForbiddenLayer::setSwallow( bool swallow )
{
	/*if(m_bSwallow!=swallow)
	{
	m_bSwallow = swallow;
	setTouchEnabled(false);
	setTouchEnabled(true);
	}*/
	setTouchEnabled(swallow);
}

void ForbiddenLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
}

void ForbiddenLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_iPriority,m_bSwallow);
}

