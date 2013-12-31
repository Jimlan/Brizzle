#ifndef _EXITWIN_H_
#define _EXITWIN_H_

#include "common/common_header.h"
#include "base/MaskLayer.h"

class ExitWin:public CCLayer
{
private:
	CCLayerColor *bgLayer;
	CCMenuItemSprite *yes;
	CCMenuItemSprite *no;
	CCSprite *winBg;
	void initBgLayer();
	void destroy();
	float yesX;
	float noX;
public:
	virtual void onEnter();
	virtual void onExit();
	void exitGame(CCObject *pSender);
	void resumeGame(CCObject *pSender);
    virtual bool init();
    CREATE_FUNC(ExitWin);
};

#endif // !_EXITWIN_H_
