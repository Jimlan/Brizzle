#include "PuzzleUtil.h"
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
                    //CCLog("row:%d,col:%d,type:%d",i,j,type);
                    sm->birds[i][j] = NULL;
                    sm->birds[i][j] = newBird;
                    newBird->row = i;
                    newBird->col = j;
                }
                else
                {
                    // CCLog("row:%d,col:%d,type:%d",j,i,type);
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
    sm->effectLayer->setSwallow(true);
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
    bool res = isCanPuzzle();
    if(res==false)
    {
        //将小鸟恢复原位
        changeBirdPosition(sm->fstBird,sm->sedBird);
    }
}

bool PuzzleUtil::isCanPuzzle()
{
    float actTime = 0.2f;
    float delayTime = 0.25f;
    bool updateFlag = true;
    CCNode *birdParentNode = sm->birdBatchNode;
    std::vector<CCArray*> dashBirds = getDashBirds();
    std::vector<CCArray*>::size_type birdArrSize = dashBirds.size();
    if(birdArrSize>0)
    {
        for(std::vector<CCArray*>::size_type outer = 0; outer<birdArrSize; outer++)
        {
            CCArray *birds= dashBirds[outer];
            CCObject *obj = NULL;
            CCARRAY_FOREACH(birds,obj)
            {
                Bird *bird = (Bird*)obj;
                for(std::vector<CCArray*>::size_type inner = outer+1; inner<birdArrSize; inner++)
                {
                    CCArray *nextBirds = dashBirds[inner];
                    if(nextBirds->containsObject(bird))
                    {
                        bird->isCorner = true;
                        nextBirds->removeObject(bird);
                        birds->addObjectsFromArray(nextBirds);
                        nextBirds->removeAllObjects();
                    }
                }
            }
        }

        std::vector<CCArray*>::iterator iter = dashBirds.begin();
        while(iter!=dashBirds.end())
        {
            int count = (*iter)->count();
            if(count>0)
            {
                CCObject *obj = NULL;
                Bird *focus = NULL;
                bool hasEffect = false;
                CCARRAY_FOREACH(*iter,obj)
                {
                    Bird *bird = (Bird*)obj;
                    if(bird->isCorner&&focus==NULL)
                    {
                        focus = bird;
                    }
                    if(bird->effectSprite!=NULL)
                    {
                        hasEffect = true;
                    }
                }
                if(count>3&&hasEffect==false)
                {

                    //找出交叉点
                    CCLog("found 3+");
                    if(focus==NULL)
                    {
                        if((*iter)->containsObject(sm->fstBird))
                        {
                            focus = sm->fstBird;
                        }
                        else if((*iter)->containsObject(sm->sedBird))
                        {
                            focus = sm->sedBird;
                        }
                        else
                        {
                            focus = (Bird*)(*iter)->objectAtIndex(ceil(float(count/2)));
                        }
                    }

                    focus->isChecked = false;
                    int effectType = count;// rand()%3+4;
                    if(focus->effectSprite==NULL)
                    {
                        (*iter)->removeObject(focus,false);
                        switch(effectType)
                        {
                        case 4:
                            dashBird(focus,"itemBomb_000.png","bomb",effectType);
                            break;
                        case 5:
                            dashBird(focus,"itemFirebird_000.png","FireBird",effectType);
                            break;
                        case 6:
                            dashBird(focus,"itemBlackhole_000.png","BlackHole",effectType);
                            break;
                        case 7:
                            //dashBird(effectBird,"itemLightning_000.png","lightning",effectType);
                        default:
                            dashBird(focus,"itemBlackhole_000.png","BlackHole",effectType);
                            break;
                        }
                    }
                    float moveTime = 0.2f;
                    CCARRAY_FOREACH(*iter,obj)
                    {
                        ShareManager::shareManager()->dashBirdsNum++;
                        Bird *bird = (Bird*)obj;
                        CCMoveTo *moveAct = CCMoveTo::create(actTime,focus->getPosition());
                        CCFadeOut *fadeOut = CCFadeOut::create(actTime);
                        CCCallFunc *moveEnd = CCCallFunc::create(bird,callfunc_selector(Bird::removeFromParent));
                        bird->runAction(CCSequence::create(moveAct,fadeOut,moveEnd,NULL));
                        //移除之后将数组位置置空
                        sm->birds[bird->row][bird->col] = NULL;
                    }
                    updateQueue++;
                    CCDelayTime *delay = CCDelayTime::create(delayTime);
                    CCCallFunc *delayCall = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::__clearSelectBirds));
                    sm->effectLayer->runAction(CCSequence::create(delay,delayCall,NULL));
                }
                else
                {
                    SoundManager::shareSoundManager()->playEffect("sounds/SFX/Bird_remove");
                    CCARRAY_FOREACH(*iter,obj)
                    {
                        Bird *bird = (Bird*)obj;
                        CCScaleTo *scaleAct = CCScaleTo::create(actTime,0);
                        bird->stopAllActions();
                        if(bird->effectSprite)
                        {
                            updateFlag = false;
                            runEffect(bird);
                        }
                        ShareManager::shareManager()->dashBirdsNum++;
                        //移除之后将数组位置置空
                        sm->birds[bird->row][bird->col] = NULL;
                        CCCallFunc *scaleFunc = CCCallFunc::create(bird,callfunc_selector(Bird::removeFromParent));
                        CCSequence *seqAct = CCSequence::create(scaleAct,scaleFunc,NULL);
                        bird->runAction(seqAct);
                    }
                    if(updateFlag)
                    {
                        updateQueue++;
                        CCDelayTime *updatePosDelay = CCDelayTime::create(delayTime);
                        CCCallFunc *updatePosFunc = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::updateBirdPosition));
                        birdParentNode->runAction(CCSequence::create(updatePosDelay,updatePosFunc,NULL));
                    }
                }
            }
            iter++;
        }
        dashBirds.clear();
