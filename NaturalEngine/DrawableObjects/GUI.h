#pragma once
#include "Engine/Window.h"
#include "drawableObjects.h"

#include <list>

class GUI : public drawableObject
{
public:
	GUI(Window& w);
	~GUI();

	virtual void draw();
	virtual void update();

	GUI& subscribe(drawableObject* subscriber);

private:
	std::list<drawableObject*> subscribers;
};