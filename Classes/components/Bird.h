﻿#ifndef _BIRD_H_
#define _BIRD_H_

#include "base/BaseSprite.h"

class Bird:public BaseSprite
{
private:
    void blink();
    CCAnimate *blinkAct;
    /* 记录点击的小鸟 */
    void __recordBird();
    /* 交换记录的小鸟的位置 */
    void __changeBirdPos();

    void __shakeBody();

	void __moveEnd(CCNode *node);
public:
	/* 记录当前是否在移动 如果正在移动的话 就禁止接收事件 */
    bool isMoving;
    short row;
    short col;
	short birdType;
    Bird():birdType(-1),isMoving(false) {};
    static Bird *create(short type);
    virtual void onEnter();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

};


#endif // !_BIRD_H_