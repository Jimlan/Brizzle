﻿#include "PuzzleUtil.h"
#include "managers/ShareManager.h"
#include "managers/SoundManager.h"


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
            short birdType = rand()%sm->birdTypes;
            Bird *bird = Bird::create(birdType);
            bird->row = i;
            bird->col = j;
            sm->birds[i][j] = bird;
        }
    }
    detectPuzzleBirds(ShareManager::row,ShareManager::col,kRow);
    detectPuzzleBirds(ShareManager::col,ShareManager::row,kCol);
}

bool PuzzleUtil::detectPuzzleBirds(short outer,short inner,PuzzleDetectDirection kDirection)
{
    /* 查看是否存在三个以上的 */
    //step 1  left->right by row
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
                while((type = rand()%sm->birdTypes)==prevBird->birdType);
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
    /* 更新小鸟的行列属性 */
    fst->row = sed->row;
    fst->col = sed->col;
    sed->row = fstRow;
    sed->col = fstCol;
    CCPoint fstPos = CCPointMake(fst->col*ShareManager::boxWidth,fst->row*ShareManager::boxHeight);//fst->getPosition();
    CCPoint secPos = CCPointMake(sed->col*ShareManager::boxWidth,sed->row*ShareManager::boxHeight);//sed->getPosition();
    CCMoveTo *sedMoveAct = CCMoveTo::create(changePosTime,secPos);
    CCMoveTo *fstMoveAct = CCMoveTo::create(changePosTime,fstPos);
    CCCallFunc *moveCall = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::__setForbiddenDisable));
    fst->runAction(fstMoveAct);
    sed->runAction(sedMoveAct);
    sm->effectLayer->runAction(CCSequence::create(CCDelayTime::create(changePosTime),moveCall,NULL));
    fst->isMoving = true;
    sed->isMoving = true;
    //移动小鸟的特效动画
    if(fst->effectSprite!=NULL)
    {
        CCMoveTo *fstMoveAct = CCMoveTo::create(changePosTime,sm->birdBatchNode->convertToWorldSpace(fstPos));
        fst->effectSprite->runAction(fstMoveAct);
    }
    if(sed->effectSprite!=NULL)
    {
        CCMoveTo *sedMoveAct = CCMoveTo::create(changePosTime,sm->birdBatchNode->convertToWorldSpace(secPos));
        sed->effectSprite->runAction(sedMoveAct);
    }
}

void PuzzleUtil::changeBirdPosition( Bird *fstBird,Bird *sedBird )
{
    sm->effectLayer->setSwallow(true);
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
    CCPoint fstPos = CCPointMake(fstBird->col*ShareManager::boxWidth,fstBird->row*ShareManager::boxHeight);//fst->getPosition();
    CCPoint secPos = CCPointMake(sedBird->col*ShareManager::boxWidth,sedBird->row*ShareManager::boxHeight);//sed->getPosition();
    CCMoveTo *sedMoveAct = CCMoveTo::create(changePosTime,secPos);
    CCMoveTo *fstMoveAct = CCMoveTo::create(changePosTime,fstPos);
    CCCallFuncN *moveFunc = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__resetBird));
    fstBird->runAction(fstMoveAct);
    sedBird->runAction(sedMoveAct);
	sm->effectLayer->runAction(CCSequence::create(CCDelayTime::create(changePosTime),moveFunc,NULL));
    if(fstBird->effectSprite!=NULL)
    {
        CCMoveTo *fstMoveAct = CCMoveTo::create(changePosTime,getWorldPos(fstPos));
        CCCallFuncN *moveFunc = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__effectMoveEnd));
        fstBird->effectSprite->runAction(CCSequence::create(fstMoveAct,moveFunc,NULL));
    }
    if(sedBird->effectSprite!=NULL)
    {
        CCMoveTo *sedMoveAct = CCMoveTo::create(changePosTime,getWorldPos(secPos));
        CCCallFuncN *moveFunc = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__effectMoveEnd));
        sedBird->effectSprite->runAction(CCSequence::create(sedMoveAct,moveFunc,NULL));
    }
}

