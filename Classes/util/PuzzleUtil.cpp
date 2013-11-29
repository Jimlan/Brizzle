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

void PuzzleUtil::changeBirdPosition(bool withCallback)
{
    ShareManager *sm = ShareManager::shareManager();
    sm->fstBird->getParent()->reorderChild(sm->fstBird,0);
    sm->sedBird->getParent()->reorderChild(sm->sedBird,1);
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
    CCMoveTo *sedMoveAct = CCMoveTo::create(changePosTime,fstPos);
    CCMoveTo *fstMoveAct = CCMoveTo::create(changePosTime,secPos);
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

void PuzzleUtil::changeBirdPosition( Bird *fstBird,Bird *sedBird )
{
    ShareManager *sm = ShareManager::shareManager();
    sm->fstBird->getParent()->reorderChild(sm->fstBird,1);
    sm->sedBird->getParent()->reorderChild(sm->sedBird,0);
    //更改他们在数组中的位置
    sm->birds[fstBird->row][fstBird->col] = sedBird;
    sm->birds[sedBird->row][sedBird->col] = fstBird;
    int tempRow = fstBird->row,tempCol = fstBird->col;
    fstBird->row = sedBird->row;
    fstBird->col = sedBird->col;
    sedBird->row = tempRow;
    sedBird->col = tempCol;
    /* 标记正在移动 不能接收用户的操作 */
    sedBird->isMoving = fstBird->isMoving = true;
    CCPoint fstPos = fstBird->getPosition();
    CCPoint secPos = sedBird->getPosition();
    CCMoveTo *sedMoveAct = CCMoveTo::create(changePosTime,fstPos);
    CCMoveTo *fstMoveAct = CCMoveTo::create(changePosTime,secPos);
    CCCallFuncN *moveFunc = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__resetBird));
    fstBird->runAction(CCSequence::create(fstMoveAct,moveFunc,NULL));
    sedBird->runAction(CCSequence::create(sedMoveAct,moveFunc,NULL));
}

/**
 *小鸟交换位置后的回调函数
 *在此方法中 判断是不是有可以消除的小鸟
 *如果没有的话 就将小鸟恢复原位
 */
void PuzzleUtil::__moveEnd( CCNode *pSender )
{
    CCLog("move end");
    ShareManager *sm = ShareManager::shareManager();
    Bird *bird = (Bird*)pSender;
    bird->isMoving = false;

    if(sm->fstBird->isMoving==false&&sm->sedBird->isMoving==false)
    {
        //获取到小鸟所在的父节点
        CCNode *birdParentNode = bird->getParent();
        CCArray *dashBirds = getDashBirds();
        int birdCount = dashBirds->count();
        CCLog("dash birds count:%d",birdCount);
        if(birdCount>0)
        {
            //消除数组内的小鸟
            CCObject *obj = NULL;
            CCARRAY_FOREACH(dashBirds,obj)
            {
                Bird *bird = (Bird*)obj;
                CCScaleTo *scaleAct = CCScaleTo::create(scaleTime,0);
                bird->stopAllActions();
                //移除之后将数组位置置空
                sm->birds[bird->row][bird->col] = NULL;
                CCCallFunc *scaleFunc = CCCallFunc::create(bird,callfunc_selector(Bird::removeFromParent));
                CCSequence *seqAct = CCSequence::create(scaleAct,scaleFunc,NULL);
                bird->runAction(seqAct);

            }
            CCDelayTime *updatePosDelay = CCDelayTime::create(scaleTime);
            CCCallFunc *updatePosFunc = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::updateBirdPosition));
            birdParentNode->runAction(CCSequence::create(updatePosDelay,updatePosFunc,NULL));
            sm->fstBird = NULL;
            sm->sedBird = NULL;
        }
        else
        {
            //将小鸟恢复原位
            changeBirdPosition(sm->fstBird,sm->sedBird);
            sm->fstBird = NULL;
            sm->sedBird = NULL;
        }

    }
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

    }
    return true;
}


