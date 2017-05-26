/*
 * Snake.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: carlin
 */

#include "Snake.h"

bool Snake::Collides(Element& e)const{
	Node* pCurrent = pHead;
	while(pCurrent != nullptr){
		if(pCurrent->mPosition == e.mPosition){
			return (true);
		}
		pCurrent = pCurrent->pNext;
	}
	return (false);
}
bool Snake::Convexes()const{
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
	return (false);
};
void Snake::Step(const Direction& DIR){

	auto move_head = [&](Node* pmove){

		if		(mDirection.MASK & DIR.KEY)		mDirection = DIR;
		if		(mDirection == Direction::UP)	pmove->mPosition.y +=  1;
		else if	(mDirection == Direction::DOWN)	pmove->mPosition.y += -1;
		else if	(mDirection == Direction::LEFT) pmove->mPosition.x += -1;
		else if	(mDirection == Direction::RIGHT)pmove->mPosition.x +=  1;

	};

	auto shift_tail = [&](Node* ptail){ 						//assert(ptail->mType == SnakeTail || ptail->mType == SnakeAppleTail);

		if(ptail->mType == Renderable::SnakeAppleBody){
			ptail->mType = Renderable::SnakeBody;
			ptail->pNext = new Node(ptail->mPosition, Renderable::SnakeTail, ptail->pNext);
		}else{
			ptail->mType = Renderable::SnakeTail;
		}

	};

	std::function<void (Node*)> shift_body = [&](Node* pbody){ //assert(pbody->mType == SnakeBody || pbody->mType == SnakeAppleBody);

		if(pbody->pNext == nullptr){
			shift_tail(pbody);
		}else{
			shift_body(pbody->pNext);
			if(pbody->mType == Renderable::SnakeAppleBody){
				pbody->pNext->mType = pbody->mType;
				pbody->mType = Renderable::SnakeBody;
			}
			pbody->pNext->mPosition = pbody->mPosition;
		}
	};
	auto shift_head = [&](Node* phead){ 						//assert(phead->mType == SnakeHead || phead->mType == SnakeAppleHead);

		if(phead->pNext != nullptr){
			shift_body(phead->pNext);
			if(phead->mType == Renderable::SnakeAppleHead){
				phead->pNext->mType = Renderable::SnakeAppleBody,
				phead->mType = Renderable::SnakeHead;
			}
			phead->pNext->mPosition = phead->mPosition;
		}else{
			if(phead->mType == Renderable::SnakeAppleHead){
				phead->pNext = new Node(phead->mPosition, Renderable::SnakeTail, phead->pNext);
				phead->mType = Renderable::SnakeHead;
			}else{
				phead->mType = Renderable::SnakeHead;
			}
		}
		move_head(phead);

	};


	shift_head(this->pHead);
	mDirection = DIR;

}
void Snake::EatApple(){
	mLength++;
	pHead->mType = Renderable::SnakeAppleHead;
}

Snake::Snake(vec2 position, unsigned length) :
	mLength(1), mDirection(Direction::RIGHT) {

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
	Node* pNext = pCurrent;
	while(pNext != nullptr){
		pCurrent = pNext;
		pNext = pNext->pNext;
		delete pCurrent;
	}
}
