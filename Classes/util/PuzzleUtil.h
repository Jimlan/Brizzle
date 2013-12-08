#ifndef _PUZZLEUTIL_H_
#define _PUZZLEUTIL_H_

#include "components/Bird.h"
#include "managers/ShareManager.h"

typedef enum
{
    kCol,
    kRow
} PuzzleDetectDirection;

/**
 *1.创建小鸟
 *2.检测创建的小鸟是否有三个连续挨着的
 *3.如果存在就替换连续的小鸟的第三个为其他的
 *4.检测替换后的小鸟中是不是有可以在移动后消除的
 *5.如果没有执行步骤1 知道第5步成立
 *6.小鸟交换后 检测是不是有可以消除的
 *7.如果有的话 就消除小鸟 然后生成新的小鸟 填充空位
 *8.填充后执行步骤4 如此循环执行 直到当前任务完成
 */
class PuzzleUtil:public CCObject
{
private:
	/* 更新位置的队列 */
	int updateQueue;
    const float changePosTime;
    const float scaleTime;
    const float downSpeed;//更新小鸟时候的位置
    static PuzzleUtil *_instance;
    void __moveEnd(CCNode *pSender);
    void __resetBird(CCNode *pSender);
    void __moveDown(CCNode *pSenderj);
    void __updatePosComplete();
	void __effectMoveEnd(CCNode *node);
	void __blackHoleEff(CCNode *node);
	void __bombEff(CCNode *node);
	void __effectEnd(float delayTime);
	void __effectEndHandler();
	void __birdBurn(CCNode *node);
	void __removeBurnBird(CCNode *node);
	void __burnEff(CCNode *node);
	void __setForbiddenDisable();
	void __clearSelectBirds();
    CCPoint getWorldPos(const CCPoint &pos);
	ShareManager *sm;
	CCSpriteFrameCache *frameCache;
public:
    PuzzleUtil():updateQueue(0),changePosTime(0.25f),scaleTime(0.2f),downSpeed(1500.0f) {
		sm = ShareManager::shareManager();
		frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	};
    static PuzzleUtil *instance();
    /* 生成没有三个连续的小鸟的二维数组 */
    void createBirds();
    /* 检测是否存在可以消除的小组 */
    bool detectPuzzleBirds(short outer,short inner,PuzzleDetectDirection kDirection);
    /* 交换两个小鸟的位置 */
    void changeBirdPosition(bool withCallback=false);
    void changeBirdPosition(Bird *fstBird,Bird *sedBird);
    void checkPuzzle();
    /* 创建新的小鸟到指定的位置 */
    void createNewBird(float delay,short row,short col);

    /* 小鸟消除后更新小鸟的位置 */
    void updateBirdPosition();
    /* 移动位置后是否可以消除 */
    bool isCanPuzzle();
    /* 获取可以消除的小鸟集合 */
    std::vector<CCArray *> getDashBirds();
    /* 如果消除的个数为3个以上的话 */
    void dashBird(Bird *bird,const char *fstFrameName,const char *animationName,int type);
	/* 执行小鸟特效 如果存在特效的话 */
	void runEffect(Bird *bird);

	void blackHole(Bird *bird);
	void fireBird(Bird *bird);
	void bombBird(Bird *bird);
	void lightning(Bird *bird);
};

#endif // !_PUZZLEUTIL_H_
