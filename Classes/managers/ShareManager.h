#ifndef _SHAREMANAGER_H_
#define _SHAREMANAGER_H_

#include "common/common_header.h"
#include "components/ForbiddenLayer.h"

class Bird;

class ShareManager:public CCObject
{

public:
	int dashCount;
	int topLevel;
	int playTime;
	int gameScore;

private:
    static ShareManager *_instance;
public:
	bool isGameOver;
	bool isGamePause;
	/* 每次移动所消除的小鸟 */
	int dashBirdsNum;
	/* 出现的小鸟的类型 */
	int birdTypes;
    static const short row = 9;
    static const short col = 7;
    Bird *birds[row][col];
    static const int boxWidth = 76;
    static const int boxHeight = 76;
    static ShareManager *shareManager();
    ShareManager():dashCount(0),topLevel(1),playTime(0),gameScore(0),isGameOver(false),isGamePause(false),fstBird(NULL),
        sedBird(NULL),birdBatchNode(NULL),birdTypes(4),dashBirdsNum(0) {};
    Bird *fstBird;
    Bird *sedBird;
	CCSpriteBatchNode *birdBatchNode;
	ForbiddenLayer *effectLayer;
	
};

#endif // !_SHAREMANAGER_H_
