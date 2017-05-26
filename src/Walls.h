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

	const Node& Head()			const{ 	return (*pHead);	};
	const unsigned& Length()	const{	return (mLength);	};

	Walls(vec2 top_left, vec2 bottom_right);
	~Walls();

	void Add(vec2 position, Renderable type);
	bool Collides(const Element& e)const;
	bool CollidesElements(const Node& n)const;

};

#endif /* WALLS_H_ */
