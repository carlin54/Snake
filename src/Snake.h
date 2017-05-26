/*
 * Snake.h
 *
 *  Created on: Nov 23, 2015
 *      Author: carlin
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include "Global.h"

struct Snake {

private:
	Node* pHead;
	unsigned mLength;
	Direction mDirection;

public:

	inline const Node& 		Head()			const{ 	return (*pHead); 		};
	inline const unsigned& 	Length()		const{ 	return (mLength); 		};
	inline const Direction& StepDirection()	const{ 	return (mDirection); 	};

	void Step(const Direction& DIR);

	void EatApple();

	bool Convexes()const;
	bool Collides(Element& e)const;

	static void Vertex(const Element& e);
	static void Fragment(const Element& e)throw(Discard&);

	Snake(vec2 position, unsigned extraLength = 0);
	~Snake();

};

#endif /* SNAKE_H_ */
