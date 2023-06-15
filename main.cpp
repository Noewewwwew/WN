#include "SnakeGame.h"
#include <ncurses/ncurses.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

auto getTime() {
    return steady_clock::now();
}
auto geteeime() {
    return steady_clock::now();
}
auto stagechangetime(const time_point<steady_clock>& start) {
    return duration_cast<milliseconds>(geteeime() - start).count();
}

auto milliInterval(const time_point<steady_clock>& start) {
    return duration_cast<milliseconds>(getTime() - start).count();
}

int main() {
    SnakeGame* game = new SnakeGame();

    game->init();
    char c;
    
    // 올바른 입력이 아니면 재대기 하는 느낌으로다가 만들어야 함
    // 300ms 마다 화면은 업데이트 되어야 함
    
    time_point start = getTime();
    time_point stttt = geteeime();
    while(!game->isLose()){
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

        if(milliInterval(start) >= TIMEOUT){
            game->update();
            start = getTime();
        }
        if(stagechangetime(stttt) == 1000){
            game->changemap();
            game->update();
            stttt = geteeime();
        }
    }

    if(game->isLose()) cout << "You Lose";
    else cout << "You Win";
    
    endwin();
    return 0;
}
