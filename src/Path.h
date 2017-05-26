/*
 * Path.h
 *
 *  Created on: Dec 14, 2015
 *      Author: carlin
 */

#ifndef PATH_H_
#define PATH_H_

#include "Global.h"

class Path {
private:
	Node* pHead;
	Node* pTail;
	unsigned mLength;

public:
	Path();
	~Path();

	const Node& Head()			const{ 	return (*pHead);	};
	const unsigned& Length()	const{	return (mLength);	};

	void Add(const vec2& position, const Renderable& type);
	bool Collides(const Element& e)const;
	bool CollidesElements(const Node& n)const;

};

#endif /* PATH_H_ */
