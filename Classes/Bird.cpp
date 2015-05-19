#include "Bird.h"
#include "Config.h"
#include <vector>


Bird::Bird(void)
	:_world(NULL),
	_body(NULL), _awake(false)
{
}


Bird::~Bird(void)
{
}

bool Bird::initWithWorld( b2World* world )
{
	if (!Sprite::initWithFile("bird.png"))
		return false;
	CCASSERT(world != nullptr, "b2 world null");
	_world = world;

	createBox2dBody();

	this->setAnchorPoint(ccp(0.5f, 0.5f));
	this->setScale(0.5f);
	return true;
}

void Bird::createBox2dBody()
{
	if (_body)
		_world->DestroyBody(_body);
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.linearDamping = 0.1f; // dang nhu ma sat -> giam dan van toc tuyen tinh
	bdef.fixedRotation = true; // khong cho xoay -> dung cho nhan vat
	bdef.position.Set(0, 0);
	_body = _world->CreateBody(&bdef);

	b2CircleShape shape;
	shape.m_radius = 16.0f / PTM_RATIO;

	b2FixtureDef fdef;
	fdef.shape = &shape;
	fdef.density = 1.0f/CC_CONTENT_SCALE_FACTOR();
	fdef.restitution = 0.0f;
	fdef.friction = 0.2f;

	_body->CreateFixture(&fdef);
}

void Bird::update( float dt )
{
	const float limited = 3.0f;
	this->setPosition(_body->GetPosition().x * PTM_RATIO, _body->GetPosition().y * PTM_RATIO);
	b2Vec2 vel = _body->GetLinearVelocity();
	if (_awake && vel.x < limited)
		_body->SetLinearVelocity(b2Vec2(limited + 1, 0));

	limitVelocity();

	CCLOG("Position: %f", this->getPositionX());
	//CCLOG("Angle: %f | %f", vel.x, vel.y);
	b2Vec2 weightedVel = vel;
	const int NUM_PREV_VELS = 5;
	queue<b2Vec2> quue;
	for(int i = 0; i < _prevVels.size(); ++i) {
		weightedVel += _prevVels[i];
	}
	weightedVel = b2Vec2(weightedVel.x/NUM_PREV_VELS, weightedVel.y/NUM_PREV_VELS);
	_prevVels.push_back(vel);
	if (_prevVels.size() >= NUM_PREV_VELS)
		_prevVels.clear();
	vel = weightedVel;
	if (vel.x < 0) {
		vel.x = - vel.x;
		vel.y = - vel.y;
	}
	float angle = -1 * CC_RADIANS_TO_DEGREES(ccpToAngle(ccp(vel.x, vel.y)));
	angle = MIN(90, angle);
	angle = MAX(-90, angle);
	if (_awake)
		this->setRotation(angle);
}

Bird* Bird::createWithWorld( b2World* world )
{
	Bird* result = new Bird();
	if (result && result->initWithWorld(world))
	{
		result->autorelease();
		return result;
	} else {
		delete result;
		result = NULL;
		return NULL;
	}
}

void Bird::wake()
{
	_awake = true;
	_body->SetActive(true);
	_body->ApplyLinearImpulse(b2Vec2(1, 2), _body->GetPosition(), true);
}

void Bird::dive()
{
	_body->ApplyForce(b2Vec2(30, -50), _body->GetPosition(), true);
}

bool Bird::getAwake()
{
	return _awake;
}

void Bird::setPosition( float x, float y )
{
	Sprite::setPosition(x, y);
	_body->SetTransform(b2Vec2(x / PTM_RATIO, y / PTM_RATIO), 0);
}

void Bird::force( Vec2 force )
{
	if (_body)
		_body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void Bird::setVelocityX( float velX )
{
	if (_body)
		_body->SetLinearVelocity(b2Vec2(velX, 0));
}

void Bird::reset()
{
	_awake = false;
	this->setRotation(0);
	_body->SetLinearVelocity(b2Vec2(0,0));
	_body->SetAngularVelocity(0);
}

void Bird::limitVelocity()
{
	if (!_awake) return;

	const float minVelocityX = 5;
	const float minVelocityY = -40;
	b2Vec2 vel = _body->GetLinearVelocity();
	if (vel.x < minVelocityX) {
		vel.x = minVelocityX;
	}
	if (vel.y < minVelocityY) {
		vel.y = minVelocityY;
	}
	_body->SetLinearVelocity(vel);
}