/**
 *小鸟交换位置后的回调函数
 *在此方法中 判断是不是有可以消除的小鸟
 *如果没有的话 就将小鸟恢复原位
 */
void PuzzleUtil::__moveEnd( CCNode *pSender )
{
    CCLog("move end");
    bool res = isCanPuzzle();
	if(sm->fstBird)
	{
		sm->fstBird->isMoving = false;
	}
	if(sm->sedBird)
	{
		sm->sedBird->isMoving = false;
	}
	
    if(res==false)
    {
        sm->effectLayer->setSwallow(true);
        //将小鸟恢复原位
        changeBirdPosition(sm->fstBird,sm->sedBird);
    }
}

bool PuzzleUtil::isCanPuzzle()
{
    CCNode *birdParentNode = sm->birdBatchNode;
    CCArray *dashBirds = getDashBirds();
    int birdCount = dashBirds->count();
    CCLog("dash birds count:%d",birdCount);
    if(birdCount>0)
    {
		CCObject *obj = NULL;
        Bird *effectBird = NULL;
        bool updateFlag = true;
		bool hasEffect = false;
		CCARRAY_FOREACH(dashBirds,obj){
			Bird *bird = (Bird*)obj;
			if(bird->effectSprite)
			{
				hasEffect = true;
				break;
			}
		}
        if(birdCount>3&&hasEffect==false)
        {
			 
            if(dashBirds->containsObject(sm->fstBird))
            {
                effectBird = sm->fstBird;
            }
            else if(dashBirds->containsObject(sm->sedBird))
            {
                effectBird = sm->sedBird;
            }
            else
            {
                effectBird = (Bird*)dashBirds->objectAtIndex(ceil(birdCount/2));
            }
            effectBird->isChecked = false;
            int effectType = rand()%3+4;
            if(effectBird->effectSprite==NULL)
            {
				dashBirds->removeObject(effectBird,false);
                switch(effectType)
                {
                case 4:
                    dashBird(effectBird,"itemBomb_000.png","bomb",effectType);
                    break;
                case 5:
                    dashBird(effectBird,"itemFirebird_000.png","FireBird",effectType);
                    break;
                case 6:
                    dashBird(effectBird,"itemBlackhole_000.png","BlackHole",effectType);
                    break;
                case 7:
                    //dashBird(effectBird,"itemLightning_000.png","lightning",effectType);
                default:
                    break;
                }
				float moveTime = 1.3f;
				sm->effectLayer->setSwallow(true);
				CCARRAY_FOREACH(dashBirds,obj)
				{
					Bird *bird = (Bird*)obj;
					CCMoveTo *moveAct = CCMoveTo::create(moveTime,effectBird->getPosition());
					CCCallFunc *moveEnd = CCCallFunc::create(bird,callfunc_selector(Bird::removeFromParent));
					bird->runAction(CCSequence::create(moveAct,moveEnd,NULL));
					//移除之后将数组位置置空
					sm->birds[bird->row][bird->col] = NULL;
				}
				CCDelayTime *delay = CCDelayTime::create(moveTime);
				CCCallFunc *delayCall = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::__clearSelectBirds));
				sm->effectLayer->runAction(CCSequence::create(delay,delayCall,NULL));
            }

        }else{
			SoundManager::shareSoundManager()->playEffect("sounds/SFX/Bird_remove.mp3");
			//消除数组内的小鸟 在消除的时候要判断小鸟的特效类型

			CCARRAY_FOREACH(dashBirds,obj)
			{
				Bird *bird = (Bird*)obj;
				CCScaleTo *scaleAct = CCScaleTo::create(scaleTime,0);
				bird->stopAllActions();
				if(bird->effectSprite)
				{
					updateFlag = false;
					runEffect(bird);
				}
				//移除之后将数组位置置空
				sm->birds[bird->row][bird->col] = NULL;
				CCCallFunc *scaleFunc = CCCallFunc::create(bird,callfunc_selector(Bird::removeFromParent));
				CCSequence *seqAct = CCSequence::create(scaleAct,scaleFunc,NULL);
				bird->runAction(seqAct);
			}
			if(updateFlag)
			{
				CCDelayTime *updatePosDelay = CCDelayTime::create(scaleTime);
				CCCallFunc *updatePosFunc = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::updateBirdPosition));
				birdParentNode->runAction(CCSequence::create(updatePosDelay,updatePosFunc,NULL));
			}

		}
       
        return true;
    }
    return false;
}


