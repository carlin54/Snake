/*
 * Apple.h
 *
 *  Created on: Nov 28, 2015
 *      Author: carlin
 */

#ifndef APPLE_H_
#define APPLE_H_

#include "Global.h"


class Apple : public Element {

public:
	Apple()						:Element(vec2(0,0), Renderable::Apple)	{};
	Apple(vec2 position)		:Element(position,  Renderable::Apple)	{};
	~Apple()															{};

	Apple& operator=(const vec2& newPosition);
	Apple& operator=(const Element& newElement);
	Apple& operator=(const Apple& newApple);

	bool collides(const Element &e)const;
	bool collidesElements(const Node &e)const;

};

#endif /* APPLE_H_ */
