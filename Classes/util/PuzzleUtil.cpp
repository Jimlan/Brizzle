#include "PuzzleUtil.h"
#include "managers/ShareManager.h"


PuzzleUtil *PuzzleUtil::_instance = NULL;

PuzzleUtil * PuzzleUtil::instance()
{
    if(!_instance)
    {
        _instance = new PuzzleUtil();
    }

    return _instance;
}

void PuzzleUtil::createBirds()
{
    short row = ShareManager::row;
    short col = ShareManager::col;
    for(short i=0; i<row; i++)
    {
        for(short j=0; j<col; j++)
        {
            short birdType = rand()%8;
            Bird *bird = Bird::create(birdType);
            bird->row = i;
            bird->col = j;
            ShareManager::shareManager()->birds[i][j] = bird;
        }
    }
    detectPuzzleBirds(ShareManager::row,ShareManager::col,kRow);
    detectPuzzleBirds(ShareManager::col,ShareManager::row,kCol);
}

bool PuzzleUtil::detectPuzzleBirds(short outer,short inner,PuzzleDetectDirection kDirection)
{
    /* 查看是否存在三个以上的 */
    //step 1  left->right by row
    ShareManager *sm = ShareManager::shareManager();
    for(int i=0; i<outer; i++)
    {
        //记录出现的次数 和上一次出现的的小鸟
        int appearCount = 0;
        Bird *prevBird = NULL;
        for(int j=0; j<inner; j++)
        {
            Bird *bird;
            if(kDirection==kRow)
            {
                bird = sm->birds[i][j];
            }
            else
            {
                bird = sm->birds[j][i];
            }
            if(prevBird==NULL)
            {
                prevBird = bird;
                appearCount++;
            }
            else if(prevBird->birdType==bird->birdType)
            {
                appearCount++;
            }
            else
            {
                prevBird = bird;
                appearCount = 1;
            }
            //如果出现的次数等 于3的时候要更改为其他的小鸟类型
            if(appearCount==3)
            {
                short type;
                while((type = rand()%8)==prevBird->birdType);
                Bird *newBird = Bird::create(type);
                if(kDirection==kRow)
                {
                    CCLog("row:%d,col:%d,type:%d",i,j,type);
                    sm->birds[i][j] = NULL;
                    sm->birds[i][j] = newBird;
                    newBird->row = i;
                    newBird->col = j;
                }
                else
                {
                    CCLog("row:%d,col:%d,type:%d",j,i,type);
                    sm->birds[j][i] = NULL;
                    sm->birds[j][i] = newBird;
                    newBird->row = j;
                    newBird->col = i;
                }
            }
        }
    }

    return true;
}

void PuzzleUtil::changeBirdPosition()
{
    ShareManager *sm = ShareManager::shareManager();
    Bird *fst = sm->fstBird;
    Bird *sed = sm->sedBird;
    /* 交换在二维数组中的位置 */
    sm->birds[fst->row][fst->col] = sed;
    sm->birds[sed->row][sed->col] = fst;
    /* 临时保存行列坐标 */
    int fstRow = fst->row;
    int fstCol = fst->col;
    CCPoint fstPos = fst->getPosition();
    CCPoint secPos = sed->getPosition();
    CCMoveTo *sedMoveAct = CCMoveTo::create(0.2f,fstPos);
    CCMoveTo *fstMoveAct = CCMoveTo::create(0.2f,secPos);
    CCCallFuncN *moveCall = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__moveEnd));
    fst->runAction(CCSequence::create(fstMoveAct,moveCall,NULL));
    sed->runAction(CCSequence::create(sedMoveAct,moveCall,NULL));
    fst->isMoving = true;
    sed->isMoving = true;
    /* 更新小鸟的行列属性 */
    fst->row = sed->row;
    fst->col = sed->col;
    sed->row = fstRow;
    sed->col = fstCol;
}

void PuzzleUtil::__moveEnd( CCNode *pSender )
{
    CCLog("move end");
    Bird *bird = (Bird*)pSender;
    bird->isMoving = false;
	isCanPuzzle();
    ShareManager::shareManager()->fstBird = NULL;
    ShareManager::shareManager()->sedBird = NULL;
}

bool PuzzleUtil::isCanPuzzle()
{
    ShareManager *sm = ShareManager::shareManager();
    Bird *first = sm->fstBird;
    Bird *second = sm->sedBird;
    int fbRow = first->row;
    int fbCol = first->col;
    int sdRow = second->row;
    int sdCol = second->col;
    bool sameRowNeighbor = fbRow==sdRow?abs(fbCol-sdCol)==1:false;
    bool sameColNeighbor = fbCol==sdCol?abs(fbRow-sdRow)==1:false;
    if(sameRowNeighbor)//如果在同一行就检测两个移动的小鸟所在的列
    {

    }
    else if(sameColNeighbor)//如果在同一列就检测两个移动的小鸟所在的行
    {
		CCArray *birds1 = getRowDashBirds(first);
		CCArray *birds2 = getRowDashBirds(second);
		CCLog("count1:%d",birds1->count());
		CCLog("count2:%d",birds2->count());
    }
    return true;
}

CCArray * PuzzleUtil::getRowDashBirds(Bird *bird )
{
    //左右方向进行检测
    short col = bird->col;
    short row = bird->row;
    short birdType = bird->birdType;
    ShareManager *sm = ShareManager::shareManager();
    CCArray *birds = CCArray::create();
	birds->retain();
    birds->addObject(bird);
    bird->isChecked = true;
    short leftCol = col-1;
    short rightCol = col+1;
    if(leftCol>=0)
    {
        Bird *leftBird = sm->birds[row][leftCol];
        short leftType = leftBird->birdType;
        if(birdType==leftType)
        {
            leftBird->isChecked = true;
            birds->addObjectsFromArray(getRowDashBirds(leftBird));
        }
    }
    if(rightCol<ShareManager::col)
    {
        Bird *rightBird = sm->birds[row][rightCol];
        short rightType = rightBird->birdType;
        if(birdType==rightType)
        {
            rightBird->isChecked = true;
            birds->addObjectsFromArray(getRowDashBirds(rightBird));
        }
    }
    return birds;
}

CCArray * PuzzleUtil::getColDashBirds(Bird *bird )
{
    //上下方向进行检测
    short col = bird->col;
    short row = bird->row;
	return NULL;
}