CCArray * PuzzleUtil::getDashBirds()
{

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
                if(currentBird->isChecked==false)
                {
                    prevBird = currentBird;
                    appearCount++;
                    rowDashBird->addObject(currentBird);
                }
            }
            else if(prevBird->birdType==currentBird->birdType&&currentBird->isChecked==false)
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
                if(currentBird->isChecked==false)
                {
                    prevBird = currentBird;
                    appearCount++;
                    if(!birdDash->containsObject(currentBird))
                        colDashBird->addObject(currentBird);
                }
            }
            else if(prevBird->birdType==currentBird->birdType&&currentBird->isChecked==false)
            {
                if(!birdDash->containsObject(currentBird))
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
                if(!birdDash->containsObject(currentBird))
                    colDashBird->addObject(currentBird);
                prevBird = currentBird;
                appearCount=1;
            }
        }
    }
    CCObject *ele = NULL;
    return birdDash;
}

void PuzzleUtil::__resetBird( CCNode *pSender )
{
    //sm->effectLayer->setSwallow(false);
	sm->fstBird->isMoving = false;
	sm->sedBird->isMoving = false;
	sm->fstBird = NULL;
	sm->sedBird = NULL;

}
/**
 * 小鸟消除后 按照列进行遍历 计算每一个小鸟下面有几个空格
 */
void PuzzleUtil::updateBirdPosition()
{
    ShareManager *sm =ShareManager::shareManager();
    for(int j=0; j<ShareManager::col; j++)
    {
        int emptyCells = 0;
        for(int i=0; i<ShareManager::row; i++)
        {
            Bird *bird = sm->birds[i][j];
            if(bird==NULL)
            {
                emptyCells++;
            }
            else if(emptyCells!=0)
            {
                //将原来的位置置空
                bird->setTouchEnabled(false);
                sm->birds[bird->row][bird->col] = NULL;
                bird->row -= emptyCells;
                sm->birds[bird->row][bird->col] = bird;
                float downTime = emptyCells*ShareManager::boxHeight/downSpeed;
                CCPoint targetPos = CCPointMake(bird->col*ShareManager::boxWidth,bird->row*ShareManager::boxHeight);
                CCMoveTo *moveToAct = CCMoveTo::create(0.2f,targetPos);
                CCCallFuncN *moveFunc = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__moveDown));
                CCSequence *moveSeq = CCSequence::create(moveToAct,moveFunc,NULL);
                bird->runAction(moveSeq);
                if(bird->effectSprite!=NULL)
                {
                    bird->effectSprite->runAction(CCMoveTo::create(0.2f,sm->birdBatchNode->convertToWorldSpace(targetPos)));
                }
            }
        }
        //小鸟的位置移动之后创建新的小鸟
		float unitTime = ShareManager::boxHeight/downSpeed;
		int idx = 0;
        while(emptyCells)
        {
            int row = ShareManager::row-emptyCells;
            createNewBird(unitTime*idx,row,j);
			idx++;
            emptyCells--;
        }

    }
    //更新了位置之后 要继续检测是否有可以消除的小鸟
    CCDelayTime *checkDelay = CCDelayTime::create(0.5f);
    CCCallFunc *checkCall = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::__updatePosComplete));
    sm->birdBatchNode->runAction(CCSequence::create(checkDelay,checkCall,NULL));
}

