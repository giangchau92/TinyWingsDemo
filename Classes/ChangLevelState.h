#pragma once
#include "State.h"
class ChangLevelState : public State
{
	bool _ready;
	void doChangeLevel(GamePrototol* context);
public:
	ChangLevelState(GameState* gameState);
	~ChangLevelState(void);

	virtual bool initWithLevel(GamePrototol* context, int levelId);
	virtual void update( GamePrototol* context, float gameTime, bool tap );
	static ChangLevelState* createWithLevel(GameState* gameState, GamePrototol* context, int levelId);
};

