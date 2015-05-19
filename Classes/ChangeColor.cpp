#include "ChangeColor.h"


ChangeColor::ChangeColor(void)
{
}


ChangeColor::~ChangeColor(void)
{
}

void ChangeColor::update( float time )
{
	if (_target)
	{
		Color4F deltaColor;
		deltaColor.r = _endColor.r - _startColor.r;
		deltaColor.g = _endColor.g - _startColor.g;
		deltaColor.b = _endColor.b - _startColor.b;
		deltaColor.a = _endColor.a - _startColor.a;
		_targetColor->r = _startColor.r + time * deltaColor.r;
		_targetColor->g = _startColor.g + time * deltaColor.g;
		_targetColor->b = _startColor.b + time * deltaColor.b;
		_targetColor->a = _startColor.a + time * deltaColor.a;
	}
}

void ChangeColor::startWithTarget( Node *target )
{
	ActionInterval::startWithTarget(target);
	_startColor = *_targetColor;
}

ChangeColor* ChangeColor::create( float duration, Color4F* targetColor, Color4F endColor )
{
	ChangeColor* result = new ChangeColor();
	if (result && result->initWithDuration(duration, targetColor, endColor))
	{
		result->autorelease();
	} else {
		delete result;
		result = NULL;
	}
	return result;
}

bool ChangeColor::initWithDuration( float duration, Color4F* targetColor, Color4F color )
{
	if (!ActionInterval::initWithDuration(duration))
		return false;
	_targetColor = targetColor;
	_endColor = color;
	return true;
}
