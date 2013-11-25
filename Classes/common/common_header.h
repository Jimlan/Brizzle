#ifndef _COMMON_HEADER_H_
#define _COMMON_HEADER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "base/VisibleRect.h"
#include "userdefault_keys.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

#define selector private
#define SPRITE(URL) CCSprite::createWithSpriteFrameName(URL)


#define UD_GET_BOOL(key) CCUserDefault::sharedUserDefault()->getBoolForKey(key)
#define UD_GET_DOUBLE(key) CCUserDefault::sharedUserDefault()->getDoubleForKey(key)



#endif // !_COMMON_HEADER_H_
