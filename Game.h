/*
 * Game.h
 *
 *  Created on: Nov 20, 2015
 *      Author: carlin
 */

#ifndef GAME_H_
#define GAME_H_

#include "Global.h"
#include "Snake.h"
#include "Walls.h"
#include "Apple.h"
#include "Path.h"

class Game {
	enum State { Loaded, Running, Won, Lost, Exit, Empty };

private:

	Direction                   mInputDirection;
	std::thread*                mThreadInput;
    std::thread*                mThreadLoop;
    bool                        mExitLoop;
    bool                        mAI;

	WINDOW*						mWindow;
	unsigned 					mWidth;
	unsigned 					mHeight;
	unsigned 					mX;
	unsigned 					mY;
	unsigned 					mRefresh;

    State 						mState;
    Snake* 						mSnake;
    Walls* 						mWalls;
	Apple* 						mApple;
    Path*                       mPathToTail;
    Path*                       mPathToApple;
	std::vector<const Node*>	mObstacles;

	Renderable** 				pBuffer;



	void initialize();
    void startInputThread();
    static void Input(Game* game);

    void startLoopThread();
    static void Loop(Game* game);

    void terminate();


	void run();
	void win();
	void lose();

	void sleep();
	Apple newApple();
	bool hasSpaceForApple();
	bool canSnakeFit();

	void drawBuffer();
	void clearBuffer(unsigned startx, unsigned starty, unsigned endx, unsigned endy,
                     Renderable clear = Renderable::Empty);

	bool isSafe(const vec2& position)const;
	bool inBounds(const vec2& isIn)const;
    bool inBounds(const int& x, const int& y) const;

	void writeToBuffer(const Element* head, void(*vert)(Element *) = Vertex, void(*frag)(Element*) = Fragment);
	void writeElementToBuffer(const Node* head, void(*vert)(Element*) = Vertex,
                              void(*frag)(Element*)throw(Discard) = Fragment);

    static void Vertex(Element* e);
    static void Fragment(Element* e) throw(Discard);
    void clip(const Element* e)const throw(Discard);

public:

	void start();
    void end();

	Game(unsigned x, unsigned y, unsigned width, unsigned height, unsigned refresh, bool ai);
	~Game();
};

#endif /* GAME_H_ */
