#include <iostream>
#include <chrono>

#include "kmu_curses.h"
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
    
    auto updateTime = getTime(), gateTime = getTime(), itemTime = getTime();
    auto startTime = getTime(), changeTime = getTime();
    bool missionFlag = false, gateFlag = true;

    while(getch() == ERR){
 string title  = "\toooooo  oo    oo      oooo      oo   oo  oooooo    oooooooo      oooo      oo       oo  oooooo \n";
        title += "\too      oooo  oo     oo  oo     oo  oo   oo        oo           oo  oo     oooo   oooo  oo     \n";
        title += "\toooooo  oo oo oo    oooooooo    ooooo    oooooo    oo  oooo    oooooooo    oo oo oo oo  oooooo \n";
        title += "\t    oo  oo  oooo   oo      oo   oo  oo   oo        oo    oo   oo      oo   oo   oo  oo  oo     \n";
        title += "\toooooo  oo   ooo  oo        oo  oo   oo  oooooo    oooooooo  oo        oo  oo       oo  oooooo \n";

        game->draw(title);
    }

    game->createItems();
    game->draw("", true);

    while(game->isGaming()){
        if(!game->wall.isUsed() && game->isMissionClear()){
            if(!missionFlag) {
                missionFlag = true;
                changeTime = getTime();
                game->changeNoticeMessage("");
                game->setDirection(SNAKE_HEAD_DIRECTION::LEFT);
            }
            
            getch();
            auto calcTime = milliInterval(changeTime);
            if(calcTime < 3000LL){
                char msg[50];
                snprintf(msg, sizeof(msg), "change %d stage to %d stage after %lld sec...", game->currStage, game->currStage + 1, 3LL - (calcTime / 1000));
                game->draw(msg);
            } else {
                missionFlag = false;
                updateTime = gateTime = itemTime = getTime();
                game->changeMap();

                game->removeExpiredItems();
                game->createItems();

                game->changeNoticeMessage("start new stage!");

                game->update(milliInterval(startTime) / 1000LL);
            }

            continue;
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

        if(milliInterval(gateTime) >= GATE_DURATION - NEXT_GATE_DURATION && !game->getNextGateShowFlag()){
            game->setNextGateShowFlag(true);
        }

        if(!game->wall.isUsed() && milliInterval(gateTime) >= GATE_DURATION){
            game->changeGate();
            gateTime = getTime();
            game->setNextGateShowFlag(false);
        }
        
        if(milliInterval(itemTime) >= ITEM_DURATION){
            game->removeExpiredItems();
            game->createItems();
            itemTime = getTime();
        }

        if(milliInterval(updateTime) >= UPDATE_DURATION){
            game->update(milliInterval(startTime) / 1000LL);
            updateTime = getTime();
        }
    }

    while(game->isLose() && getch() == ERR){
 string title  = "\too      oo oooooooo  oo    oo    oo       oooooooo  oooooo  oooooo \n";
        title += "\t oo    oo  oo    oo  oo    oo    oo       oo    oo  oo      oo     \n";
        title += "\t  oooooo   oo    oo  oo    oo    oo       oo    oo  oooooo  oooooo \n";
        title += "\t    oo     oo    0o  oo    oo    oo       oo    oo      oo  oo     \n";
        title += "\t    oo     oooooooo  oooooooo    oooooooo oooooooo  oooooo  oooooo \n";

        game->draw(title, true);
    }

    while(game->isWin() && getch() == ERR){
 string title  = "\too      oo oooooooo  oo    oo    oo     oo     oo oooooo  oo    oo  \n";
        title += "\t oo    oo  oo    oo  oo    oo    oo    oooo    oo   oo    oooo  oo  \n";
        title += "\t  oooooo   oo    oo  oo    oo     oo  oo  oo  oo    oo    oo oo oo  \n";
        title += "\t    oo     oo    0o  oo    oo     oo  oo  oo  oo    oo    oo  oooo  \n";
        title += "\t    oo     oooooooo  oooooooo      oooo    oooo   oooooo  oo   ooo \n";

        game->draw(title, true);
    }
    
    endwin();
    return 0;
}
