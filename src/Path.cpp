/*
 * Path.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: carlin
 */

#include "Path.h"

Path::Path()
	:	pHead(nullptr)
	,	pTail(nullptr)
	,	mLength(0)
{

}

Path::~Path(){
	Node* pCurrent = pHead;
	while(pCurrent != nullptr){
		pHead = pCurrent;
		pCurrent = pCurrent->pNext;
		delete pHead;
	}
}

void Path::Add(const vec2& position, const Renderable& type){
	if(pHead == nullptr){
		assert(pTail == nullptr);
		pHead = new Node(position, type, nullptr);
		pTail = pHead;
	}else{
		assert(pTail != nullptr);
		pTail->pNext = new Node(position, type, pTail->pNext);
		pTail = pTail->pNext;
	}
}
