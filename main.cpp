#include "SnakeGame.h"
#include <ncurses/ncurses.h>
#include <iostream>

int main() {
    SnakeGame* game = new SnakeGame();

    game->init();
    getch();
    endwin();
    return 0;
}
