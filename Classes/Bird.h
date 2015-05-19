#pragma once
#include "cocos2d.h"
#include "box2D/Box2D.h"
#include <vector>

using namespace cocos2d;
using namespace std;

class Bird : public Sprite
{
	b2World* _world;
	b2Body* _body;
	bool _awake;
	vector<b2Vec2> _prevVels;

	void createBox2dBody();
public:
	Bird(void);
	~Bird(void);

	bool initWithWorld(b2World* world);
	bool getAwake();
	void wake();
	void dive();
	void limitVelocity();
	void force(Vec2 force);
	void setVelocityX(float velX);
	void update(float dt);
	void reset();
	virtual void setPosition(float x, float y);
	static Bird* createWithWorld(b2World* world);
};