void PuzzleUtil::__moveDown( CCNode *pSender )
{
    Bird *bird = (Bird*)pSender;
    bird->setTouchEnabled(true);
    //bird->shakeBody(1.1f,0.8f);
}

void PuzzleUtil::createNewBird(float delay, short row,short col )
{
    CCLog("row:%d,col:%d",row,col);
    short type = rand()%sm->birdTypes;
    Bird *bird = Bird::create(type);
    bird->setPosition(ccp(col*ShareManager::boxWidth,VisibleRect::top().y));
    ShareManager::shareManager()->birdBatchNode->addChild(bird);
    bird->row = row;
    bird->col = col;
    bird->setScale(0.9f);
    ShareManager::shareManager()->birds[row][col] = bird;
    float targetY = row*ShareManager::boxHeight;
    float moveTime = (bird->getPositionY() - row*ShareManager::boxHeight)/downSpeed;
    CCMoveTo *moveAct = CCMoveTo::create(moveTime,ccp(bird->getPositionX(),targetY));
    CCCallFuncN *moveCall = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__moveDown));
    bird->setTouchEnabled(false);
    bird->runAction(CCSequence::create(CCDelayTime::create(delay),moveAct,moveCall,NULL));
}
/* 监测是否有可以消除的小鸟 */
void PuzzleUtil::checkPuzzle()
{

}
/* 位置更新完毕之后需要重新监测 是否有可以消除的小鸟 */
void PuzzleUtil::__updatePosComplete()
{
    bool res = isCanPuzzle();
	if(res==false)
	{
		//sm->effectLayer->setSwallow(false);
		sm->fstBird = NULL;
		sm->sedBird = NULL;
	}
    
}

void PuzzleUtil::dashBird(Bird *bird,const char *fstFrameName,const char *animationName,int type)
{

    CCSprite *sprite = SPRITE(fstFrameName);
    sm->effectLayer->addChild(sprite);
    sprite->setPosition(getWorldPos(ccp(bird->col*ShareManager::boxWidth,bird->row*ShareManager::boxHeight)));
    sprite->runAction(CCRepeatForever::create(GET_ANIMATE(animationName)));
    bird->effectSprite = sprite;
    sprite->setScale(0.9f);
    bird->effectType = type;
}

//通过小鸟的坐标来获取到全局坐标
CCPoint PuzzleUtil::getWorldPos(const CCPoint &pos )
{
    CCPoint globalPos = ShareManager::shareManager()->birdBatchNode->convertToWorldSpace(pos);
    return globalPos;
}

void PuzzleUtil::__effectMoveEnd( CCNode *node )
{
    CCLog("move end effect x:%f,y:%f",node->getPositionX(),node->getPositionY());
}

void PuzzleUtil::runEffect( Bird *bird )
{
    if(bird->effectSprite)
    {
        sm->effectLayer->stopAllActions();
        sm->effectLayer->setSwallow(true);
        bird->effectSprite->removeFromParentAndCleanup(true);
        bird->effectSprite = NULL;
        switch(bird->effectType)
        {
        case 4:
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_starbomb.mp3");
            bombBird(bird);
            break;
        case 5:
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_firebomb.mp3");
            fireBird(bird);
            break;
        case 6:
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_blackhole.mp3");
            blackHole(bird);
            break;
        case 7:
            //  sm->effectLayer->setSwallow(false);
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_lighting.mp3");
            //   lightning(bird);
            break;
        default:
            break;
        }
    }
}

