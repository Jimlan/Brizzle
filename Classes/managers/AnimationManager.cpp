#include "AnimationManager.h"

AnimationManager *AnimationManager::_instance = NULL;

AnimationManager * AnimationManager::shareManager()
{
    if(!_instance)
    {
        _instance = new AnimationManager();
    }

    return _instance;
}

void AnimationManager::addAnimation(const char *prefix, const char *animationName,int began,int end,float fps )
{
	CCArray *frames = CCArray::create();
	CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for(int i=began;i<=end;i++)
	{ 
		std::string suffix;
		if(i<10){
			suffix = "_00%d.png";
		}else{
			suffix = "_0%d.png";
		}
		CCString *frameName = CCString::createWithFormat((prefix+suffix).c_str(),i);
		CCSpriteFrame *frame = frameCache->spriteFrameByName(frameName->getCString());
		frames->addObject(frame);
	}
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(frames,1.0/fps);
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation,animationName);
}