CCArray * PuzzleUtil::getDashBirds()
{
    ShareManager *sm = ShareManager::shareManager();
    CCArray *birdDash = CCArray::create();
    for(int i=0; i<ShareManager::row; i++)
    {
        CCArray *rowDashBird = CCArray::create();
        int appearCount = 0;
        Bird *prevBird = NULL;
        for(int j=0; j<ShareManager::col; j++)
        {
            Bird *currentBird = sm->birds[i][j];
            if(currentBird==NULL)
            {
				if(appearCount>=3)
				{
					birdDash->addObjectsFromArray(rowDashBird);
				}
                rowDashBird->removeAllObjects();
				appearCount = 0;
				prevBird = NULL;
                continue;
            }
            if(prevBird==NULL)
            {
                prevBird = currentBird;
                appearCount++;
                rowDashBird->addObject(currentBird);
            }
            else if(prevBird->birdType==currentBird->birdType)
            {
                rowDashBird->addObject(currentBird);
                appearCount++;
                if(j==ShareManager::col-1&&appearCount>=3)
                {
                    birdDash->addObjectsFromArray(rowDashBird);
                    rowDashBird->removeAllObjects();
                }
            }
            else
            {
                if(appearCount>=3)
                {
                    birdDash->addObjectsFromArray(rowDashBird);
                }
                rowDashBird->removeAllObjects();
                rowDashBird->addObject(currentBird);
                prevBird = currentBird;
                appearCount=1;
            }
        }
    }

    for(int j=0; j<ShareManager::col; j++)
    {
        CCArray *colDashBird = CCArray::create();
        int appearCount = 0;
        Bird *prevBird = NULL;
        for(int i=0; i<ShareManager::row; i++)
        {
            Bird *currentBird = sm->birds[i][j];
			if(currentBird==NULL)
			{
				if(appearCount>=3)
				{
					birdDash->addObjectsFromArray(colDashBird);
				}
				colDashBird->removeAllObjects();
				appearCount = 0;
				prevBird = NULL;
				continue;
			}
            if(prevBird==NULL)
            {
                prevBird = currentBird;
                appearCount++;
                colDashBird->addObject(currentBird);
            }
            else if(prevBird->birdType==currentBird->birdType)
            {
                colDashBird->addObject(currentBird);
                appearCount++;
                if(i==ShareManager::row-1&&appearCount>=3)
                {
                    birdDash->addObjectsFromArray(colDashBird);
                    colDashBird->removeAllObjects();
                }
            }
            else
            {
                if(appearCount>=3)
                {
                    birdDash->addObjectsFromArray(colDashBird);
                }
                colDashBird->removeAllObjects();
                colDashBird->addObject(currentBird);
                prevBird = currentBird;
                appearCount=1;
            }
        }
    }

    return birdDash;
}

void PuzzleUtil::__resetBird( CCNode *pSender )
{
    Bird *bird = (Bird*)pSender;
    bird->isMoving = false;
}
/**
 * 小鸟消除后 按照列进行遍历 计算每一个小鸟下面有几个空格
 */
void PuzzleUtil::updateBirdPosition()
{
	return;
	ShareManager *sm =ShareManager::shareManager();
	for(int j=0;j<ShareManager::col;j++)
	{
		int emptyCells = 0;
		for(int i=0;i<ShareManager::row;i++)
		{
			Bird *bird = sm->birds[i][j];
			if(bird==NULL)
			{
				emptyCells++;
			}else if(emptyCells!=0){
				bird->setTouchEnabled(false);
				bird->row -= emptyCells;
				sm->birds[bird->row][bird->col] = bird;
				float downTime = emptyCells*ShareManager::boxHeight/downSpeed;
				CCMoveTo *moveToAct = CCMoveTo::create(downTime,ccp(bird->col*ShareManager::boxWidth,bird->row*ShareManager::boxHeight));
				CCCallFuncN *moveFunc = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__moveDown));
				CCSequence *moveSeq = CCSequence::create(moveToAct,moveFunc,NULL);
				bird->runAction(moveSeq);
			}
		}
	}
}

void PuzzleUtil::__moveDown( CCNode *pSender )
{
	Bird *bird = (Bird*)pSender;
	bird->setTouchEnabled(true);
}
