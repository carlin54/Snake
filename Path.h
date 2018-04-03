//
// Created by richard on 30/03/18.
//

#ifndef SNAKES_PATH_H
#define SNAKES_PATH_H


#include "Global.h"

#include <exception>
using namespace std;

class PathException : public exception {

public:

    enum Error{
        CantGetDirection
    };

    PathException(Error e):
            mError(e)
    {

    }


    virtual const char* what() const throw()
    {
        switch(mError){
            case CantGetDirection:
                return "Cant find a direction!";

        }
    }
private:
    Error mError;

};

class Path {
private:
    Node* pHead;
    const std::vector<const Node*>& mObstacles;
    Renderable mRender;
    void destroy(){
        Node* pCurrent = pHead;
        Node* pNext = pHead;
        while(pCurrent != nullptr){
            pNext = pNext->pNext;
            delete pCurrent;
            pCurrent = pNext;
        }
        pHead = nullptr;
    }

public:
    Path(const std::vector<const Node*>& obstacles, const Renderable& render):
            pHead(nullptr),
            mObstacles(obstacles),
            mRender(render)
    {

    }

    ~Path(){
        destroy();
    }
    const Node* getHead(){
        return pHead;
    }

    bool isClear(const vec2& check, const Node* pPath){
        const Node* pCurrent;
        for(int i = 0; i < mObstacles.size(); i++){
            pCurrent = mObstacles[i];

            while(pCurrent != nullptr){
                if(pCurrent->mPosition == check){
                    return false;
                }

                pCurrent = pCurrent->pNext;
            }
        }
        pCurrent = pPath;
        while(!pCurrent){
            if(pCurrent->mPosition == check){
                return false;
            }
            pCurrent = pCurrent->pNext;
        }
        return true;
    }
    void pop(){
        Node* del = pHead;
        pHead = pHead->pNext;
        delete del;
    }
    bool hasPath(){
        if(pHead == nullptr) return false;
        if(pHead->pNext == nullptr) return false;
        return true;
    }
    Direction getDirection() {
        if(pHead == nullptr || pHead->pNext == nullptr){
            throw PathException(PathException::CantGetDirection);

        }
        vec2 start = pHead->mPosition;
        vec2 end = pHead->pNext->mPosition;
        vec2 dir = end - start;

        if(dir.x == 1)      return Direction::RIGHT;
        else if(dir.x ==-1) return Direction::LEFT;
        else if(dir.y == 1) return Direction::UP;
        else if(dir.y ==-1) return Direction::DOWN;

    }

    bool generatePath(const vec2& start, const vec2& end, Node* pNewHead = nullptr, Node* pCurrent = nullptr){

        destroy();

        vec2 dir = end - start;
        vec2 pos = start;
        std::cout << "pos x : " << pos.x << " pos y : " << pos.y << std::endl;
        if(dir == vec2(0,0)) return true;

        if(!pNewHead) {
            pNewHead = new Node(start, mRender);
            pCurrent = pNewHead;
        }

        int mag_x = magnitude(dir.x);
        int mag_y = magnitude(dir.y);

        vec2 up   ( 0, 1);
        vec2 down ( 0,-1);
        vec2 left (-1, 0);
        vec2 right( 1, 0);

        vec2 C1;
        vec2 C2;
        vec2 C3;
        vec2 C4;

        if(mag_x >= mag_y){
            C1 = (dir.x > 0) ? right    :   left;
            C2 = (dir.y > 0) ? up       :   down;
            C3 = (dir.x > 0) ? left     :   right;
            C4 = (dir.y > 0) ? down     :   up;
        }else{
            C1 = (dir.y > 0) ? up       :   down;
            C2 = (dir.x > 0) ? right    :   left;
            C3 = (dir.y > 0) ? down     :   up;
            C4 = (dir.x > 0) ? left     :   right;
        }

        vec2 move[4] = {C1, C2, C3, C4};

        for(int i = 0; i < 4; i++){
            vec2 check = move[i] + pos;
            if(isClear(check, pNewHead)){
                pCurrent->pNext = new Node(check, mRender);
                if(generatePath(check, end, pNewHead, pCurrent->pNext)){
                    if(pCurrent == pNewHead){
                        pHead = pNewHead;
                    }
                    return true;
                }else{
                    delete pCurrent->pNext;
                }
            }
        }
        delete pCurrent;
        return false;




        //[3, 4]

            //[0, 1] <- cant go snake
            //[1, 0]
            //[0, 1]
            //[1, 0]

    }


};


#endif //SNAKES_PATH_H
