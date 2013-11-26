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

void AnimationManager::addAnimation( const char *animationName )
{

}

