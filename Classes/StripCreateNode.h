#pragma once
#include <cocos2d.h>

using namespace cocos2d;

class StripCreateNode : public Node
{
	CustomCommand _customCommand2;
public:
	StripCreateNode(void);
	~StripCreateNode(void);
	bool init();

	Sprite* createSpriteWithStrip(ccColor4F color1, ccColor4F color2, int with, int height, int nStrips);

	CREATE_FUNC(StripCreateNode);
};

