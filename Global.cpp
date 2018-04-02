/*
 * Global.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: carlin
 */

#include "Global.h"


const Direction Direction::UP    	( 8, 11 );	// 0x1000, 0x1011
const Direction Direction::DOWN    	( 4, 7 );	// 0x0100, 0x0111
const Direction Direction::LEFT    	( 2, 14 );	// 0x0010, 0x1110
const Direction Direction::RIGHT	( 1, 13 ); 	// 0x0001, 0x1101

int magnitude(int x){
    return int(sqrt(x*x));
}