#include "SoundManager.h"

SoundManager *SoundManager::_instance = NULL;

SoundManager * SoundManager::shareSoundManager()
{
    if(!_instance)
    {
        _instance = new SoundManager();
    }
    return _instance;
}

void SoundManager::playBackground( const char *sound,bool repeat )
{
    bool soundEnable = CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_SOUND_ENABLE);
    if(soundEnable)
    {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(sound,repeat);
    }
}
