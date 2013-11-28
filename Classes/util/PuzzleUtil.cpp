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
                if(kDirection==kRow)
                {
                    CCLog("row:%d,col:%d,type:%d",i,j,type);
					sm->birds[i][j] = NULL;
                    sm->birds[i][j] = Bird::create(type);
                }
                else
                {
                    CCLog("row:%d,col:%d,type:%d",j,i,type);
					sm->birds[j][i] = NULL;
                    sm->birds[j][i] = Bird::create(type);
                }
            }
        }
    }

    //step 2  bottom->up by col
    /*for(int i=0;i<ShareManager::col;i++)
    {
    	for(int j=0;j<ShareManager::row;j++)
    	{

    	}
    }*/
    return true;
}

void PuzzleUtil::changeBirdPosition()
{

}
