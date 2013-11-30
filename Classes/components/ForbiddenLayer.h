#ifndef _FORBIDDENLAYER_H_
#define _FORBIDDENLAYER_H_

#include "common/common_header.h"

class ForbiddenLayer:public CCLayer
{
public:
	bool m_bSwallow;
	bool m_bTouchEnable;
	int m_iPriority;
	void setTouchEnable(bool enabled);
	void setPriority(int priority);
	void setSwallow(bool swallow);
	ForbiddenLayer():m_bSwallow(true),m_bTouchEnable(true),m_iPriority(MININT){};
	virtual bool init();
	CREATE_FUNC(ForbiddenLayer);
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

#endif // !_FORBIDDENLAYER_H_
