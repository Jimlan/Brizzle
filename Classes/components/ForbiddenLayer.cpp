#include "ForbiddenLayer.h"

bool ForbiddenLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }

    return true;
}

void ForbiddenLayer::onEnter()
{
	CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_iPriority,m_bSwallow);
}

void ForbiddenLayer::onExit()
{
	CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool ForbiddenLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLog("forbidden layer touched");
    return true;
}

void ForbiddenLayer::setTouchEnable( bool enabled )
{
    if (m_bTouchEnable!= enabled)
    {
        m_bTouchEnable = enabled;
        if (m_bRunning)
        {
            if (enabled)
            {
				CCLog("set swallow true");
                CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_iPriority,m_bSwallow);
            }
            else
            {
                CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
            }
        }
    }
}

void ForbiddenLayer::setPriority( int priority )
{
    if(m_iPriority!=priority)
    {
        m_iPriority = priority;
        if(m_bTouchEnable)
        {
            setTouchEnable(false);
            setTouchEnable(true);
        }
    }
}

void ForbiddenLayer::setSwallow( bool swallow )
{
    if(m_bSwallow!=swallow)
    {
        m_bSwallow = swallow;
        if(m_bTouchEnable)
        {
            setTouchEnable(false);
            setTouchEnable(true);
        }
    }
}

