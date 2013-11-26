#ifndef _PANDORASCENE_H_
#define _PANDORASCENE_H_

#include "base/BaseScene.h"

class PandoraScene:public BaseScene
{
private:
	void __initBackground();
public:
	virtual void loadAssets();
	virtual bool init();
	CREATE_FUNC(PandoraScene);
};
#endif // !_PANDORASCENE_H_
