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
	enum State { Running, Won, Lost, Exit, Empty };

private:

	static Direction 			InputDirection;
	static std::thread* 		ThreadInput;
	static bool 				JoinThreadInput;
	static std::vector<Game*> 	RunningGames;

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
	Renderable** 				pBuffer;
	std::thread* 				mThreadLoop;
	bool 						mExitLoop;
	bool						mIsLoaded;


	Path* 						pDirectPath;
	Path* 						pCollisionPath;
	Path* 						pAIPath;
	MultiNode<vec2>*			pGraphHead;

	Path* DirectionPath();
	void ReloadGraph();
	void PathFind();

	static void Input();
	static void Loop(Game* game);

	// clearing values
		// opens values
	void Load();
		// closes values
	void Unload();

	// opens and closes loop
		// whenever
	void StartLoop();
		// whenever
	void CloseLoop();



	void Run();
	void Win();
	void Lose();

	void Sleep();
	Apple NewApple();
	bool HasSpaceForApple();
	bool SnakeCanFit();

	void RenderBuffer();
	void ClearBuffer(unsigned startx, unsigned starty, unsigned endx, unsigned endy, Renderable clear = Renderable::Empty);

	bool isSafe(const vec2& position)const;
	bool inBounds(const vec2& isIn)const;
	bool inBounds(const int& x, const int& y) const;
	void Clip(const Element& e)const throw(Discard);

	static void Vertex(Element& e);
	static void Fragment(Element& e)throw(Discard);
	void Draw(const Element& head, void(*vert)(Element&)=Vertex, void(*frag)(Element&)=Fragment);
	void DrawElements(const Node& head, void(*vert)(Element&)=Vertex, void(*frag)(Element&)throw(Discard)=Fragment);

public:
	static void Join();

	void Start();
	void End();

	Game(unsigned x, unsigned y, unsigned width, unsigned height, unsigned refresh);
	~Game();
};

#endif /* GAME_H_ */
