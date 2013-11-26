#include "BaseMenuItemSprite.h"
#include "managers/SoundManager.h"

BaseMenuItemSprite * BaseMenuItemSprite::create( CCNode* normalSprite, CCNode* pushSprite,CCNode *target,SEL_MenuHandler sel )
{
    BaseMenuItemSprite *item = new BaseMenuItemSprite();
    if(item)
    {
        item->m_target = target;
        item->sel = sel;
        item->initWithNormalSprite(normalSprite,pushSprite,NULL,item,menu_selector(BaseMenuItemSprite::__delegateSelector));
        item->autorelease();
        return item;
    }
    return NULL;
}

void BaseMenuItemSprite::__delegateSelector( CCObject *pSender )
{
	SoundManager::shareSoundManager()->playEffect("sounds/SFX/buttonclick.mp3");
    if(sel&&m_target)
    {
        (m_target->*sel)(pSender);
    }

}
