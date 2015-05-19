#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Terrain.h"
#include "Box2D/Box2D.h"
#include "Bird.h"
#include "Background.h"
#include "GameContext.h"
#include "GameState.h"

using namespace cocos2d;


class HelloWorld : public cocos2d::Layer, public GamePrototol
{
	bool _tap;

	GameState* _gameState;
	Bird* _bird;
	Terrain* _terrain;
	Background* _background;
	b2World* _world;
public:
    HelloWorld();
	~HelloWorld();
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onEnter();
	virtual void update(float delta);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	virtual Bird* getBird();

	virtual Terrain* getTerrain();

	virtual Background* getBackground();

	virtual b2World* getWorld();

	CREATE_FUNC(HelloWorld);

};

#endif // __HELLOWORLD_SCENE_H__
