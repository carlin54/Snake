/*
 * Walls.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: carlin
 */

#include "Walls.h"

Walls::Walls(vec2 top_left, vec2 bottom_right):
	pHead(nullptr){

    add(top_left, Renderable::WallTopLeft);
    add(vec2(bottom_right.x, top_left.y), Renderable::WallTopRight);
    add(vec2(top_left.x, bottom_right.y), Renderable::WallBottomLeft);
    add(bottom_right, Renderable::WallBottomRight);

	auto switch_max = [&](auto& max, auto& min){
		if(max < min)
			std::swap(max, min);
	};

	switch_max(top_left.x, bottom_right.x);
	switch_max(top_left.y, bottom_right.y);

	unsigned width = top_left.x - bottom_right.x;
	unsigned height = top_left.y - bottom_right.y;
	counter i;

	for(i = bottom_right.x+1; i <= width-1; i++)
        add(vec2(i, top_left.y), Renderable::WallHorizontal),	// |	|
                add(vec2(i, bottom_right.y), Renderable::WallHorizontal);	// |	|

	for(i = bottom_right.y+1; i <= height-1; i++)
        add(vec2(top_left.x, i), Renderable::WallVertical),		// ------
                add(vec2(bottom_right.x, i), Renderable::WallVertical);		// ------

}

void Walls::add(vec2 position, Renderable type) {
	mLength++;
	pHead = new Node(position, type, pHead);
}

bool Walls::collides(const Element *e)const{
	Node* pCurrent = pHead;
	while(pCurrent != nullptr){
		if(pCurrent->mPosition == e->mPosition){
			return (true);
		}
		pCurrent = pCurrent->pNext;
	}
	return (false);
}
bool Walls::collidesElements(const Node *n)const{
	const Node* pCurrentWallElement = pHead;
	const Node* pCurrentElement;
	while(pCurrentWallElement != nullptr){
		pCurrentElement = n;
		while(pCurrentElement != nullptr){
			if(pCurrentWallElement->mPosition == pCurrentElement->mPosition){
				return (true);
			}
			pCurrentElement = pCurrentElement->pNext;
		}
		pCurrentWallElement = pCurrentWallElement->pNext;
	}
	return (false);
}

Walls::~Walls() {
	Node* pCurrent = pHead;
	while(pCurrent != nullptr){
		pHead = pCurrent;
		pCurrent = pCurrent->pNext;
		delete pHead;
	}
}

