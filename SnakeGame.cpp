#include <ncurses/ncurses.h>
#include <iostream>
#include <windows.h>
#include <memory.h>
#include "SnakeGame.h"

#define DEBUG false

using namespace std;

SnakeGame::SnakeGame(): snake(1, MAP_SIZE / 2), wall(MAP_SIZE, 1) {
    // 시작 상태 초기화(스테이지, 아이템)
    CurrentStage = 1;
    B = GI = PI = G = 0;
     
    mapupdate();
}

void SnakeGame::ScoreBoard(){
     for(int i = 0 ; i < 20 ; i ++){
          for(int j = 0 ; i < 20 ; j++){
               this->board[33 + i][1 + i]  = ELEMENT_KIND::SCOREBOARD;
          }
     }
}

void SnakeGame::init() {
    // Screen 초기화
    initscr();

    // CMD 색상 사용
    start_color();

    noecho();

    nodelay(stdscr, TRUE);

    // 칸 별 색상 정의
    // 그런데, pair는 1부터 시작할 수 있기 때문에 + 1해서 증가
    // 이제 굳이 board 전체를 순회해서 1로 만들어줄 필요는 없기 때문
    init_pair(ELEMENT_KIND::BOARD + 1, COLOR_WHITE, COLOR_WHITE);
    init_pair(ELEMENT_KIND::SCOREBOARD + 1, COLOR_MAGENTA, COLOR_MAGENTA);
    
    init_pair(ELEMENT_KIND::IMMU_WALL + 1, COLOR_BLUE, COLOR_BLUE);
    init_pair(ELEMENT_KIND::WALL + 1, COLOR_CYAN, COLOR_CYAN);

    init_pair(ELEMENT_KIND::SNAKE_HEAD + 1, COLOR_GREEN, COLOR_GREEN);
    init_pair(ELEMENT_KIND::SNAKE_BODY + 1, COLOR_YELLOW, COLOR_YELLOW);

    init_pair(ELEMENT_KIND::PORTAL + 1, COLOR_MAGENTA, COLOR_MAGENTA);
    
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
            mvprintw(i, j, DEBUG ? "a " : "  ");
        }
    }

    // ncurses 함수 / 화면 갱신
    refresh();
}

void SnakeGame::changePortal() {
    setElement(this->wall.getPortal1(), ELEMENT_KIND::WALL);
    setElement(this->wall.getPortal2(), ELEMENT_KIND::WALL);
    
    this->wall.initPortal();
    setElement(this->wall.getPortal1(), ELEMENT_KIND::PORTAL);
    setElement(this->wall.getPortal2(), ELEMENT_KIND::PORTAL);
}

void SnakeGame::changemap(){
     // 게임 상태 업데이트 작업
     // 미션 달성 체크 및 스테이지 변경 로직 구현
     // 스테이지 넘어가는 로직
     CurrentStage++;
     SnakeGame::mapupdate(); // 맵 업데이트 호출
     // 다음 스테이지 초기화 작업 등을 수행
}

