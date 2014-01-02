#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "common/common_header.h"

class GameOver:public CCNode
{
public:
	void quitHandler(CCObject *pSender);
	void restartHandler(CCObject *pSender);
	virtual bool init();
	virtual void onExit();
	CREATE_FUNC(GameOver);
};

#endif // !_GAMEOVER_H_
