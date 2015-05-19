#pragma once
#include "State.h"
class BeginState : public State
{
private:
	float _levelTime;
public:
	BeginState(GameState* gameState);
	~BeginState(void);

	
	virtual bool initWithLevel(GamePrototol* context, int levelId);
	virtual void update( GamePrototol* context, float gameTime, bool tap );

	static BeginState* createWithLevel(GameState* gameState, GamePrototol* context, int levelId);

};