void PuzzleUtil::fireBird( Bird *bird )
{
    CCSprite *effect = SPRITE("firebird_000.png");
    effect->runAction(CCRepeatForever::create(GET_ANIMATE("FireBirdEff")));
    sm->effectLayer->addChild(effect);
    effect->setPosition(getWorldPos(bird->getPosition()));
    const float speed = 400.0f;
    float time = (effect->getPositionY()+100)/speed;
    CCMoveTo *moveAct = CCMoveTo::create(time,ccp(effect->getPositionX(),-100));
    CCCallFunc *moveCall = CCCallFunc::create(this,callfunc_selector(CCSprite::removeFromParent));
    effect->runAction(CCSequence::create(moveAct,NULL));
    bird->stopAllActions();
    __birdBurn(bird);
    int row = bird->row,col = bird->col;
    for(int i=0; i<row; i++)
    {
        Bird *currentBird = sm->birds[i][col];
        if(currentBird&&currentBird->isChecked==false)
        {
            currentBird->isChecked = true;
            float dt = (row-i)*ShareManager::boxHeight/speed;
            CCDelayTime *delay = CCDelayTime::create(dt);
            CCCallFuncN *delayCall = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__birdBurn));
            currentBird->runAction(CCSequence::create(delay,delayCall,NULL));
        }
    }
    __effectEnd(time+0.1f);
}

void PuzzleUtil::blackHole( Bird *bird )
{
    const float effTime = 1.5f;
    CCSprite *effect = SPRITE("Blackhole_000.png");
    CCPoint birdPos = bird->getPosition();
    effect->runAction(CCRepeatForever::create(GET_ANIMATE("BlackHoleEff")));
    sm->effectLayer->addChild(effect);
    effect->setPosition(getWorldPos(birdPos));
    CCScaleTo *scaleAct = CCScaleTo::create(1.5f,0);
    //CCCallFunc *scaleCall = CCCallFunc::create(this,callfunc_selector(CCSprite::removeFromParent));
    effect->runAction(CCSequence::create(scaleAct,NULL));
    for(int i=0; i<ShareManager::row; i++)
    {
        for(int j=0; j<ShareManager::col; j++)
        {
            Bird *currentBird = sm->birds[i][j];
            if(currentBird&&currentBird->isChecked==false&&currentBird->birdType==bird->birdType)
            {
                currentBird->isChecked=true;
                if(currentBird->effectSprite)
                {
                    currentBird->effectSprite->removeFromParentAndCleanup(true);
                    currentBird->effectSprite = NULL;
                }
                currentBird->getParent()->reorderChild(currentBird,100);
                float angle = rand()%360+200;
                CCRotateBy *rotateAct = CCRotateBy::create(effTime,angle);
                CCScaleTo *scaleAct = CCScaleTo::create(effTime,0.3f);
                CCMoveTo *moveAct = CCMoveTo::create(effTime,birdPos);
                CCSpawn *spawnAct = CCSpawn::create(rotateAct,moveAct,scaleAct,NULL);
                CCCallFuncN *actHandler = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__blackHoleEff));
                currentBird->runAction(CCSequence::create(spawnAct,actHandler,NULL));
            }
        }
    }
    __effectEnd(1.6f);
}

void PuzzleUtil::bombBird( Bird *bird )
{
    CCSprite *effect = SPRITE("bomb_001.png");
    CCCallFunc *callfunc = CCCallFunc::create(effect,callfunc_selector(CCSprite::removeFromParent));
    effect->runAction(CCSequence::create(GET_ANIMATE("BombEff"),callfunc,NULL));
    sm->effectLayer->addChild(effect);
    effect->setPosition(getWorldPos(bird->getPosition()));
    int row = bird->row,col = bird->col;
    int top = (row+1)==ShareManager::row?row:row+1,
        left=(col-1)<0?col:col-1,
        right=(col+1)==ShareManager::col?col:col+1
              ,bottom=(row-1)<0?0:row-1;
    for(int i=bottom; i<=top; i++)
    {
        for(int j=left; j<=right; j++)
        {
            Bird *currentBird = sm->birds[i][j];
            if(currentBird&&currentBird->isChecked==false)
            {
                currentBird->isChecked = true;
                currentBird->runAction(CCScaleTo::create(0.5f,0));
                if(currentBird->effectSprite)
                {
                    currentBird->effectSprite->removeFromParentAndCleanup(true);
                    currentBird->effectSprite = NULL;
                }
            }
            sm->birds[i][j] = NULL;
        }
    }
    __effectEnd(0.5f);
}

