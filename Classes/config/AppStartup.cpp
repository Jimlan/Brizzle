#include "AppStartup.h"

bool AppStartup::init()
{
    //判断是否存在UserDefault生成的xml文件 如果不存在的话 在条件内初始化配置数据
    if(!UD_GET_BOOL(KEY_IS_EXIST))
    {
        UD_SET_BOOL(KEY_IS_EXIST,true);
        UD_SET_BOOL(KEY_SOUND_ENABLE,true);
    }
    return true;
}

