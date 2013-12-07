#ifndef _BIRD_H_
#define _BIRD_H_

#include "base/BaseSprite.h"

class Bird:public BaseSprite
{
private:
    void blink();
    CCAnimate *blinkAct;
    /* 记录点击的小鸟 */
    void __recordBird();
    
public:
	//是不是拐弯的地方
	bool isCorner;
	const char *effectTypeName;
	//和小鸟跟随的动画
	CCSprite *effectSprite;
	//特效类型
	int effectType;
	//小鸟颤动身体的动作
	void shakeBody(float scaleX,float scaleY);
	/* 记录当前是否在移动 如果正在移动的话 就禁止接收事件 */
    bool isMoving;
	/* 是否被检测过了 */
	bool isChecked;
    short row;
    short col;
	short birdType;
    Bird():isCorner(false),effectTypeName("normaltype"),effectSprite(NULL),effectType(0),birdType(-1),isMoving(false),isChecked(false) {
		m_bSwallow = false;
	};
    static Bird *create(short type);
    virtual void onEnter();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
};


#endif // !_BIRD_H_
