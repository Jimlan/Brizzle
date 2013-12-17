#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "common/common_header.h"

#if(CC_PLATFORM_WIN32==CC_TARGET_PLATFORM)
#define suffix std::string(".mp3")
#else
#define suffix std::string(".ogg")
#endif

class SoundManager
{
private:
    static SoundManager *_instance;
public:
	void playEffect(const char *sound);
    void playBackground(const char *sound,bool repeat);
    static SoundManager *shareSoundManager();
};

#endif // !_SOUNDMANAGER_H_
