#include "ChangLevelState.h"
#include "GameContext.h"
#include "Bird.h"
#include "GameLevelData.h"
#include "GameLevelBank.h"
#include "State.h"
#include "FlyingState.h"
#include "GameState.h"


ChangLevelState::ChangLevelState(GameState* gameState)
	:State(gameState), _ready(false)
{
}


ChangLevelState::~ChangLevelState(void)
{
}

bool ChangLevelState::initWithLevel( GamePrototol* context, int levelId )
{
	const float timeDelay = 1.5f;
	_levelId = levelId;
	GameLevelData* levelData = GameLevelBank::getInstance()->getGameLevelData(_levelId);
	Bird* bird = context->getBird();
	bird->stopAllActions();
	float timedelay = 2;
	CallFuncN* callFunc = CallFuncN::create(std::bind(&ChangLevelState::doChangeLevel, this, context));
	bird->runAction(Sequence::create(DelayTime::create(timeDelay), callFunc, NULL));
	context->getBackground()->reset();
	
	return true;
}

void ChangLevelState::update( GamePrototol* context, float gameTime, bool tap )
{
	Bird* bird = context->getBird();
	float limitedHeight = Director::getInstance()->getWinSize().height - 100;
	CCLOG("%f", bird->getPositionY());
	if (bird->getPosition().y < limitedHeight)
		bird->force(Vec2(40, 40));
	else
		bird->setVelocityX(10);

	if (_ready) {
		if (bird->getPositionX() > 0)
			_gameState->changeState(FlyingState::createWithLevel(_gameState, context, _levelId));
	}
}

ChangLevelState* ChangLevelState::createWithLevel( GameState* gameState, GamePrototol* context, int levelId )
{
	ChangLevelState* result = new ChangLevelState(gameState);
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

void ChangLevelState::doChangeLevel(GamePrototol* context)
{
	Bird* bird = context->getBird();
	Terrain* terrain = context->getTerrain();
	Background* background = context->getBackground();

	bird->setPosition(- 1500, bird->getPositionY());
	CCLOG("Change to LEVEL: %d", _levelId);
	GameLevelData* levelData = GameLevelBank::getInstance()->getGameLevelData(_levelId);
	terrain->parseTerrain(levelData);
	background->changeColorWithAnimation(levelData->colorBg, 1.0f);
	_ready = true;
}
