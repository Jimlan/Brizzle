#ifndef _BASEMENUITEMSPRITE_H_
#define _BASEMENUITEMSPRITE_H_

#include "common/common_header.h"

class BaseMenuItemSprite:public CCMenuItemSprite
{
private:
	CCObject *m_target;
	SEL_MenuHandler sel;
	void __delegateSelector(CCObject *pSender);
public:
	static BaseMenuItemSprite *create(CCNode* normalSprite, CCNode* pushSprite,CCNode *target,SEL_MenuHandler sel);
};

#endif // !_BASEMENUITEMSPRITE_H_
