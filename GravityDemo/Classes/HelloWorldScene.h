#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;

#define PTM_RATIO 32

#define WORLD_TO_SCREEN(n) ((n) * PTM_RATIO)

class HelloWorld : public CCSprite, public CCTargetedTouchDelegate
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
	void tick(float dt);
	void createStar(CCPoint p);
	// overwrite virtual function
	virtual void onEnter();
    virtual void onExit();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
