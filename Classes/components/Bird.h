#ifndef _BIRD_H_
#define _BIRD_H_

#include "base/BaseSprite.h"

class Bird:public BaseSprite
{
private:
	void blink();
	CCAnimate *blinkAct;
public:
	short row;
	short col;
	static Bird *create(short type);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	
};


#endif // !_BIRD_H_
