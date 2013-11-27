#include "ShareManager.h"

ShareManager *ShareManager::_instance = NULL;

ShareManager * ShareManager::shareManager()
{
    if(!_instance)
    {
        _instance = new ShareManager();
    }
    return _instance;
}

