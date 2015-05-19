#pragma once
#include "State.h"
class FlyingState : public State
{
public:
	FlyingState(GameState* gameState);
	~FlyingState(void);

	virtual bool initWithLevel(GamePrototol* context, int levelId);
	virtual void update( GamePrototol* context, float gameTime, bool tap );
	static FlyingState* createWithLevel(GameState* gameState, GamePrototol* context, int levelId);
};

