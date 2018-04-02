/*
 * Snake.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: carlin
 */

#include "Snake.h"

bool Snake::collidesWith(const Element* e)const{
	Node* pCurrent = pHead;
	while(pCurrent != nullptr){
		if(pCurrent->mPosition == e->mPosition){
			return (true);
		}
		pCurrent = pCurrent->pNext;
	}
	return false;
}

bool Snake::convexes()const{
	Node* pCheck = pHead;
	Node* pAgainst = nullptr;
	while(pCheck->pNext != nullptr){
		pAgainst = pCheck->pNext;
		while(pAgainst != nullptr){
			if(pCheck->mPosition == pAgainst->mPosition){
				return (true);
			}
			pAgainst = pAgainst->pNext;
		}
		pCheck = pCheck->pNext;
	}
	return false;
};

void Snake::step(const Direction& DIR){

	auto move_head = [&](Node* pMove){

		if		(mDirection.MASK & DIR.KEY)		mDirection = DIR;
		if		(mDirection == Direction::UP)	pMove->mPosition.y +=  1;
		else if	(mDirection == Direction::DOWN)	pMove->mPosition.y += -1;
		else if	(mDirection == Direction::LEFT) pMove->mPosition.x += -1;
		else if	(mDirection == Direction::RIGHT)pMove->mPosition.x +=  1;

	};

	auto shift_tail = [&](Node* pTail){ 						//assert(ptail->mType == SnakeTail || ptail->mType == SnakeAppleTail);

		if(pTail->mType == Renderable::SnakeAppleBody){
            pTail->mType = Renderable::SnakeBody;
            pTail->pNext = new Node(pTail->mPosition, Renderable::SnakeTail, pTail->pNext);
		}else{
            pTail->mType = Renderable::SnakeTail;
		}

	};

	std::function<void (Node*)> shift_body = [&](Node* pBody){ //assert(pbody->mType == SnakeBody || pbody->mType == SnakeAppleBody);

		if(pBody->pNext == nullptr){
			shift_tail(pBody);
		}else{
			shift_body(pBody->pNext);
			if(pBody->mType == Renderable::SnakeAppleBody){
                pBody->pNext->mType = pBody->mType;
                pBody->mType = Renderable::SnakeBody;
			}
            pBody->pNext->mPosition = pBody->mPosition;
		}
	};
	auto shift_head = [&](Node* pHead){ 						//assert(phead->mType == SnakeHead || phead->mType == SnakeAppleHead);

		if(pHead->pNext != nullptr){
			shift_body(pHead->pNext);
			if(pHead->mType == Renderable::SnakeAppleHead){
                pHead->pNext->mType = Renderable::SnakeAppleBody;
                pHead->mType = Renderable::SnakeHead;
			}
            pHead->pNext->mPosition = pHead->mPosition;
		}else{
			if(pHead->mType == Renderable::SnakeAppleHead){
                pHead->pNext = new Node(pHead->mPosition, Renderable::SnakeTail, pHead->pNext);
                pHead->mType = Renderable::SnakeHead;
			}else{
                pHead->mType = Renderable::SnakeHead;
			}
		}
		move_head(pHead);

	};


	shift_head(this->pHead);
	mDirection = DIR;

}

void Snake::eatApple(){
	mLength++;
	pHead->mType = Renderable::SnakeAppleHead;
}

Snake::Snake(vec2 position, unsigned length) :
	mLength(length), mDirection(Direction::RIGHT) {

	pHead = new Node(position, Renderable::SnakeHead);
	Node* pCurrent = pHead;

	for(counter i = 0; i < length; i++){ // + 2 for the extra body
		position.x--;
		pCurrent->pNext = new Node(position, Renderable::SnakeBody);
		pCurrent = pCurrent->pNext;
		mLength++;
	}

	pCurrent->mType = Renderable::SnakeTail;

}

Snake::~Snake(){
	Node* pCurrent = pHead;
	Node* pNext = pHead;
	while(pCurrent != nullptr){
		pNext = pNext->pNext;
		delete pCurrent;
		pCurrent = pNext;
	}
}
