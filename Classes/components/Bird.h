#ifndef _BIRD_H_
#define _BIRD_H_

#include "base/BaseSprite.h"

class Bird:public BaseSprite
{
private:
	void blink();
	CCAnimate *blinkAct;
public:
	static Bird *create(short type);
};


#endif // !_BIRD_H_
