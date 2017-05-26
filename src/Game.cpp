/*
 * Game.cpp
 *
 *  Created on: Nov 20, 2015
 *      Author: carlin
 */

#include "Game.h"

void Game::Vertex(Element& e){}
void Game::Fragment(Element& e)throw(Discard){}

void Game::Input(){

	while(!JoinThreadInput){
		int ch = getch();
		switch(ch){
			case KEY_UP:
				InputDirection = Direction::UP;
				break;
			case KEY_DOWN:
				InputDirection = Direction::DOWN;
				break;
			case KEY_LEFT:
				InputDirection = Direction::LEFT;
				break;
			case KEY_RIGHT:
				InputDirection = Direction::RIGHT;
				break;
			case (chtype)(char)27:
				for(counter i = 0; i < RunningGames.size(); i++){
					RunningGames[i]->End();
				}
		}
	}
}

void Game::StartLoop(){
	assert(mThreadLoop == nullptr);
	if(ThreadInput == nullptr){
		assert(RunningGames.size() == 0);
		JoinThreadInput = false;
		ThreadInput = new std::thread(Input);

	}

	mExitLoop = false;
	mState = State::Running;
	mThreadLoop = new std::thread(Loop, this);


	RunningGames.push_back(this);
}
void Game::CloseLoop(){
	if(mState != State::Empty){
		assert(mThreadLoop != nullptr);
		assert(ThreadInput != nullptr);
		mExitLoop = true;
		mThreadLoop->join();
		delete mThreadLoop;
		mThreadLoop = nullptr;

		for(auto it = RunningGames.begin(); it != RunningGames.end(); it++){
			if(*it == this){
				RunningGames.erase(it);
				break;
			}
		}
		if(RunningGames.size() == 0){
			JoinThreadInput = true;
			ThreadInput->join();
			delete ThreadInput;
			ThreadInput = nullptr;
		}
	}
}
void Game::Loop(Game* game){
	assert(game->mState != State::Empty);
	while(!game->mExitLoop){
		switch(game->mState){
		case State::Empty:
			game->Load();
			break;
		case State::Running:
			game->Run();
			break;
		case State::Won:
			game->Win();
			break;
		case State::Lost:
			game->Lose();
			break;
		case State::Exit:

			break;
		}
	}
}
void Game::Join(){
	bool isGameRunning = true;
	while(isGameRunning){
		isGameRunning = false;
		if(RunningGames.size() == 0){
			isGameRunning = false;
		}else{
			isGameRunning = true;
			for(counter i = 0; i < RunningGames.size(); i++){
				if(RunningGames[i]->mState == State::Exit){
					RunningGames[i]->End();
				}
			}
		}
	}

}

void Game::Load(){
	if(mState == State::Empty){

		assert(pDirectPath == nullptr),
				pDirectPath 	= new Path();
		assert(pCollisionPath == nullptr),
				pCollisionPath 	= new Path();
		assert(pAIPath == nullptr),
				pAIPath 		= new Path();

		// make render targets
		assert(mSnake == nullptr),
				mSnake = new Snake(vec2(mWidth/2, mHeight/2), 1);
		assert(mWalls == nullptr),
				mWalls = new Walls(vec2(0, mHeight-1), vec2(mWidth-1, 0));
		assert(mApple == nullptr),
				mApple = new Apple(NewApple());

		// set up my window
		mWindow = newwin(mHeight, mWidth, mX, mY);
		wclear(mWindow);
		wrefresh(mWindow);

		// create a buffer for the window
		pBuffer = (Renderable**)malloc(sizeof(Renderable**) * mHeight);
		for(unsigned i = 0; i < mHeight; i++)
			pBuffer[i] = (Renderable*)malloc(sizeof(Renderable*) * mWidth);

		// Check that it works
		assert(SnakeCanFit());
		assert(HasSpaceForApple());

		// empty that buffer
		ClearBuffer(0, 0, mWidth, mHeight, Renderable::Empty);
	}
}
void Game::Unload(){
	if(mState != State::Empty){
		assert(mState != State::Empty);

		// free buffer
		delwin(mWindow);
		assert(pBuffer != nullptr);
		if(pBuffer != nullptr){
			for(unsigned i = 0; i < mHeight; i++){
				if(pBuffer[i] != nullptr){
					assert(pBuffer[i] != nullptr), free(pBuffer[i]);
				}
			}
			free(pBuffer);
		}

		// clear render targets
		assert(mSnake != nullptr), delete mSnake;
		assert(mWalls != nullptr), delete mWalls;
		assert(mApple != nullptr), delete mApple;


		if(pDirectPath != nullptr) delete pDirectPath;
		if(pCollisionPath != nullptr) delete pCollisionPath;
		if(pAIPath != nullptr) delete pAIPath;

		mState = State::Empty;
	}
}

