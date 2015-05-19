#include "StripCreateNode.h"
#include <vector>

using namespace std;

StripCreateNode::StripCreateNode(void)
{
}


StripCreateNode::~StripCreateNode(void)
{
}

Sprite* StripCreateNode::createSpriteWithStrip( ccColor4F color1, ccColor4F color2, int width, int height, int nStrips )
{
	RenderTexture* rt = RenderTexture::create(width, height);
	rt->beginWithClear(color1.r, color1.g, color1.b, color1.a);

	/*Sprite* noise = Sprite::create("Noise.png");
	noise->setAnchorPoint(ccp(0, 0));
	BlendFunc blenFunc;
	blenFunc.src = GL_DST_COLOR;
	blenFunc.dst = GL_ZERO;
	noise->setBlendFunc(blenFunc);*/
	//noise->visit();
	
	_customCommand2.init(_globalZOrder);
	_customCommand2.func = [=]()->void {
		vector<Point> vertices;
		vector<Color4F> colors;

		float dx = width / nStrips;
		int n = ceil(nStrips * (height / (float)width));
		float px1, px2;
		for (int i = 0; i < n ; i++)
		{
			px1 = i * 2 * dx;
			px2 = (i * 2 + 1) * dx;
			vertices.push_back(Point(px1, height));
			vertices.push_back(Point(px1 - width, 0));
			vertices.push_back(Point(px2, height));
			vertices.push_back(Point(px1 - width, 0));
			vertices.push_back(Point(px2, height));
			vertices.push_back(Point(px2 - width, 0));
			for (int j = 0; j < 6; j++)
				colors.push_back(color2);
		}
		setGLProgram(GLProgramCache::sharedShaderCache()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
		CC_NODE_DRAW_SETUP();
		ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, &colors[0]);
		glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		// gratient
		float boderA = 0.0f;
		vertices.clear();
		colors.clear();
		vertices.push_back(ccp(0, 0));
		colors.push_back(ccColor4F(ccColor4B(200, 200, 200, 255)));
		vertices.push_back(ccp(width, 0));
		colors.push_back(ccColor4F(ccColor4B(200, 200, 200, 255)));
		vertices.push_back(ccp(0, height));
		colors.push_back(ccColor4F(ccColor4B(62, 62, 62, 255)));
		vertices.push_back(ccp(width, height));
		colors.push_back(ccColor4F(ccColor4B(62, 62, 62, 255)));

		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, &colors[0]);
		glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());

	};
	Director::getInstance()->getRenderer()->addCommand(&_customCommand2);

	rt->end();
	return rt->getSprite();
}

bool StripCreateNode::init()
{
	if (!Node::init())
		return false;
	return true;
}
