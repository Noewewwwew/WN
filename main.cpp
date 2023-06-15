#include <ncurses/ncurses.h>
#include <iostream>
#include <chrono>

#include "SnakeGame.h"


using namespace std;
using namespace std::chrono;

auto getTime() {
    return steady_clock::now();
}

auto milliInterval(const time_point<steady_clock>& start) {
    return duration_cast<milliseconds>(getTime() - start).count();
}

int main() {
    SnakeGame* game = new SnakeGame();

    game->init();
    char c;
    
    time_point startTime = getTime(), portalTime = getTime(), itemTime = getTime();
    time_point changeTime = getTime();
    bool flag = false;

    game->createItems();
    game->draw();

    while(game->isGaming()){
        if(!game->wall.isUsed() && game->isMissionClear()){
            if(!flag) {
                flag = true;
                changeTime = getTime();
            }
            
            auto calcTime = milliInterval(changeTime);
            if(calcTime < 3000){
                char msg[50];
                snprintf(msg, sizeof(msg), "change %d stage to %d stage after %lld sec...", game->currStage, game->currStage + 1, 3LL - (calcTime / 1000));
                game->changeNoticeMessage(msg);
            } else {
                flag = false;
                startTime = portalTime = itemTime = getTime();
                game->changeMap();

                game->removeExpiredItems();
                game->createItems();

                game->changeNoticeMessage("start new stage!");

                game->update();
            }
        }

        switch (getch()){
        case 'w':
            game->setDirection(SNAKE_HEAD_DIRECTION::UP);
            break;
        
        case 'a':
            game->setDirection(SNAKE_HEAD_DIRECTION::LEFT);
            break;
        
        case 's':
            game->setDirection(SNAKE_HEAD_DIRECTION::DOWN);
            break;
        
        case 'd':
            game->setDirection(SNAKE_HEAD_DIRECTION::RIGHT);
            break;

        default: break;
        }

        if(!game->wall.isUsed() && milliInterval(portalTime) >= PORTAL_DURATION){
            game->changePortal();
            portalTime = getTime();
        }
        
        if(milliInterval(itemTime) >= ITEM_DURATION){
            game->removeExpiredItems();
            game->createItems();
            itemTime = getTime();
        }

        if(milliInterval(startTime) >= UPDATE_DURATION){
            game->update();
            startTime = getTime();
        }
    }

    if(game->isLose()) cout << "You Lose";
    else cout << "You Win";
    
    endwin();
    return 0;
}
