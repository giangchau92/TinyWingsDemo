#include "State.h"
#include "GameState.h"

State::State( GameState* gameState )
{
	_gameState = gameState;
	_gameState->retain();
}

State::~State()
{
	CC_SAFE_RELEASE(_gameState);
}