//        dashBirds.swap(std::vector<CCArray*>());
        return true;
    }
    return false;
}


std::vector<CCArray *> PuzzleUtil::getDashBirds()
{
    std::vector<CCArray *> birds;
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
                    CCArray *birdDash = CCArray::create();
                    birdDash->addObjectsFromArray(rowDashBird);
                    birds.push_back(birdDash);
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
                    CCArray *birdDash = CCArray::create();
                    birdDash->addObjectsFromArray(rowDashBird);
                    birds.push_back(birdDash);
                    rowDashBird->removeAllObjects();
                }
            }
            else
            {
                if(appearCount>=3)
                {
                    CCArray *birdDash = CCArray::create();
                    birdDash->addObjectsFromArray(rowDashBird);
                    birds.push_back(birdDash);
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
                    CCArray *birdDash = CCArray::create();
                    birdDash->addObjectsFromArray(colDashBird);
                    birds.push_back(birdDash);
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
                    // if(!birdDash->containsObject(currentBird))
                    colDashBird->addObject(currentBird);
                }
            }
            else if(prevBird->birdType==currentBird->birdType&&currentBird->isChecked==false)
            {
                // if(!birdDash->containsObject(currentBird))
                colDashBird->addObject(currentBird);
                appearCount++;
                if(i==ShareManager::row-1&&appearCount>=3)
                {
                    CCArray *birdDash = CCArray::create();
                    birdDash->addObjectsFromArray(colDashBird);
                    birds.push_back(birdDash);
                    colDashBird->removeAllObjects();
                }
            }
            else
            {
                if(appearCount>=3)
                {
                    CCArray *birdDash = CCArray::create();
                    birdDash->addObjectsFromArray(colDashBird);
                    birds.push_back(birdDash);
                }
                colDashBird->removeAllObjects();
                //if(!birdDash->containsObject(currentBird))
                colDashBird->addObject(currentBird);
                prevBird = currentBird;
                appearCount=1;
            }
        }
    }
    return birds;
}

void PuzzleUtil::__resetBird( CCNode *pSender )
{
    sm->effectLayer->setSwallow(false);
    sm->fstBird = NULL;
    sm->sedBird = NULL;

}
/**
 * 小鸟消除后 按照列进行遍历 计算每一个小鸟下面有几个空格
 */
