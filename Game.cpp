/*
 * Game.cpp
 *
 *  Created on: Nov 20, 2015
 *      Author: carlin
 */

#include <term.h>
#include <vector>

#include "Game.h"

void Game::Vertex(Element* e){}
void Game::Fragment(Element* e)throw(Discard){}

void Game::start(){
    initialize();
    startInputThread();
    startLoopThread();
}

void Game::initialize(){

    terminate();

    mSnake = new Snake(vec2(mWidth/2, mHeight/2), 1);
    mWalls = new Walls(vec2(0, mHeight-1), vec2(mWidth-1, 0));


	mObstacles.push_back(mSnake->getHead());
	mObstacles.push_back(mWalls->getHead());

    mApple = new Apple(newApple());
    generatePathToApple();

    //clear();
	//refresh();

    // create a buffer for the window
    pBuffer = (Renderable**)malloc(sizeof(Renderable**) * mHeight);
    for(unsigned i = 0; i < mHeight; i++)
        pBuffer[i] = (Renderable*)malloc(sizeof(Renderable*) * mWidth);

    // empty that buffer
    clearBuffer(0, 0, mWidth, mHeight, Renderable::Empty);

    mState = State::Loaded;
}

void Game::startInputThread() {
    this->mThreadInput = new std::thread(Input, this);
}

void Game::Input(Game* game){
    std::cout << "Input thread started!" << std::endl;
	while(game->mState != Game::Exit){
		int ch = getch();
		switch(ch){
			case KEY_UP:
                game->mInputDirection = Direction::UP;
				break;
			case KEY_DOWN:
                game->mInputDirection = Direction::DOWN;
				break;
			case KEY_LEFT:
                game->mInputDirection = Direction::LEFT;
				break;
			case KEY_RIGHT:
                game->mInputDirection = Direction::RIGHT;
				break;
			case (chtype)(char)27:
				game->mState = Game::Exit;
		}
	}
}

void Game::startLoopThread() {
    this->mThreadInput = new std::thread(Loop, this);
}

void Game::Loop(Game* game){
    std::cout << "Loop thread started!" << std::endl;
    while(game->mState != State::Exit){
        switch(game->mState){
            case State::Empty:
                std::cout << "State::Empty" << std::endl;
                game->initialize();
                break;
            case State::Running:
                std::cout << "State::Running" << std::endl;
                game->run();
                break;
            case State::Won:
                std::cout << "State::Won" << std::endl;
                game->win();
                break;
            case State::Lost:
                std::cout << "State::Lost" << std::endl;
                game->lose();
                break;
            case State::Loaded:
                std::cout << "State::Loaded" << std::endl;
                game->mState = State::Running;
                break;
        }
    }
}

void Game::terminate(){
    // free buffer
    if(mWindow != nullptr) {
        delwin(mWindow);
        mWindow = nullptr;
    }

    if(pBuffer != nullptr){
        for(unsigned i = 0; i < mHeight; i++){
            if(pBuffer[i] != nullptr){
                free(pBuffer[i]);
            }
        }
        free(pBuffer);
        pBuffer = nullptr;
    }

    // clear drawBuffer targets
    if(mSnake != nullptr) delete mSnake, mSnake = nullptr;
    if(mWalls != nullptr) delete mWalls, mWalls = nullptr;
    if(mApple != nullptr) delete mApple, mApple = nullptr;

	mObstacles.erase(mObstacles.begin(), mObstacles.end());

    mState = State::Empty;
}

void Game::end(){

    if(mThreadLoop != nullptr) {
        mThreadLoop->join();
        delete mThreadLoop;
        mThreadLoop = nullptr;
    }

    if(mThreadInput != nullptr) {
        mThreadInput->join();
        delete mThreadInput;
        mThreadInput = nullptr;
    }

    terminate();
}

bool Game::isSafe(const vec2& position)const{
	Element e(position, Renderable::Empty);
	return (inBounds(position) == true	 		&&
            mSnake->collidesWith(&e) == false 	&&
            mWalls->collides(&e) == false
			);
}
void Game::generatePathToApple(){
    if(mPathToApple != nullptr){
        delete mPathToApple;
        mPathToApple = nullptr;
    }


    vec2 start = mSnake->getHead()->mPosition;
    vec2 end = mApple->mPosition;
    mPathToApple = new Path(mObstacles, Renderable::PathApple);
    bool check = mPathToApple->generatePath(start, end);
    if(!check){
        std::cout << "NO PATH FOUND!" << std::endl;
    }
}
void Game::run(){

	assert(mState == State::Running);
	// clear screen
	clearBuffer(0, 0, mWidth, mHeight, Renderable::Empty);

    if(mAI){
        if(mPathToApple->hasPath()){
            mInputDirection = mPathToApple->getDirection();
            mPathToApple->pop();
        }


        /*if(mPathToTail != nullptr){
            vec2 start = mSnake->getHead()->mPosition;
            vec2 end = mSnake->getTail()->mPosition;

            mPathToTail = new Path(obstacles, Renderable::PathTail);
            mPathToTail->generatePath(start, end);
        }*/
    }

	// physics
    mSnake->step(mInputDirection);

	// colliders
	if(mSnake->collidesWith(mApple)) {
        mSnake->eatApple();
        *mApple = newApple();
        generatePathToApple();
    }

	if(mSnake->convexes()) {
        mState = State::Lost;
    }

	if(mWalls->collidesElements(mSnake->getHead())) {
        mState = State::Lost;
    }



	// push elements

    writeElementToBuffer(mPathToApple->getHead(), Vertex, Fragment);
    writeElementToBuffer(mWalls->getHead(), Vertex, Fragment);
	writeElementToBuffer(mSnake->getHead(), Vertex, Fragment);
	writeToBuffer(mApple, Vertex, Fragment);



	//writeElementToBuffer(mDirectPath->getHead(), 		Vertex, Fragment);
	//writeElementToBuffer(mCollisionPath->getHead(), 	Vertex, Fragment);
	//writeElementToBuffer(mAIPath->getHead(), 			Vertex, Fragment);


	// drawBuffer elements
	drawBuffer();

	sleep();

}
void Game::win(){
	assert(mState == State::Won);
	clearBuffer(0, 0, mWidth, mHeight, Renderable::Apple);
	drawBuffer();
	sleep();
	mState = State::Exit;
}
void Game::lose(){
	assert(mState == State::Lost);
	clearBuffer(0, 0, mWidth, mHeight, Renderable::Apple);
	drawBuffer();
	sleep();
    terminate();
}

