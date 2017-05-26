/*
 * Apple.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: carlin
 */

#include "Apple.h"

bool Apple::Collides(const Element& e)const{
	return (mPosition == e.mPosition);
}

bool Apple::CollidesElements(const Node& n)const{
	const Node* pCurrent = &n;
	while(pCurrent != nullptr){
		if(mPosition == pCurrent->mPosition) return (true);
	}
	return (false);
}
Apple& Apple::operator=(const vec2& newPosition){
	this->mPosition = newPosition;
	this->mType = Renderable::Apple;
	return (*this);
};
Apple& Apple::operator=(const Element& newElement){
	this->mPosition = newElement.mPosition;
	this->mType = Renderable::Apple;
	return (*this);
};
Apple& Apple::operator=(const Apple& newApple){
	this->mPosition = newApple.mPosition;
	this->mType = Renderable::Apple;
	return (*this);
};
