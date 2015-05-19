#pragma once
#include <cocos2d.h>

using namespace cocos2d;

class GameState;
class GamePrototol;

class State : public Ref
{
protected:
	int _levelId;
	GameState* _gameState;
public:
	State(GameState* gameState);
	~State();
	virtual bool initWithLevel(GamePrototol* context, int levelId) = 0;
	virtual void update(GamePrototol* context, float gameTime, bool tap) = 0;
};

