/*
 * Main.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: Carlin Connell
 */

#include <iostream>
#include "Game.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    int x = 0;
    int y = 0;
    int width = 25;
    int height = 25;
    int step = 500;

	Game game(x, y, width, height, step, true);
    game.start();
    game.end();

	return (0);
}




