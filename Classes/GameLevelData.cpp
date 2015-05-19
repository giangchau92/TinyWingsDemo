#include "GameLevelData.h"
#include "StripCreateNode.h"

void GameLevelData::initStrip()
{
	if (stripsSprite)
		return;
	stripCreate = StripCreateNode::create();
	stripCreate->retain();
	stripsSprite = stripCreate->createSpriteWithStrip(colorStrip1, colorStrip2, 512, 512, 6);
	stripsSprite->retain();
}

GameLevelData::GameLevelData()
{
	stripsSprite = NULL;
	stripCreate = NULL;
}

GameLevelData::~GameLevelData()
{
	CC_SAFE_RELEASE(stripsSprite);
	CC_SAFE_RELEASE(stripCreate);
}
