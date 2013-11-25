#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "common/common_header.h"

class SoundManager
{
private:
    static SoundManager *_instance;
public:
    void playBackground(const char *sound,bool repeat);
    SoundManager *shareSoundManager();
};

#endif // !_SOUNDMANAGER_H_
