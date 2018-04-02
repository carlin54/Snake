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

	inline const Node* 		getHead()			const{ 	return (pHead); 		};
	inline const unsigned* 	getLength()			const{ 	return (&mLength); 		};
	inline const Direction* getStepDirection()	const{ 	return (&mDirection); 	};

	void step(const Direction &DIR);

	void eatApple();

	bool convexes()const;
	bool collidesWith(const Element* e)const;

	static void Vertex(const Element* e);
	static void Fragment(const Element* e)throw(Discard&);

	Snake(vec2 position, unsigned extraLength = 0);
	~Snake();

};

#endif /* SNAKE_H_ */
