#ifndef _HOMESCENE_H_
#define _HOMESCENE_H_

#include "base/BaseScene.h"

class HomeScene:public BaseScene
{
private:
	void __initBackground();
	void __initLighting();
public:
    virtual bool init();
    virtual void loadAssets();
    CREATE_FUNC(HomeScene);
};

#endif // !_HOMESCENE_H_
