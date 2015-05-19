#include "HelloWorldScene.h"
#include "BeginState.h"

USING_NS_CC;


HelloWorld::HelloWorld()
	:_tap(false),
	_bird(NULL),
	_terrain(NULL),
	_background(NULL),
	_world(NULL),
	_gameState(NULL)
{

}

HelloWorld::~HelloWorld()
{
	CC_SAFE_RELEASE(_bird);
	CC_SAFE_RELEASE(_terrain);
	CC_SAFE_RELEASE(_background);
	CC_SAFE_DELETE(_world);
	CC_SAFE_RELEASE(_gameState);
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	b2Vec2 gravity = b2Vec2(0, -7.0f);
	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);

	_background = Background::createWithColor(ccColor4F(ccColor4B(255, 0, 0, 255)));
	_background->retain();
	addChild(_background);
    
    _terrain = Terrain::createWithWorld(_world);
	_terrain->retain();
	addChild(_terrain);

	_bird = Bird::createWithWorld(_world);
	_bird->retain();
	_terrain->addChild(_bird);
    
	// touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	// Key event
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	scheduleUpdate();

	// game state
	_gameState = GameState::create();
	_gameState->retain();
	_gameState->changeState(BeginState::createWithLevel(_gameState, this, 0));

    return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();
}

void HelloWorld::update( float delta )
{
	Layer::update(delta);
	
	/*if (_tap)
	{
		if (!_bird->getAwake())
		{
			_bird->wake();
			_tap = false;
		}
		else
			_bird->dive();
	}
	*/
	_gameState->update(this, delta, _tap);

	static double UPDATE_INTERVAL = 1.0f/60.0f;
	static double MAX_CYCLES_PER_FRAME = 5;
	static double timeAccumulator = 0;

	timeAccumulator += delta;
	if (timeAccumulator > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
		timeAccumulator = UPDATE_INTERVAL;
	}

	int32 velocityIterations = 3;
	int32 positionIterations = 2;
	while (timeAccumulator >= UPDATE_INTERVAL) {
		timeAccumulator -= UPDATE_INTERVAL;
		_world->Step(UPDATE_INTERVAL,
			velocityIterations, positionIterations);
		_world->ClearForces();

	}

	Size winSize = Director::getInstance()->getWinSize();
	_bird->update(delta);
	_terrain->setOffset(_bird->getPosition().x);
	/*const float limitedTop = winSize.height - 50;
	Point pos = _bird->getPosition();
	if (pos.y > limitedTop)
		_terrain->setScale(limitedTop / pos.y);*/
	
}

bool HelloWorld::onTouchBegan( Touch *touch, Event *unused_event )
{
	_tap = true;
	return true;
}

void HelloWorld::onTouchEnded( Touch *touch, Event *unused_event )
{
	_tap = false;
}

void HelloWorld::onKeyPressed( EventKeyboard::KeyCode keyCode, Event* event )
{
	//_terrain->setOffset(_terrain->getOffset() + 500);
}

Bird* HelloWorld::getBird()
{
	return _bird;
}

Terrain* HelloWorld::getTerrain()
{
	return _terrain;
}

Background* HelloWorld::getBackground()
{
	return _background;
}

b2World* HelloWorld::getWorld()
{
	return _world;
}