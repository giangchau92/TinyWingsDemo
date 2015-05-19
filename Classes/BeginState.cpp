#include "BeginState.h"
#include "GameLevelBank.h"
#include "FlyingState.h"
#include "GameState.h"
#include "GameContext.h"


BeginState::BeginState(GameState* gameState)
	:State(gameState)
{
}


BeginState::~BeginState(void)
{
}

void BeginState::update( GamePrototol* context, float gameTime, bool tap )
{
	Bird* _bird = context->getBird();
	if (tap)
	{
		if (!_bird->getAwake())
		{
			_bird->wake();
			_gameState->changeState(FlyingState::createWithLevel(_gameState, context, _levelId));
		}
	}
}

BeginState* BeginState::createWithLevel( GameState* gameState, GamePrototol* context, int levelId )
{
	BeginState* result = new BeginState(gameState);
	if (result && result->initWithLevel(context, levelId))
	{
		result->autorelease();
		return result;
	} else {
		delete result;
		result = NULL;
		return NULL;
	}
}

bool BeginState::initWithLevel( GamePrototol* context, int levelId )
{
	_levelId = levelId;
	GameLevelData* data = GameLevelBank::getInstance()->getGameLevelData(levelId);
	_levelTime = data->time;
	GameLevelBank::getInstance()->initAllStrip();
	context->getTerrain()->parseTerrain(data);
	context->getBackground()->changeColor(data->colorBg);
	//context->getBackground()->changeColorWithAnimation(data->colorBg, 5.0f);

	Point startPoint = context->getTerrain()->getStartPoint();
	context->getBird()->setPosition(startPoint.x, startPoint.y);

	return true;
}