void SnakeGame::mapupdate(){
     if(CurrentStage == 6){
          this->setGameStatus(GAME_STATUS::WIN);
          return;
     }

     // map 초기화
     memset(map, ELEMENT_KIND::BOARD, sizeof(map));

     this->snake = Snake(1, MAP_SIZE / 2);

     // Snake 시작 위치 초기화
     this->map[1][MAP_SIZE / 2] = ELEMENT_KIND::SNAKE_HEAD;

     for(int i = 1; i < 4; i++){
          this->map[1][MAP_SIZE / 2 + i] = ELEMENT_KIND::SNAKE_BODY;
     }

     this->wall = Wall(MAP_SIZE, CurrentStage);

     // map 외곽 초기화(WALL)
     for(pos p: this->wall.get_wall_info())
          setElement(p, ELEMENT_KIND::WALL);
     
     this->wall.initPortal();
     setElement(this->wall.getPortal1(), ELEMENT_KIND::PORTAL);
     setElement(this->wall.getPortal2(), ELEMENT_KIND::PORTAL);

     // map 외곽 초기화(IMMU_WALL)
     this->map[0][0] = this->map[0][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
     this->map[MAP_SIZE - 1][0] = this->map[MAP_SIZE - 1][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
     if(CurrentStage == 5){
          for(int i = 11; i < 20; i++){
               this->map[i][3] = ELEMENT_KIND::IMMU_WALL;
          }

          for(int i = 3; i < 10; i++){
               this->map[11][i] = ELEMENT_KIND::IMMU_WALL;
          }

          for(int i = 11; i < 15; i++){
               this->map[i][9] = ELEMENT_KIND::IMMU_WALL;
          }

          for(int i = 9; i < 16; i++){
               this->map[19][i] = ELEMENT_KIND::IMMU_WALL;
          }

          for(int i = 11; i < 19; i++){
               this->map[i][15] = ELEMENT_KIND::IMMU_WALL;
          }

          for(int i = 15; i < 22; i++){
               this->map[11][i] = ELEMENT_KIND::IMMU_WALL;
          }

          for(int i = 15; i < 19; i++){
               this->map[i][21] = ELEMENT_KIND::IMMU_WALL;
          }

          for(int i = 21; i < 28; i++){
               this->map[19][i] = ELEMENT_KIND::IMMU_WALL;
          }

          for(int i = 11; i < 19; i++){
               this->map[i][27] = ELEMENT_KIND::IMMU_WALL;
          }
     }
}

void SnakeGame::update(){
    // 새로운 머리 좌표에 해당하는 map의 값을 가져옴
    if(this->wall.isUsed()) this->wall.update_remain_length();

    switch(this->getElement(this->snake.new_head())){
    // 앞으로 이동
    case ELEMENT_KIND::BOARD:
        // 현재 머리 좌표는 SNAKE::BODY로 변경, 새로운 머리 좌표는 SNAKE_HEAD로 변경
        if(this->getElement(this->snake.head()) != ELEMENT_KIND::PORTAL){
            this->setElement(this->snake.head(), ELEMENT_KIND::SNAKE_BODY);
        
            // 현재 꼬리는 ELEMENT_KIND::BOARD로 변경하고, 
            this->setElement(this->snake.tail(), ELEMENT_KIND::BOARD);
            this->snake.shrink();
    
            this->snake.grow();
            this->setElement(this->snake.head(), ELEMENT_KIND::SNAKE_HEAD);
        } else {
            this->snake.move_head(this->snake.new_head());
            this->setElement(this->snake.head(), ELEMENT_KIND::SNAKE_HEAD);
        }

        // 새로운 머리의 위치를 맨 앞에 삽입
        break;

    case ELEMENT_KIND::GROWTH_ITEM:
    case ELEMENT_KIND::POISON_ITEM:
        break;

    case ELEMENT_KIND::PORTAL:{
        this->wall.setUsed(this->snake.get_snake_length());

        const pos& exit = this->snake.new_head() != this->wall.getPortal1() ? this->wall.getPortal1() : this->wall.getPortal2();

        // 포탈을 통해 이동한 것과 같으니까 꼬리 1칸 줄이기
        this->setElement(this->snake.tail(), ELEMENT_KIND::BOARD);
        this->snake.shrink();

        // 원래 머리는 SNAKE_BODY로 변경, 탈출구 위치에 머리 놓기 ==> 만약에 포탈 앞에 아이템이 놓여있으면 적용 X
        this->setElement(this->snake.head(), ELEMENT_KIND::SNAKE_BODY);
        // 탈출구 쪽으로 머리 늘리기
        this->snake.grow(exit);

        // 포탈에서 나갈 방향 정하기
        if(exit.Y == 0) this->snake.set_head_direction(SNAKE_HEAD_DIRECTION::DOWN);
        else if(exit.Y == MAP_SIZE - 1) this->snake.set_head_direction(SNAKE_HEAD_DIRECTION::UP);
        else if(exit.X == 0) this->snake.set_head_direction(SNAKE_HEAD_DIRECTION::RIGHT);
        else if(exit.X == MAP_SIZE - 1) this->snake.set_head_direction(SNAKE_HEAD_DIRECTION::LEFT);
        else {
            int direction = this->snake.get_head_direction();
            do {
                if(this->getElement(this->snake.new_head()) != ELEMENT_KIND::WALL)
                    break;
                this->snake.set_head_direction((this->snake.get_head_direction() + 1) % 4);
            } while(direction != this->snake.get_head_direction());
        }
        
        // 새롭게 업데이트
        update();
        return;
    }
    case ELEMENT_KIND::SNAKE_BODY:
    case ELEMENT_KIND::IMMU_WALL:
    case ELEMENT_KIND::WALL:
        this->gameStatus = GAME_STATUS::LOSE;
        break;
    }

    draw();
}

