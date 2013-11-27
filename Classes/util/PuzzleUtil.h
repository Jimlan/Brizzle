#ifndef _PUZZLEUTIL_H_
#define _PUZZLEUTIL_H_

#include "components/Bird.h"

class PuzzleUtil:public CCObject
{
private:
    static PuzzleUtil *_instance;
public:
    static PuzzleUtil *instance();
    /* 生成没有三个连续的小鸟的二维数组 */
    void createBirds();
    /* 检测是否存在可以消除的小组 */
    bool detectPuzzleBirds();
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
