#pragma once
#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "sceneElements.h"

class drawableObject
{
public:
	virtual void draw() = 0;
	virtual void setGui() {};

	// ������������һЩ�߼�����ô��������ÿ����Ϸѭ����ͨ������update��ˢ�£�����Ͳ�Ҫ��������
	virtual void update() {};

public:
	static sceneElements * scene;
};


#endif // !DRAWABLEOBJECT_H