void PuzzleUtil::updateBirdPosition()
{
    updateQueue--;
    CCLog("updateBirdPosition quene length:%d",updateQueue);
    if(updateQueue!=0)
    {
        return;
    }
    sm->effectLayer->setSwallow(true);
    float maxTime = 0.5f;
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
                float downTime = 0.2f;//emptyCells*ShareManager::boxHeight/downSpeed;
                CCPoint targetPos = CCPointMake(bird->col*ShareManager::boxWidth,bird->row*ShareManager::boxHeight);
                CCMoveTo *moveToAct = CCMoveTo::create(downTime,targetPos);
                CCCallFuncN *moveFunc = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__moveDown));
                CCSequence *moveSeq = CCSequence::create(moveToAct,moveFunc,NULL);
                bird->runAction(moveSeq);
                if(bird->effectSprite!=NULL)
                {
                    bird->effectSprite->runAction(CCMoveTo::create(downTime,sm->birdBatchNode->convertToWorldSpace(targetPos)));
                }
            }
        }
        //小鸟的位置移动之后创建新的小鸟
        float unitTime = ShareManager::boxHeight/downSpeed;
        int idx = 0;
        maxTime = unitTime*emptyCells+0.5f;
        while(emptyCells)
        {
            int row = ShareManager::row-emptyCells;
            createNewBird(unitTime*idx,row,j);
            idx++;
            emptyCells--;
        }

    }
    //更新了位置之后 要继续检测是否有可以消除的小鸟
    CCDelayTime *checkDelay = CCDelayTime::create(maxTime);
    CCCallFunc *checkCall = CCCallFunc::create(this,callfunc_selector(PuzzleUtil::__updatePosComplete));
    sm->birdBatchNode->runAction(CCSequence::create(checkDelay,checkCall,NULL));
}

void PuzzleUtil::__moveDown( CCNode *pSender )
{
    Bird *bird = (Bird*)pSender;
    bird->setTouchEnabled(true);
    bird->setPosition(ccp(ShareManager::boxWidth*bird->col,bird->row*ShareManager::boxHeight));
    //bird->shakeBody(1.1f,0.8f);
}

void PuzzleUtil::createNewBird(float delay, short row,short col )
{
    short type = rand()%sm->birdTypes;
    Bird *bird = Bird::create(type);
    bird->setPosition(ccp(col*ShareManager::boxWidth,VisibleRect::top().y));
    ShareManager::shareManager()->birdBatchNode->addChild(bird);
    bird->row = row;
    bird->col = col;
    bird->setScale(0.9f);
    ShareManager::shareManager()->birds[row][col] = bird;
    float targetY = row*ShareManager::boxHeight;
    float moveTime = 0.4f;//(bird->getPositionY() - row*ShareManager::boxHeight)/downSpeed;
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
    sm->effectLayer->setSwallow(true);
    bool res = isCanPuzzle();
    if(res==false)
    {
        CCLog("update pos complete ,clear select bird birds num:%d",sm->birdBatchNode->getChildrenCount());
        sm->effectLayer->setSwallow(false);
        sm->fstBird = NULL;
        sm->sedBird = NULL;
        int dashBirdsCount = ShareManager::shareManager()->dashBirdsNum;
        if(dashBirdsCount>50)
        {
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/run3");
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_TIME_REWARD,CCFloat::create(10.0f));
        }
        else if(dashBirdsCount>40)
        {
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/run2");
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_TIME_REWARD,CCFloat::create(8.0f));
        }
        else if(dashBirdsCount>30)
        {
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/shoot4");
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_TIME_REWARD,CCFloat::create(6.0f));
        }
        else if(dashBirdsCount>20)
        {
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/run2");
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_TIME_REWARD,CCFloat::create(4.0f));
        }
        else if(ShareManager::shareManager()->dashBirdsNum>10)
        {
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/run1");
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_TIME_REWARD,CCFloat::create(2.0f));
        }
        sm->dashCount += dashBirdsCount;
        CCLog("dashbirds count:%d",ShareManager::shareManager()->dashBirdsNum);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTI_SHOW_SCORE);
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
        updateQueue++;
        CCLog("runEffect quene length:%d",updateQueue);
        sm->effectLayer->setSwallow(true);
        bird->effectSprite->removeFromParentAndCleanup(true);
        bird->effectSprite = NULL;
        switch(bird->effectType)
        {
        case 4:
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_starbomb");
            bombBird(bird);
            break;
        case 5:
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_firebomb");
            fireBird(bird);
            break;
        case 6:
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_blackhole");
            blackHole(bird);
            break;
        case 7:
            //SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_lighting");
            //   lightning(bird);
        default:
            SoundManager::shareSoundManager()->playEffect("sounds/SFX/item_blackhole");
            blackHole(bird);
            break;
        }
    }
}

