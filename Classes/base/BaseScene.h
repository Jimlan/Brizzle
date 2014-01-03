#ifndef _BASESCENE_H_
#define _BASESCENE_H_

#include "common/common_header.h"
#include "managers/SoundManager.h"

#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
#include "ProtocolAds.h"

using namespace cocos2d::plugin;

class MyAdsListener:public AdsListener{
public:
	virtual void onAdsResult(AdsResultCode code, const char* msg);
	virtual void onPlayerGetPoints(ProtocolAds* pAdsPlugin, int points);
};


#endif

class BaseScene:public CCScene
{
#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)

private:
	ProtocolAds *m_pAdmob;
	MyAdsListener *m_pAdsListener;

#endif
protected:
	bool isShowExit;
    CCSize m_winSize;
    CCSpriteFrameCache *m_pFrameCache;
    CCAnimationCache *m_pAnimationCache;
    CCDirector *m_pDirector;
	void showAds();
	void exitGame();
	void exitGameDelegate(float del);
	void resumeGame(CCObject *obj);
	void toast(const char *msg);
public:
    virtual bool init();
	virtual void onEnter();
	virtual void onExit();
    virtual void loadAssets() = 0;
	BaseScene();
};

#endif // !1
