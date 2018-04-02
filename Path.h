//
// Created by richard on 30/03/18.
//

#ifndef SNAKES_PATH_H
#define SNAKES_PATH_H


#include "Global.h"

class Path {
private:
    Node* pHead;
    const std::vector<const Node*>& mObstacles;

public:
    Path(const std::vector<const Node*>& obstacles):
            mObstacles(obstacles)
    {

    }

    ~Path(){
        delete pHead;
    }
    const Node* getHead(){
        return pHead;
    }
    bool isClear(const vec2& check, const Node* pPath){
        const Node* pCurrent;
        for(int i = 0; i < mObstacles.size(); i++){
            pCurrent = mObstacles[i];
            while(!pCurrent){
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

    bool generatePath(const vec2& start, const vec2& end, Node* pNewHead = nullptr, Node* pCurrent = nullptr)
    {

        if(pHead != nullptr) {
            delete pHead;
            pHead = nullptr;
        }

        vec2 dir = end - start;
        vec2 pos = start;
        std::cout << "pos x : " << pos.x << " pos y : " << pos.y << std::endl;
        if(dir == vec2(0,0)) return true;

        if(!pNewHead) {
            pNewHead = new Node(start, Renderable::Path);
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

        if(mag_x > mag_y){
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
                pCurrent->pNext = new Node(start, Renderable::Path);
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
