#ifndef _SETTINGSECNE_H_
#define _SETTINGSECNE_H_

#include "base/BaseScene.h"

class SettingScene:public BaseScene
{
private:
	void __soundHandler(CCObject *pSender);
	void __backToHome(CCObject *pSender);
public:
	virtual void onEnter();
	virtual bool init();
	virtual void loadAssets();
	CREATE_FUNC(SettingScene);
};

#endif // !_SETTINGSECNE_H_