void PuzzleUtil::lightning( Bird *bird )
{
    CCSprite *effect = SPRITE("firebird_000.png");
    effect->runAction(CCRepeatForever::create(GET_ANIMATE("FireBirdEff")));
    sm->effectLayer->addChild(effect);
    effect->setPosition(getWorldPos(bird->getPosition()));
    CCSprite *colLight = CCSprite::create("images/item_effect/lightning_A_RETINA.png");
    CCSprite *rowLight = CCSprite::create("images/item_effect/lightning_B_RETINA.png");
    rowLight->setPosition(ccp(VisibleRect::center().x,bird->getPositionY()));
    colLight->setAnchorPoint(ccp(0.5,0));
    colLight->setPosition(ccp(getWorldPos(bird->getPosition()).x,ShareManager::boxHeight/2));
    sm->effectLayer->addChild(rowLight);
    sm->effectLayer->addChild(colLight);
    CCScaleBy *colScaleBy = CCScaleBy::create(0.5f,0.2f,1.0f);
    CCScaleBy *rowScaleBy = CCScaleBy::create(0.5f,1.0f,0.2f);
    CCScaleTo *colScaleTo = CCScaleTo::create(0.5f,0,1.0f);
    CCScaleTo *rowScaleTo = CCScaleTo::create(0.5f,1.0f,0);
    colLight->runAction(CCSequence::create(colScaleBy,colScaleBy->reverse(),colScaleTo,NULL));
    rowLight->runAction(CCSequence::create(rowScaleBy,rowScaleBy->reverse(),rowScaleTo,NULL));
    // sm->effectLayer->setSwallow(false);
}

void PuzzleUtil::__blackHoleEff( CCNode *node )
{
    Bird *bird = (Bird*)node;

    bird->removeFromParentAndCleanup(true);
    sm->birds[bird->row][bird->col] = NULL;
    bird = NULL;
}

void PuzzleUtil::__effectEnd(float delayTime)
{
    CCDelayTime *delay = CCDelayTime::create(delayTime);
    CCCallFunc *delayCall = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::__effectEndHandler));
    sm->effectLayer->runAction(CCSequence::create(delay,delayCall,NULL));
}

void PuzzleUtil::__effectEndHandler()
{
    updateBirdPosition();
}

void PuzzleUtil::__birdBurn( CCNode *node )
{
    Bird *bird = (Bird*)node;
    bird->stopAllActions();
    CCString *burnName = CCString::createWithFormat("box0%d_burn@2x.png",bird->birdType);
    bird->setDisplayFrame(frameCache->spriteFrameByName(burnName->getCString()));
    CCDelayTime *delay = CCDelayTime::create(0.2f);
    CCCallFuncN *delayCall = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__removeBurnBird));
    bird->runAction(CCSequence::create(delay,delayCall,NULL));
}

void PuzzleUtil::__removeBurnBird( CCNode *node )
{
    Bird *bird = (Bird*)node;
    bird->setDisplayFrame(frameCache->spriteFrameByName("bone@2x.png"));
    bird->runAction(CCSequence::create(CCDelayTime::create(0.1f),CCFadeOut::create(0.1f),CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__burnEff)),NULL));
}

void PuzzleUtil::__burnEff( CCNode *node )
{
    Bird *bird = (Bird*)node;
    if(bird->effectSprite)
    {
        bird->effectSprite->removeFromParent();
        bird->effectSprite = NULL;
    }
    sm->birds[bird->row][bird->col] = NULL;
    node->removeFromParent();
}

void PuzzleUtil::__setForbiddenDisable()
{
	__moveEnd(NULL);
}

void PuzzleUtil::__clearSelectBirds()
{
	sm->fstBird = NULL;
	sm->sedBird = NULL;
	updateBirdPosition();
}