Game::Game(unsigned x, unsigned y, unsigned width, unsigned height, unsigned refresh, bool ai) :
	mWidth(width),	mHeight(height),
	mX(x), mY(y),
	mRefresh(refresh),
	mState(State::Empty),
	mSnake(nullptr),
	mWalls(nullptr),
	mApple(nullptr),
	pBuffer(nullptr),
	mThreadLoop(nullptr),
	mExitLoop(false),
    mInputDirection(Direction::RIGHT),
    mAI(ai),
    mPathToApple(nullptr),
    mPathToTail(nullptr)
{

    //mWindow = initscr();
    //raw();
    //keypad(stdscr, TRUE);
    //noecho();
}

Game::~Game(){
    mState = State::Exit;
	end();
}

void Game::drawBuffer(){
    //wclear(mWindow);
    std::cout << std::endl;
	for(counter i = 0; i < mHeight; i++){
		for(counter j = 0; j < mWidth; j++){
            std::cout << (char)pBuffer[i][j];
            //        mvwaddch(mWindow, i, j, (chtype)pBuffer[mHeight-i-1][j]);
		}
        std::cout << std::endl;
	}
    // wrefresh(mWindow);
}

void Game::clearBuffer(unsigned startx, unsigned starty, unsigned endx, unsigned endy, Renderable clear){
	assert(startx < endx);
	assert(starty < endy);
	for(counter i = starty; i < endy; i++){
		for(counter j = startx; j < endx; j++){
			if(inBounds(j,i)){
				pBuffer[i][j] = clear;
			}
		}
	}
}
bool Game::canSnakeFit(){
	return ((mWidth * mHeight) - *mWalls->getLength() > *mSnake->getLength());
}
bool Game::hasSpaceForApple(){
	assert(mSnake != nullptr);
	assert(mWalls != nullptr);
	return ((mWidth * mHeight) > (*mSnake->getLength() + *mWalls->getLength()));
}

void Game::sleep(){
	usleep(mRefresh * 1000);
}
Apple Game::newApple(){
	assert(hasSpaceForApple());
	Apple e(vec2(0,0));
    std::vector<vec2> possible_spawn_locations;

    for(int i = 0; i < mWidth; i++){
        for(int j = 0; j < mHeight; j++){
            e.mPosition = vec2(i,j);
            if(!mSnake->collidesWith(&e) && (!mWalls->collides(&e))){
                possible_spawn_locations.emplace_back(vec2(i,j));
            }
        }
    }
    long unsigned int size = possible_spawn_locations.size();
    int loc_index = rand() % possible_spawn_locations.size();
    e.mPosition = possible_spawn_locations[loc_index];
    std::cout << "new position " << e.mPosition.x << " " << e.mPosition.y << endl;
	return (e);
}
void Game::clip(const Element* e)const throw(Discard){
	if(!inBounds(e->mPosition)) throw Discard();
}

inline bool Game::inBounds(const int& x, const int& y) const {
	return ((x >= 0) && (x < int(mWidth)) &&
			(y >= 0) && (y < int(mHeight)));
}
inline bool Game::inBounds(const vec2& isIn) const {
	return ((isIn.x >= 0) && (isIn.x < int(mWidth)) &&
			(isIn.y >= 0) && (isIn.y < int(mHeight)));
}

void Game::writeToBuffer(const Element* E, void(*vert)(Element*), void(*frag)(Element*)){

	Element e(E->mPosition, E->mType);
	vert(&e);
	try{
		clip(&e);
		frag(&e);
		pBuffer[e.mPosition.y][e.mPosition.x] = e.mType;
	}catch(Discard& d){}

}
void Game::writeElementToBuffer(const Node* head, void(*vert)(Element*), void(*frag)(Element*)throw(Discard)){
		const Node* E = head;
		Element e;
		while(E != nullptr){
			e.mPosition = E->mPosition;
			e.mType = E->mType;

			vert(&e);
			try{
				clip(&e);
				frag(&e);
				pBuffer[e.mPosition.y][e.mPosition.x] = e.mType;
			}catch(Discard& d){}
			E = E->pNext;
		}
}

