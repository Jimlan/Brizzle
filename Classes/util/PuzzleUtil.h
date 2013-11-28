#ifndef _PUZZLEUTIL_H_
#define _PUZZLEUTIL_H_

#include "components/Bird.h"

typedef enum
{
    kCol,
    kRow
} PuzzleDetectDirection;


class PuzzleUtil:public CCObject
{
private:
    static PuzzleUtil *_instance;
    void __moveEnd(CCNode *pSender);
public:
    static PuzzleUtil *instance();
    /* 生成没有三个连续的小鸟的二维数组 */
    void createBirds();
    /* 检测是否存在可以消除的小组 */
    bool detectPuzzleBirds(short outer,short inner,PuzzleDetectDirection kDirection);
    /* 交换两个小鸟的位置 */
    void changeBirdPosition();
    /* 移动位置后是否可以消除 */
    CCArray *getRowDashBirds(Bird *bird);
    CCArray *getColDashBirds(Bird *bird);
    bool isCanPuzzle();
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
