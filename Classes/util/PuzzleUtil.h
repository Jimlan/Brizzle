#ifndef _PUZZLEUTIL_H_
#define _PUZZLEUTIL_H_

#include "components/Bird.h"

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
	const float changePosTime = 0.25f;
    static PuzzleUtil *_instance;
    void __moveEnd(CCNode *pSender);
	void __resetBird(CCNode *pSender);
public:
    static PuzzleUtil *instance();
    /* 生成没有三个连续的小鸟的二维数组 */
    void createBirds();
    /* 检测是否存在可以消除的小组 */
    bool detectPuzzleBirds(short outer,short inner,PuzzleDetectDirection kDirection);
    /* 交换两个小鸟的位置 */
    void changeBirdPosition(bool withCallback=false);
	void changeBirdPosition(Bird *fstBird,Bird *sedBird);
	/* 小鸟消除后更新小鸟的位置 */
	void updateBirdPosition();
    /* 移动位置后是否可以消除 */
    bool isCanPuzzle();
	/* 获取可以消除的小鸟集合 */
	CCArray *getDashBirds();
    /* 如果消除的个数为4个 */
    void dash4Bird();
    /* 如果消除的个数为5个 */
    void dash5Bird();
    /* 如果消除的个数为6个 */
    void dash6Bird();
    /* 如果消除的个数为7个或者7个以上 */
    void dash7Bird();
};

#endif // !_PUZZLEUTIL_H_
