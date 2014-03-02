#include "HelloWorldScene.h"

USING_NS_CC;

b2World *world;
B2Sprite *bird;
CCSize screenSize;
B2Sprite *ground ;
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
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    screenSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	initWorld();

	addBird();
    
	addBarContainer();

	addGround();

	scheduleOnce(schedule_selector(HelloWorld::startGame),1.5);

	setTouchEnabled(true);
    
    return true;
}

void HelloWorld::initWorld()
{
	//初始化世界的重力大小和方向
	world = new b2World(b2Vec2(0.0f,-15.0f));

	world->SetContactListener(this);
}

void HelloWorld::addBird()
{
	bird = B2Sprite::create("bird.png");

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	//初试位置,单位为米
	bodydef.position = b2Vec2(screenSize.width/2/RATIO,9);
	b2Body *birdBody = world->CreateBody(&bodydef);

	// 将小鸟当做矩形
	b2PolygonShape birdShape;
	birdShape.SetAsBox(bird->getContentSize().width/2/RATIO,bird->getContentSize().height/2/RATIO);
	//创建碰撞检测
	b2FixtureDef birdFixtureDef;
	birdFixtureDef.shape = &birdShape;
	birdBody->CreateFixture(&birdFixtureDef);

	bird ->setB2Body(birdBody);
	bird -> setPTMRatio(RATIO);
	addChild(bird); 
}

void HelloWorld::update(float dt)
{
	//时间间隔，速度迭代，位置迭代
	world -> Step(dt,8,3);
	CCSprite *s;

	for(b2Body *c=world->GetBodyList();c!=NULL;c=c->GetNext())
	{
		if(c->GetPosition().x < -3)
		{
			s = (CCSprite*)c->GetUserData();
			if(s != NULL)
			{
				s->removeFromParent();
			}

			world ->DestroyBody(c);
		}
	}
}

void HelloWorld::addGround()
{
	// 添加地面
	ground = B2Sprite::create("ground.png");
	b2BodyDef groundDef;
	groundDef.type = b2_staticBody;
	groundDef.position = b2Vec2(ground->getContentSize().width/2/RATIO,ground->getContentSize().height/2/RATIO);

	b2Body *groundBody = world->CreateBody(&groundDef);
	
	b2PolygonShape gshape;
	gshape.SetAsBox(ground->getContentSize().width/2/RATIO,ground->getContentSize().height/2/RATIO);

	b2FixtureDef gfix;
	gfix.shape = &gshape;
	groundBody->CreateFixture(&gfix);

	ground->setB2Body(groundBody);
	ground->setPTMRatio(RATIO);
	addChild(ground);
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

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	//设置线性速度
	bird->getB2Body()->SetLinearVelocity(b2Vec2(0,5));
}

void HelloWorld::addBar(float dt)
{
	float offSet = -rand()%5;
	CCSize groundSize = ground->getContentSize();
	//down
	B2Sprite *downBar = B2Sprite::create("bar.png");

	b2BodyDef downDef;
	downDef.type = b2_kinematicBody;// 不受重力影响的类型
	downDef.position = b2Vec2(screenSize.width/RATIO+2,downBar->getContentSize().height/2/RATIO+offSet+groundSize.height/2/RATIO);
	downDef.linearVelocity = b2Vec2(-5,0);
	b2Body *downBarBody = world->CreateBody(&downDef);

	b2PolygonShape downShape;
	downShape.SetAsBox(downBar->getContentSize().width/2/RATIO,downBar->getContentSize().height/2/RATIO);

	b2FixtureDef downFix;
	downFix.shape = &downShape;
	downBarBody->CreateFixture(&downFix);

	downBar->setB2Body(downBarBody);
	downBar->setPTMRatio(RATIO);
	barContainer->addChild(downBar);
	//end down

	B2Sprite *upBar = B2Sprite::create("bar.png");

	b2BodyDef upDef;
	upDef.type = b2_kinematicBody;// 不受重力影响的类型
	upDef.position = b2Vec2(screenSize.width/RATIO+2,downBar->getContentSize().height/2/RATIO+offSet+2+upBar->getContentSize().height/RATIO+groundSize.height/2/RATIO);
	upDef.linearVelocity = b2Vec2(-5,0);
	b2Body *upBarBody = world->CreateBody(&upDef);

	b2PolygonShape upShape;
	upShape.SetAsBox(upBar->getContentSize().width/2/RATIO,upBar->getContentSize().height/2/RATIO);

	b2FixtureDef upFix;
	upFix.shape = &upShape;
	upBarBody->CreateFixture(&upFix);

	upBar->setB2Body(upBarBody);
	upBar->setPTMRatio(RATIO);
	barContainer->addChild(upBar);
}

void HelloWorld::addBarContainer()
{
	barContainer = CCSprite::create();
	addChild(barContainer);
}

void HelloWorld::BeginContact(b2Contact *contact)
{
	if(contact->GetFixtureA()->GetBody()->GetUserData() == bird
		|| contact->GetFixtureB()->GetBody()->GetUserData() == bird)
	{
		stopGame();
		CCMessageBox("Failed!","Game Failed!");
	}
}

void HelloWorld::startGame(float dt)
{
	scheduleUpdate();
	schedule(schedule_selector(HelloWorld::addBar),1);
}

void HelloWorld::stopGame()
{
	unscheduleUpdate();
	unschedule(schedule_selector(HelloWorld::addBar));
}
