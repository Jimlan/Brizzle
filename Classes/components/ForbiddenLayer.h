#ifndef _FORBIDDENLAYER_H_
#define _FORBIDDENLAYER_H_

#include "common/common_header.h"

class ForbiddenLayer:public CCLayer
{
public:
	bool m_bSwallow;
	int m_iPriority;
	void setPriority(int priority);
	void setSwallow(bool swallow);
	ForbiddenLayer():m_bSwallow(true),m_iPriority(INT_MIN){};
	virtual bool init();
	CREATE_FUNC(ForbiddenLayer);
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif // !_FORBIDDENLAYER_H_
