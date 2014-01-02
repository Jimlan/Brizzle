#ifndef _MASKLAYER_H_
#define _MASKLAYER_H_

#include "common/common_header.h"
/************************************************************************/
/* 继承自CCLayerColor 可以屏蔽其他的接收事件的显示对象                    */
/************************************************************************/
class MaskLayer:public CCLayerColor
{
public:
	int touchPriority;
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	MaskLayer():touchPriority(INT_MIN){

	};
    CREATE_FUNC(MaskLayer);
};

#endif // !_MASKLAYER_H_
