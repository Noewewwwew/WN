#include <ncurses/ncurses.h>
#include <iostream>
#include "SnakeGame.h"

SnakeGame::SnakeGame() {
    // map 외곽 초기화(IMMU_WALL)
    this->map[0][0] = this->map[0][MAP_SIZE - 1] = IMMU_WALL;
    this->map[MAP_SIZE - 1][0] = this->map[MAP_SIZE - 1][MAP_SIZE - 1] = IMMU_WALL;

    // map 외곽 초기화(WALL)
    for(int i = 1; i < MAP_SIZE - 1; i++){
        this->map[0][i] = this->map[i][0] = this->map[MAP_SIZE - 1][i] = this->map[i][MAP_SIZE - 1] = WALL;
    }
}

void SnakeGame::init() {
    // Screen 초기화
    initscr();

    // CMD 색상 사용
    start_color();

    // 칸 별 색상 정의
    // 그런데, pair는 1부터 시작할 수 있기 때문에 + 1해서 증가
    // 이제 굳이 board 전체를 순회해서 1로 만들어줄 필요는 없기 때문
    init_pair(BOARD + 1, COLOR_WHITE, COLOR_WHITE);
    
    init_pair(IMMU_WALL + 1, COLOR_BLUE, COLOR_BLUE);
    init_pair(WALL + 1, COLOR_CYAN, COLOR_CYAN);

    init_pair(SNAKE_HEAD + 1, COLOR_GREEN, COLOR_GREEN);
    init_pair(SNAKE_BODY + 1, COLOR_YELLOW, COLOR_YELLOW);
    
    // 화면에 그리기
    this->draw();
}

void SnakeGame::draw() {
    // 반각문자를 출력하기 때문에
    // 가로의 좌표를 출력할 때는 2칸씩 건너뛰어야 함
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE * 2; j += 2){
            // 2칸씩 좌표를 건너뛰지만, map의 길이는 MAP_SIZE 만큼이기 때문에
            // j / 2 를 해서 올바른 접근을 하게 함
            attron(COLOR_PAIR(this->map[i][j / 2] + 1));

            // 반각문자이기 때문에 공백 2칸 출력
            mvprintw(i, j, "  ");
        }
    }

    // ncurses 함수 / 화면 갱신
    refresh();
}
