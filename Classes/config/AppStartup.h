#ifndef _APPSTARTUP_H_
#define _APPSTARTUP_H_
#include "common/common_header.h"
class AppStartup:CCObject
{
public:
	virtual bool init();
	CREATE_FUNC(AppStartup);
};

#endif // !_APPSTARTUP_H_
