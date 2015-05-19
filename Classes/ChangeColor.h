#pragma once
#include <cocos2d.h>

using namespace cocos2d;

class ChangeColor : public ActionInterval
{
public:
	ChangeColor(void);
	~ChangeColor(void);

	virtual void startWithTarget(Node *target);
	virtual void update(float time);
	bool initWithDuration(float duration, Color4F* targetColor, Color4F color);

	static ChangeColor* create(float duration, Color4F* targetColor, Color4F endColor);

protected:
	Color4F* _targetColor;
	Color4F _startColor;
	Color4F _endColor;
};

