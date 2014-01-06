#ifndef _SPLASHSCENE_H_
#define _SPLASHSCENE_H_

#include "base/BaseScene.h"

class SplashScene:public BaseScene
{
private:
	void __gotoHome();
public:
    virtual bool init();
	virtual void loadAssets();
    CREATE_FUNC(SplashScene);
};

#endif // !_SPLASHSCENE_H_