void Game::Start(){
	Load();
	StartLoop();
}
void Game::End(){
	CloseLoop();
	Unload();
}
bool Game::isSafe(const vec2& position)const{
	Element e(position, Renderable::Empty);
	return (inBounds(position) == true	 &&
			mSnake->Collides(e) == false &&
			mWalls->Collides(e) == false
			);
}
Path* Game::DirectionPath(){
	vec2 from = mSnake->Head().mPosition;
	vec2 to = mApple->mPosition;
	Path* pathTo = new Path();
	while(true){
		if(from == to) break;
		if(from.x > to.x) from.x--;
		if(from.x < to.x) from.x++;

		if(from.y > to.y) from.y--;
		if(from.y < to.y) from.y++;
		if(from == to) break;
		pathTo->Add(from, Renderable::WallTopRight);
	}
	return (pathTo);
}
void Game::ReloadGraph(){

	std::function<void(MultiNode<vec2>*)> DeleteGraph = [&](MultiNode<vec2>* destroy){

		if(destroy->pUp 	!= nullptr){
			destroy->pUp->pDown = nullptr;
			DeleteGraph(destroy->pUp);
		}
		if(destroy->pDown 	!= nullptr){
			destroy->pDown->pUp = nullptr;
			DeleteGraph(destroy->pDown);
		}
		if(destroy->pLeft 	!= nullptr){
			destroy->pLeft->pRight = nullptr;
			DeleteGraph(destroy->pLeft);
		}
		if(destroy->pRight 	!= nullptr){
			destroy->pRight->pLeft = nullptr;
			DeleteGraph(destroy->pRight);
		}

		delete destroy;
	};

	std::function<MultiNode<vec2>* (const vec2&)> MakeGraph =
			[&](const vec2& start) {

		std::function<void(MultiNode<vec2>*, const Direction&)> MakeNode =
				[&](MultiNode<vec2>* pParent, const Direction& nodeDirection) {

			static const vec2 up	( 0, 1);	static const vec2 down ( 0,-1);
			static const vec2 left	(-1, 0);	static const vec2 right( 1, 0);

			MultiNode<vec2>* node;
			if(nodeDirection == Direction::UP){
				assert(pParent->pUp == nullptr);
				node = new MultiNode<vec2>(up + pParent->value);
				pParent->pUp = node;
				node->pDown = pParent;
			}
			else if(nodeDirection == Direction::DOWN){
				assert(pParent->pDown == nullptr);
				node = new MultiNode<vec2>(down + pParent->value);
				pParent->pDown = node;
				node->pUp = pParent;
			}
			else if(nodeDirection == Direction::LEFT){
				assert(pParent->pLeft == nullptr);
				node = new MultiNode<vec2>(left + pParent->value);
				pParent->pLeft = node;
				node->pRight = pParent;
			}
			else if(nodeDirection == Direction::RIGHT){
				assert(pParent->pRight == nullptr);
				node = new MultiNode<vec2>(right + pParent->value);
				pParent->pRight = node;
				node->pLeft = pParent;
			}


			if(node->pUp 	== nullptr && isSafe(node->value + up))
				MakeNode(node, Direction::UP);
			if(node->pDown 	== nullptr && isSafe(node->value + down))
				MakeNode(node, Direction::DOWN);
			if(node->pLeft 	== nullptr && isSafe(node->value + left))
				MakeNode(node, Direction::LEFT);
			if(node->pRight == nullptr && isSafe(node->value + right))
				MakeNode(node, Direction::RIGHT);

		};

		MultiNode<vec2>* pHead = new MultiNode<vec2>(start);
		MakeNode(pHead, Direction::UP);
		MakeNode(pHead, Direction::DOWN);
		MakeNode(pHead, Direction::LEFT);
		MakeNode(pHead, Direction::RIGHT);
		return (pHead);
	};
	if(pGraphHead != nullptr)	DeleteGraph(pGraphHead);
	pGraphHead = MakeGraph(mSnake->Head().mPosition);

}
void Game::PathFind(){
	if(pDirectPath 		!= nullptr) 	delete pDirectPath;
	if(pCollisionPath 	!= nullptr) 	delete pCollisionPath;
	if(pAIPath 			!= nullptr) 	delete pAIPath;

	ReloadGraph();

	pDirectPath 		= DirectionPath();


}

