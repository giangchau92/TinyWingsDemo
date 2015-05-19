#pragma once
#include "Bird.h"
#include "Terrain.h"
#include "Background.h"
#include <Box2D/Box2D.h>

class GamePrototol
{
public:
	GamePrototol();
	~GamePrototol();
	virtual Bird* getBird() = 0;
	virtual Terrain* getTerrain() = 0;
	virtual Background* getBackground() = 0;
	virtual b2World* getWorld() = 0;
};