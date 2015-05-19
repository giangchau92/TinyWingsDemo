#pragma once
#include <cocos2d.h>

using namespace cocos2d;

class State;
class GamePrototol;

class GameState : public Ref
{
	State* _currentState;
public:
	GameState(void);
	~GameState(void);

	void changeState(State* newState);
	void update(GamePrototol* context, float gameTime, bool tap);
	static GameState* create();
};

