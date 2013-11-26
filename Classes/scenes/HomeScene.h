#ifndef _HOMESCENE_H_
#define _HOMESCENE_H_

#include "base/BaseScene.h"

class HomeScene:public BaseScene
{
private:
    /* 设置场景的背景图片 */
    void __initBackground();
    /* 添加旋转的光芒 */
    void __initLighting();
    /* 初始化主页按钮 */
    void __initMainMenu();
public:
    virtual bool init();
    virtual void loadAssets();
    CREATE_FUNC(HomeScene);
selector:
	/* 切换到经典模式场景 */
	void __gotoClassic(CCObject *pSender);
	/* 切换到潘多拉模式 */
	void __gotoPandora(CCObject *pSender);
	/* 切换到破冰模式 */
	void __gotoIcebreak(CCObject *pSender);
};

#endif // !_HOMESCENE_H_
