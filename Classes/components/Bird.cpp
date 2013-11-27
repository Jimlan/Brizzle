#include "Bird.h"
#include "managers/SoundManager.h"
#include "managers/ShareManager.h"

void Bird::onEnter()
{
    CCSprite::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_iTouchPriority,false);
}

Bird * Bird::create( short type )
{
    Bird *bird = new Bird();
    CCString *name = CCString::createWithFormat("box0%d_normal_00@2x.png",type);
    if(bird&&bird->initWithSpriteFrameName(name->getCString()))
    {
		bird->birdType = type;
        bird->autorelease();
        CCArray *frames = CCArray::create();
        CCString *blinkFrame = CCString::createWithFormat("box0%d_normal_02@2x.png",type);
        frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString()));
        frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(blinkFrame->getCString()));
        frames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString()));
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(frames,0.2f);
        bird->blinkAct = CCAnimate::create(animation);
        bird->blink();
        return bird;
    }
    return NULL;
}
/* 小鸟眨眼的action 在回调函数中 循环调用*/
void Bird::blink()
{
    float delaySec = rand()%10+2.0f;
    CCDelayTime *delayTime = CCDelayTime::create(delaySec);
    CCCallFunc *callfunc = CCCallFunc::create(this,callfunc_selector(Bird::blink));
    runAction(CCSequence::create(delayTime,blinkAct,callfunc,NULL));
}

void Bird::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    if(_isContainPoint(pTouch))
    {
        const float actTime = 0.1f;
        SoundManager::shareSoundManager()->playEffect("sounds/SFX/Bird_droped.mp3");
        CCActionInterval *scaleIn = CCScaleBy::create(0.2f,0.7f,1.2f);
        CCActionInterval *scaleBack = CCEaseElasticOut::create(CCScaleTo::create(0.6f,0.9f,0.9f));
        CCSequence *scaleSeq = CCSequence::create(scaleIn,scaleBack,NULL);
        runAction(scaleSeq);
        ShareManager *sm = ShareManager::shareManager();
		if(isMoving==false){
			__recordBird();
		}
        if(sm->sedBird!=NULL)
        {
            sm->fstBird = NULL;
            sm->sedBird = NULL;
        }
        BaseSprite::ccTouchEnded(pTouch,pEvent);
    }

}

void Bird::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    if(_isContainPoint(pTouch)&&isMoving==false)
    {
		__recordBird();
        BaseSprite::ccTouchMoved(pTouch,pEvent);
    }
}

bool Bird::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    BaseSprite::ccTouchBegan(pTouch,pEvent);
    return true;
}

void Bird::__recordBird()
{
    ShareManager *sm = ShareManager::shareManager();
    Bird *first = sm->fstBird;
    Bird *second = sm->sedBird;
    if(first==NULL)
    {
        sm->fstBird = this;
        CCLog("first");
    }
    else if(second==NULL&&first!=this)
    {
        sm->sedBird = this;
        int fbRow = first->row;
        int fbCol = first->col;
        /*
         如果点击了第二个小鸟后 要判断是不是同一行或者同一列 相邻的小鸟
        */
        bool sameRowNeighbor = fbRow==row?abs(fbCol-col)==1:false;
        bool sameColNeighbor = fbCol==col?abs(fbRow-row)==1:false;
        if(sameColNeighbor||sameRowNeighbor)
        {
            __changeBirdPos();
        }
        CCLog("second");

    }
}

void Bird::__changeBirdPos()
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
    CCPoint secPos = getPosition();
    CCMoveTo *sedMoveAct = CCMoveTo::create(0.2f,fstPos);
    CCMoveTo *fstMoveAct = CCMoveTo::create(0.2f,secPos);
	CCCallFuncN *moveCall = CCCallFuncN::create(this,callfuncN_selector(Bird::__moveEnd));
    fst->runAction(CCSequence::create(fstMoveAct,moveCall,NULL));
    sed->runAction(CCSequence::create(sedMoveAct,moveCall,NULL));
	fst->isMoving = true;
	sed->isMoving = true;
	/* 更新小鸟的行列属性 */
    fst->row = row;
    fst->col = col;
    row = fstRow;
    col = fstCol;
}

void Bird::__moveEnd( CCNode *node )
{
	CCLog("move end");
	Bird *bird = (Bird*)node;
	bird->isMoving = false;
}