void Game::Run(){
	assert(mState == State::Running);

	// clear screen
	ClearBuffer(0, 0, mWidth, mHeight, Renderable::Empty);

	// physics
	mSnake->Step(InputDirection);

	// colliders
	if(mSnake->Collides(*mApple))
		mSnake->EatApple(),
		*mApple = NewApple();

	if(mSnake->Convexes())
		mState = State::Lost;

	if(mWalls->CollidesElements(mSnake->Head()))
		mState = State::Lost;

	// push elements
	DrawElements(mWalls->Head(), Vertex, Fragment);
	DrawElements(mSnake->Head(), Vertex, Fragment);
	Draw(*mApple, Vertex, Fragment);

	PathFind();
	//DrawElements(mDirectPath->Head(), 		Vertex, Fragment);
	//DrawElements(mCollisionPath->Head(), 	Vertex, Fragment);
	//DrawElements(mAIPath->Head(), 			Vertex, Fragment);


	// render elements
	RenderBuffer();

	Sleep();

}
void Game::Win(){
	assert(mState == State::Won);
	ClearBuffer(0, 0, mWidth, mHeight, Renderable::Apple);
	RenderBuffer();
	Sleep();
	mState = State::Exit;
}
void Game::Lose(){
	assert(mState == State::Lost);
	ClearBuffer(0, 0, mWidth, mHeight, Renderable::Apple);
	RenderBuffer();
	Sleep();
	mState = State::Exit;
}

Game::Game(unsigned x, unsigned y, unsigned width, unsigned height, unsigned refresh) :
	mWindow(nullptr),
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
	mIsLoaded(false),
	pGraphHead(nullptr),
	pDirectPath(nullptr),
	pCollisionPath(nullptr),
	pAIPath(nullptr)
{

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

}
Game::~Game(){
	End();
}

void Game::RenderBuffer(){
	wclear(mWindow);
	for(counter i = 0; i < mHeight; i++){
		for(counter j = 0; j < mWidth; j++){
			mvwaddch(mWindow, i, j, (chtype)pBuffer[mHeight-i-1][j]);
		}
	}
	wrefresh(mWindow);
}
void Game::ClearBuffer(unsigned startx, unsigned starty, unsigned endx, unsigned endy, Renderable clear){
	assert(startx < endx);
	assert(starty < endy);
	for(counter i = starty; i < endy; i++){
		for(counter j = startx; j < endx; j++){
			if(inBounds(j,i)){
				pBuffer[i][j] = clear;
			}else{
				assert(false);
			}
		}
	}
}
bool Game::SnakeCanFit(){
	return ((mWidth * mHeight) - mWalls->Length() > mSnake->Length());
}
bool Game::HasSpaceForApple(){
	assert(mSnake != nullptr);
	assert(mWalls != nullptr);
	return ((mWidth * mHeight) > (mSnake->Length() + mWalls->Length()));
}
void Game::Sleep(){
	usleep(mRefresh * 1000);
}
Apple Game::NewApple(){
	assert(HasSpaceForApple());
	Apple e(vec2(0, 0));
	while(true){
		e.mPosition.x = rand() % mWidth		-	1;
		e.mPosition.y = rand() % mHeight	-	1;
		if((!mSnake->Collides(e)) && (!mWalls->Collides(e))) return (e);
	}
	return (e);
}
void Game::Clip(const Element& e)const throw(Discard){
	if(!inBounds(e.mPosition)) throw Discard();
}
inline bool Game::inBounds(const int& x, const int& y) const {
	return ((x >= 0) && (x < int(mWidth)) &&
			(y >= 0) && (y < int(mHeight)));
}
inline bool Game::inBounds(const vec2& isIn) const {
	return ((isIn.x >= 0) && (isIn.x < int(mWidth)) &&
			(isIn.y >= 0) && (isIn.y < int(mHeight)));
}
void Game::Draw(const Element& E, void(*vert)(Element&), void(*frag)(Element&)){

	Element e(E.mPosition, E.mType);
	vert(e);
	try{
		Clip(e);
		frag(e);
		pBuffer[e.mPosition.y][e.mPosition.x] = e.mType;
	}catch(Discard& d){}

}
void Game::DrawElements(const Node& head, void(*vert)(Element&), void(*frag)(Element&)throw(Discard)){
		const Node* E = &head;
		Element e;
		while(E != nullptr){
			e.mPosition = E->mPosition;
			e.mType = E->mType;

			vert(e);
			try{
				Clip(e);
				frag(e);
				pBuffer[e.mPosition.y][e.mPosition.x] = e.mType;
			}catch(Discard& d){}
			E = E->pNext;
		}
}

Direction Game::InputDirection = Direction::RIGHT;
std::vector<Game*> Game::RunningGames;
std::thread* Game::ThreadInput = nullptr;
bool Game::JoinThreadInput = false;
