﻿#ifndef _SHAREMANAGER_H_
#define _SHAREMANAGER_H_

#include "common/common_header.h"
#include "components/Bird.h"

class ShareManager:public CCObject
{
private:
    static ShareManager *_instance;
public:
	static const short row = 9;
	static const short col = 7;
	Bird *birds[row][col];
	static const int boxWidth = 76;
	static const int boxHeight = 76;
    static ShareManager *shareManager();
    ShareManager():fstBird(NULL),
        sedBird(NULL) {};
    Bird *fstBird;
    Bird *sedBird;
};

#endif // !_SHAREMANAGER_H_