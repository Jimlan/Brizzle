#ifndef _ICEBREAKSCENE_H_
#define _ICEBREAKSCENE_H_

#include "base/BaseScene.h"

class IcebreakScene:public BaseScene
{
private:
	void __initBackground();
public:
	virtual bool init();
	virtual void loadAssets();
	CREATE_FUNC(IcebreakScene);
};

#endif // !_ICEBREAKSCENE_H_
