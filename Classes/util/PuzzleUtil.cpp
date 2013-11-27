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
}

bool PuzzleUtil::detectPuzzleBirds()
{
	/* 查看是否存在三个以上的 */
	//step 1  left->right by row
	ShareManager *sm = ShareManager::shareManager();
	for(int i=0;i<ShareManager::row;i++)
	{
		//记录出现的次数 和上一次出现的的小鸟
		int appearCount = 0;
		Bird *prevBird = NULL;
		for(int j=0;j<ShareManager::col;j++)
		{
			Bird *bird = sm->birds[i][j];
			if(prevBird==NULL)
			{
				prevBird = bird;
				appearCount++;
			}else if(prevBird->birdType==bird->birdType){
				appearCount++;
			}
		}
	}
	return true;
}
