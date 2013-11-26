#include "Bird.h"
#include "managers/SoundManager.h"

Bird * Bird::create( short type )
{
    Bird *bird = new Bird();
    CCString *name = CCString::createWithFormat("box0%d_normal_00@2x.png",type);
    if(bird&&bird->initWithSpriteFrameName(name->getCString()))
    {
        bird->autorelease();
        CCArray *frames = CCArray::create();
        CCString *blinkFrame = CCString::createWithFormat("box0%d_normal_02@2x.png",type);
        frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString()));
        frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(blinkFrame->getCString()));
        frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString()));
		CCAnimation *animation = CCAnimation::createWithSpriteFrames(frames,0.2f);
        bird->blinkAct = CCAnimate::create(animation);
        bird->blink();
        return bird;
    }
    return NULL;
}

void Bird::blink()
{
    float delaySec = rand()%5+4.0f;
    CCDelayTime *delayTime = CCDelayTime::create(delaySec);
    CCCallFunc *callfunc = CCCallFunc::create(this,callfunc_selector(Bird::blink));
    runAction(CCSequence::create(delayTime,blinkAct,callfunc,NULL));
}

void Bird::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	SoundManager::shareSoundManager()->playEffect("sounds/SFX/Bird_droped.mp3");
	BaseSprite::ccTouchEnded(pTouch,pEvent);
}

