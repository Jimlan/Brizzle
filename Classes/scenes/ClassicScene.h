#ifndef _CLASSICSCENE_H_
#define _CLASSICSCENE_H_

#include "base/BaseScene.h"
#include "components/PauseMenu.h"

class ClassicScene:public BaseScene
{
private:
    void __initBackground();
    void __initPauseSprite();
	void __createBird();
    PauseMenu *m_pPauseMenu;
public:
    virtual void loadAssets();
    virtual bool init();
    CREATE_FUNC(ClassicScene);
    virtual void onEnter();
    virtual void onExit();
    ClassicScene():m_pPauseMenu(NULL) {};
selector:
    /* 背景木头移动完毕后的回调函数 */
    void __woodMoveCall();
    /* 延时后 把背景木头的透明度设置为不透明 */
    void __delayCall(CCNode *node);
    /* 显示暂停按钮 */
    void __showPauseMenu(CCObject *pSender);

	void __resumeGame(CCObject *pSender);
};


#endif // !_CLASSICSCENE_H_
