#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include "common/common_header.h"

class PauseMenu:public CCNode
{
private:
    CCNode *m_pMenuNode;
public:
    PauseMenu():m_pMenuNode(NULL){};
    virtual bool init();
    CREATE_FUNC(PauseMenu);
selector:
	void __resumeGame(CCObject *);
	void __quitGame(CCObject *);
};

#endif // !_PAUSEMENU_H_
