#include "HelloWorldScene.h"

b2World* world;
b2Vec2 gravity;


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
	if ( !CCSprite::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    

	gravity.Set(0.0f,-WORLD_TO_SCREEN(9.8));
	bool doSleep = true;

	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);



	schedule(schedule_selector(HelloWorld::tick));

    return true;
}

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent* event)
{
	HelloWorld::createStar(touch->getLocation());
	return true;
}

void HelloWorld::createStar(CCPoint p)
{
	auto starSprite = CCSprite::create("CloseNormal.png");
	starSprite -> setPosition(p);
	this->addChild(starSprite,0);

	// add physics

	b2BodyDef starBodyDef;
	starBodyDef.type = b2_dynamicBody;
	starBodyDef.position = b2Vec2(starSprite->getPositionX(),starSprite->getPositionY());
	starBodyDef.userData = starSprite;

	auto starBody = world->CreateBody(&starBodyDef);

	b2CircleShape circle;
	circle.m_radius = WORLD_TO_SCREEN(0.6);

	b2FixtureDef starFixDef;
	starFixDef.shape = &circle;
	starFixDef.density = 1.0f;
	starFixDef.friction = 0.6f;
	starFixDef.restitution = 0.0f;

	starBody->CreateFixture(&starFixDef);
}

void HelloWorld::tick(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 3;
	world->Step(dt,velocityIterations,positionIterations);

	for(auto b = world->GetBodyList();b!=NULL;b=b->GetNext())
	{
		
		if(b->GetUserData() != NULL)
		{
			auto myActor = (CCSprite*) b->GetUserData();
			myActor -> setPosition(CCPoint(b->GetPosition().x,b->GetPosition().y));
			myActor->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));

			if(myActor->getPositionY()< - WORLD_TO_SCREEN(1)
				|| myActor->getPositionX() < - WORLD_TO_SCREEN(1)
				|| myActor->getPositionX() > CCDirector::sharedDirector() ->getVisibleSize().width + WORLD_TO_SCREEN(1) )
			{
				this->removeChild(myActor,0);
				world->DestroyBody(b);
				
			}
		}
		
	}
}

void HelloWorld::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void HelloWorld::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}
void HelloWorld::touchDelegateRetain()
{
    this->retain();
}

void HelloWorld::touchDelegateRelease()
{
    this->release();
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