void PuzzleUtil::fireBird( Bird *bird )
{
    CCSprite *effect = SPRITE("firebird_000.png");
    CCRepeatForever *repeateAnimate = CCRepeatForever::create(GET_ANIMATE("FireBirdEff"));
    effect->runAction(repeateAnimate);
    sm->effectLayer->addChild(effect);
    effect->setPosition(getWorldPos(bird->getPosition()));
    const float speed = 400.0f;
    float time = (effect->getPositionY()+100)/speed;
    CCMoveTo *moveAct = CCMoveTo::create(time,ccp(effect->getPositionX(),-100));
    CCCallFuncN *moveCall = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__removeEffectSprite));
    effect->runAction(CCSequence::create(moveAct,moveCall,NULL));
    bird->stopAllActions();
    __birdBurn(bird);
    int row = bird->row,col = bird->col;
    for(int i=0; i<row; i++)
    {
        Bird *currentBird = sm->birds[i][col];
        if(currentBird&&currentBird->isChecked==false)
        {
            ShareManager::shareManager()->dashBirdsNum++;
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
    const float effTime = 1.2f;
    CCSprite *effect = SPRITE("Blackhole_000.png");
    CCPoint birdPos = bird->getPosition();
    CCRepeatForever *repeateAnimate = CCRepeatForever::create(GET_ANIMATE("BlackHoleEff"));
    effect->runAction(repeateAnimate);
    sm->effectLayer->addChild(effect);
    effect->setPosition(getWorldPos(birdPos));
    CCScaleTo *scaleAct = CCScaleTo::create(effTime,0);
    CCCallFuncN *scaleCall = CCCallFuncN::create(this,callfuncN_selector(PuzzleUtil::__removeEffectSprite));
    CCSequence *scaleSeq = CCSequence::create(scaleAct,scaleCall,NULL);
    effect->runAction(scaleSeq);
    for(int i=0; i<ShareManager::row; i++)
    {
        for(int j=0; j<ShareManager::col; j++)
        {
            Bird *currentBird = sm->birds[i][j];
            if(currentBird&&currentBird->effectSprite)
            {
                //currentBird->effectSprite->removeFromParentAndCleanup(true);
                //currentBird->effectSprite = NULL;
            }
            if(currentBird&&currentBird->isChecked==false&&currentBird->birdType==bird->birdType)
            {
                currentBird->isChecked=true;
                //runEffect(currentBird);
                ShareManager::shareManager()->dashBirdsNum++;
                currentBird->getParent()->reorderChild(currentBird,100);
                float angle = rand()%360+300;
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
                CCScaleTo *scaleAct = CCScaleTo::create(0.5f,0);
                CCCallFunc *scaleCall = CCCallFunc::create(currentBird,callfunc_selector(Bird::removeFromParent));
                currentBird->runAction(CCSequence::create(scaleAct,scaleCall,NULL));
                if(currentBird->effectSprite)
                {
                    currentBird->effectSprite->removeFromParentAndCleanup(true);
                    currentBird->effectSprite = NULL;
                }
                ShareManager::shareManager()->dashBirdsNum++;
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
    updateBirdPosition();
}

void PuzzleUtil::__removeEffectSprite( CCNode *node )
{

    node->removeFromParentAndCleanup(true);
}
