/*
 * Global.h
 *
 *  Created on: Nov 23, 2015
 *      Author: carlin
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <thread>
#include <iostream>
#include <cstddef>
#include <assert.h>
#include <cstdlib>
#include <curses.h>
#include <thread>
#include <exception>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <functional>
#include <vector>
#include <math.h>

class Apple;
class Snake;
class Walls;

class Vector2;
class Element;
class Node;

typedef unsigned counter;
typedef float real;
typedef Vector2 vec2;

int magnitude(int x);

enum class Renderable : chtype {
	Empty		 		= ' ',
	Apple		 		= 'a',
	SnakeHead	 		= 'H',
	SnakeBody	 		= 'B',
	SnakeTail	 		= 'T',
	SnakeAppleHead	 	= 'A',
	SnakeAppleBody		= 'A',
	SnakeAppleTail		= 'o',
	WallHorizontal  	= '-',
	WallVertical  		= '|',
	WallTopLeft  		= '+',
	WallTopRight 		= '+',
	WallBottomLeft		= '+',
	WallBottomRight		= '+',
	PathApple			= '+',
    PathTail            = 'T'
};

class Direction {
private:
	Direction():
        KEY(0), MASK(0){};
	Direction(unsigned char key, unsigned char mask):
        KEY(key), MASK(mask){};

public:
	Direction(const Direction& OTHER):
    	KEY(OTHER.KEY), MASK(OTHER.MASK){};
    ~Direction()
    	{};
    Direction& operator=(const Direction& rhs){
    	KEY = rhs.KEY, MASK = rhs.MASK; return (*this);}

    inline bool operator==(const Direction& rhs)const{
    	return (((KEY == rhs.KEY) && (MASK == rhs.MASK)));}

    unsigned char KEY;
    unsigned char MASK;

    static const Direction UP;
    static const Direction DOWN;
    static const Direction LEFT;
    static const Direction RIGHT;
};

class Vector2 {
public:
	int x; int y;

	inline Vector2():
		x(0), y(0){};

	inline Vector2(int x, int y):
		x(x), y(y){};

	inline bool operator==(const Vector2& rhs)const{
		return ((this->x == rhs.x) && (this->y == rhs.y));}
	inline void operator+=(const Vector2& rhs)
			{ this->x += rhs.x, this->y += rhs.y; }
	inline void operator-=(const Vector2& rhs)
			{ this->x -= rhs.x, this->y -= rhs.y; }
	inline Vector2 operator+(const Vector2& rhs)const
			{ return (Vector2((this->x + rhs.x), (this->y + rhs.y)));}
	inline Vector2 operator-(const Vector2& rhs)const
			{ return (Vector2((this->x - rhs.x), (this->y - rhs.y)));}
};

class Element {
public:
	vec2 mPosition;
	Renderable mType;

	Element():
		mPosition(), mType(Renderable::Empty){};
	Element(vec2 position, Renderable type):
		mPosition(position), mType(type){}

};

class Node : public Element {

public:
	Node* pNext;

	inline Node(vec2 position, Renderable type, Node* pNext = nullptr):
		Element(position, type), pNext(pNext){};

    inline ~Node(){

    }

};

class Discard : public std::exception {};


#endif /* GLOBAL_H_ */
