#include "FlyingState.h"
#include "GameContext.h"
#include "ChangLevelState.h"
#include "State.h"
#include "GameState.h"
#include "GameLevelData.h"
#include "GameLevelBank.h"
#include "BeginState.h"

FlyingState::FlyingState( GameState* gameState )
	:State(gameState)
{

}

FlyingState::~FlyingState(void)
{
}

void FlyingState::update( GamePrototol* context, float gameTime, bool tap )
{
	Bird* bird = context->getBird();
	if (tap)
		bird->dive();

	Terrain* terrain = context->getTerrain();
	Point endPoint = terrain->getEndPoint();
	if (bird->getPositionX() > endPoint.x)
		_gameState->changeState(ChangLevelState::createWithLevel(_gameState, context, _levelId + 1));
}

bool FlyingState::initWithLevel( GamePrototol* context, int levelId )
{
	_levelId = levelId;
	GameLevelData* lvldata = GameLevelBank::getInstance()->getGameLevelData(levelId);
	context->getBackground()->start(lvldata->time);
	context->getBird()->stopActionByTag(11);
	GamePrototol* ct = context;
	Sequence* action = Sequence::create(DelayTime::create(lvldata->time),
		CallFunc::create([=]()->void {
			ct->getBackground()->reset();
			ct->getBird()->reset();
			_gameState->changeState(BeginState::createWithLevel(_gameState, ct, 0));
	}), NULL);
	action->setTag(11);
	context->getBird()->runAction(action);
	return true;
}

FlyingState* FlyingState::createWithLevel( GameState* gameState, GamePrototol* context, int levelId )
{
	FlyingState* result = new FlyingState(gameState);
	if (result && result->initWithLevel(context, levelId))
	{
		result->autorelease();
		return result;
	} else {
		delete result;
		result = NULL;
		return result;
	}
}
