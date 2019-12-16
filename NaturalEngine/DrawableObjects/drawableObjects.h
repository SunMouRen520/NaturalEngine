#pragma once
#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "sceneElements.h"

class drawableObject
{
public:
	virtual void draw() = 0;
	virtual void setGui() {};

	// 如果这个类会加入一些逻辑，那么它必须在每个游戏循环中通过调用update来刷新，否则就不要覆盖它。
	virtual void update() {};

public:
	static sceneElements * scene;
};


#endif // !DRAWABLEOBJECT_H
