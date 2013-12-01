#ifndef _COMMON_HEADER_H_
#define _COMMON_HEADER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "base/VisibleRect.h"
#include "userdefault_keys.h"
#include "notifications_names.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

#define selector private
#define SPRITE(URL) CCSprite::createWithSpriteFrameName(URL)
#define GET_ANIMATE(NAME) CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName(NAME))

#define UD_GET_BOOL(key) CCUserDefault::sharedUserDefault()->getBoolForKey(key)
#define UD_GET_DOUBLE(key) CCUserDefault::sharedUserDefault()->getDoubleForKey(key)

#define UD_SET_BOOL(key,val) CCUserDefault::sharedUserDefault()->setBoolForKey(key,val)


#endif // !_COMMON_HEADER_H_
