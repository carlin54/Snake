/*
 * Walls.h
 *
 *  Created on: Nov 23, 2015
 *      Author: carlin
 */

#ifndef WALLS_H_
#define WALLS_H_

#include "Global.h"


class Walls {
	Node* pHead;
	unsigned mLength;

public:

	const Node* getHead()		const{ 	return (pHead);	};
	const unsigned* getLength()	const{	return (&mLength);	};

	Walls(vec2 top_left, vec2 bottom_right);
	~Walls();

	void add(vec2 position, Renderable type);
	bool collides(const Element* e)const;
	bool collidesElements(const Node* n)const;

};

#endif /* WALLS_H_ */
