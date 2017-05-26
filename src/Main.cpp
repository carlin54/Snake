/*
 * Main.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: carlin
 */

#include <iostream>
#include "Game.h"

int main() {

	Game G1(0,0,7,7,500);
	G1.Start();
	Game::Join();
	return (0);
}
