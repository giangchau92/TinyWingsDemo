#include "GameState.h"
#include "State.h"
#include "GameContext.h"



GameState::GameState(void)
	:_currentState(NULL)
{
}


GameState::~GameState(void)
{
	CC_SAFE_RELEASE(_currentState);
}

void GameState::changeState( State* newState )
{
	if (_currentState)
		_currentState->release();
	_currentState = newState;
	_currentState->retain();
}

GameState* GameState::create()
{
	GameState* result = new GameState();
	if (result) {
		result->autorelease();
		return result;
	} else {
		delete result;
		result = NULL;
		return NULL;
	}
}

void GameState::update( GamePrototol* context, float gameTime, bool tap )
{
	if (_currentState)
		_currentState->update(context, gameTime, tap);
}
