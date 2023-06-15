#include <ncurses/ncurses.h>
#include <iostream>
#include <windows.h>
#include "SnakeGame.h"

#define DEBUG false

using namespace std;

SnakeGame::SnakeGame(): snake(MAP_SIZE / 2, MAP_SIZE / 2), wall(MAP_SIZE) {
    // map 외곽 초기화(WALL)
    for(pos p: this->wall.get_wall_info())
        setElement(p, ELEMENT_KIND::WALL);

    this->wall.initPortal();
    setElement(this->wall.getPortal1(), ELEMENT_KIND::PORTAL);
    setElement(this->wall.getPortal2(), ELEMENT_KIND::PORTAL);
    
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
     switch(CurrentStage){
          case 1:
               // Snake 시작 위치 초기화
               this->map[MAP_SIZE / 2][MAP_SIZE / 2] = ELEMENT_KIND::SNAKE_HEAD;

               for(int i = 1; i < 4; i++){
                    this->map[MAP_SIZE / 2][MAP_SIZE / 2 + i] = ELEMENT_KIND::SNAKE_BODY;
               }
               
               // map 외곽 초기화(IMMU_WALL)
               this->map[0][0] = this->map[0][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
               this->map[MAP_SIZE - 1][0] = this->map[MAP_SIZE - 1][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;

          break;

          case 2:
               // map 초기화(Board)
               for(int i = 1; i < MAP_SIZE-1; i++){
                    for(int j = 1; j < MAP_SIZE - 1; j++){
                         this->map[i][j] = ELEMENT_KIND::BOARD;
                    }
               }

               // snake 초기화
               this->snake = Snake(MAP_SIZE / 2, MAP_SIZE / 2);

               this->map[MAP_SIZE / 2][MAP_SIZE / 2] = ELEMENT_KIND::SNAKE_HEAD;

               for(int i = 1; i < 4; i++){
                    this->map[MAP_SIZE / 2][MAP_SIZE / 2 + i] = ELEMENT_KIND::SNAKE_BODY;
               }

               // map 외곽 초기화
               this->map[0][0] = this->map[0][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
               this->map[MAP_SIZE - 1][0] = this->map[MAP_SIZE - 1][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;

               // map 외곽 초기화(WALL)
               for(int i = 1; i < MAP_SIZE - 1; i++){
                    this->map[0][i] = this->map[i][0] = this->map[MAP_SIZE - 1][i] = this->map[i][MAP_SIZE - 1] = ELEMENT_KIND::WALL;    
               }

               for(int i = 0; i < 3; i++){
                    for(int j = 0 ; j < 10; j++){
                         this->map[4 + 4 * i][4 + j] = this->map[18 + 4 * i][4 + j] = ELEMENT_KIND::WALL;
                         this->map[4 + 4 * i][17 + j] = this->map[18 + 4 * i][17 + j] = ELEMENT_KIND::WALL;
                    }
               }
          
               break;

          case 3:
               // map 초기화(Board)
               for(int i = 1; i < MAP_SIZE-1; i++){
                    for(int j = 1; j < MAP_SIZE - 1; j++){
                         this->map[i][j] = ELEMENT_KIND::BOARD;
                    }
               }
               // snake 초기화
               this->snake = Snake(MAP_SIZE / 2, MAP_SIZE / 2);

               // Snake 시작 위치 초기화
               this->map[MAP_SIZE / 2][MAP_SIZE / 2] = ELEMENT_KIND::SNAKE_HEAD;

               for(int i = 1; i < 4; i++){
                    this->map[MAP_SIZE / 2][MAP_SIZE / 2 + i] = ELEMENT_KIND::SNAKE_BODY;
               }

               // map 외곽 초기화(IMMU_WALL)
               map[0][0] =  map[0][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
               map[MAP_SIZE - 1][0] =  map[MAP_SIZE - 1][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;

               // map 외곽 초기화(WALL)
               for(int i = 1; i < MAP_SIZE - 1; i++){
                    map[0][i] =  map[i][0] =  map[MAP_SIZE - 1][i] =  map[i][MAP_SIZE - 1] = ELEMENT_KIND::WALL;
               }

               
               for(int i = 1; i <= 3 ; i++ ){
                    for(int j = 1; j <= 3; j++){
                         map[8 * i + i -1][8 * j + j -1] =  map[8 * i + i][8 * j + j -1] = ELEMENT_KIND::WALL;
                         map[8 * i + i -1][8 * j + j] =  map[8 * i + i][8 * j + j] = ELEMENT_KIND::WALL;
                    }
               }

               for(int i = 1; i <= 3 ; i++ ){
                    for(int j = 0; j < 3; j++){
                         map[8 * i + i -1][3 * (3 * j + 1)] =  map[8 * i + i - 1][3 * (3 * j + 1) + 1] = ELEMENT_KIND::WALL;
                         map[8 * i + i][3 * (3 * j + 1)] =  map[8 * i + i][3 * (3 * j + 1) + 1] = ELEMENT_KIND::WALL;
                    }
               }

               for(int i = 0; i < 3; i++ ){
                    for(int j = 0; j < 3; j++){
                         map[3 * (3 * i + 1) ][3 * (3 * j + 1)] =  map[3 * (3 * i + 1)][3 * (3 * j + 1) + 1] = ELEMENT_KIND::WALL;
                         map[3 * (3 * i + 1) + 1][3 * (3 * j + 1)] =  map[3 * (3 * i + 1) + 1][3 * (3 * j + 1) + 1] = ELEMENT_KIND::WALL;
                    }
               }

               for(int i = 0; i < 3; i++ ){
                    for(int j = 1; j <= 3; j++){
                         map[3 * (3 * i + 1)][8 * j + j -1] =  map[3 * (3 * i + 1)][8 * j + j] = ELEMENT_KIND::WALL;
                         map[3 * (3 * i + 1) + 1][8 * j + j -1] =  map[3 * (3 * i + 1) + 1][8 * j + j] = ELEMENT_KIND::WALL;
                    }
               }
          break;

          case 4:
               // map 초기화(Board)
               for(int i = 1; i < MAP_SIZE-1; i++){
                    for(int j = 1; j < MAP_SIZE - 1; j++){
                         this->map[i][j] = ELEMENT_KIND::BOARD;
                    }
               }

               // snake 초기화
               this->snake = Snake(1, 15);

               this->map[1][15] = ELEMENT_KIND::SNAKE_HEAD;

               for(int i = 1; i < 3; i++){
                    this->map[1][15 + i] = ELEMENT_KIND::SNAKE_BODY;
               }

               // map 외곽 초기화(WALL)
               this->map[0][0] = this->map[0][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
               this->map[MAP_SIZE - 1][0] = this->map[MAP_SIZE - 1][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;

               // map 외곽 초기화(WALL)
               for(int i = 1; i < MAP_SIZE - 1; i++){
                    map[0][i] =  map[i][0] =  map[MAP_SIZE - 1][i] =  map[i][MAP_SIZE - 1] = ELEMENT_KIND::WALL;
               }

               //가로1
               for(int i = 0; i < 23; i++){
                    map[4][4 + i] =  map[26][4 + i] = ELEMENT_KIND::WALL;
               }
               
                    //가로2
               for(int i = 0; i < 11; i++){
                    map[8][8 + i] =  map[22][12 + i] = ELEMENT_KIND::WALL;
                    map[18][8 + i] =  map[12][12 + i] = ELEMENT_KIND::WALL;
               }

                    //세로1
               for(int i = 0; i < 18; i++){
                    map[8 + i][4] =  map[4 + i][26] = ELEMENT_KIND::WALL;
               }

               //세로1
               for(int i = 0; i < 14; i++){
                    map[9 + i][8] =  map[8 + i][22] = ELEMENT_KIND::WALL;
               }

               //세로1
               for(int i = 0; i < 3; i++){
                    map[13 + i][12] =  map[15 + i][18] = ELEMENT_KIND::WALL;
               }
          break;
          
          case 5:
               // map 초기화(Board)
               for(int i = 1; i < MAP_SIZE-1; i++){
                    for(int j = 1; j < MAP_SIZE - 1; j++){
                         this->map[i][j] = ELEMENT_KIND::BOARD;
                    }
               }

               // snake 초기화
               this->snake = Snake(1, 15);

               map[1][15] = ELEMENT_KIND::SNAKE_HEAD;

               for(int i = 1; i < 3; i++){
                    map[1][15 + i] = ELEMENT_KIND::SNAKE_BODY;
               }
               
               // map 외곽 초기화(IMMU_WALL)
                    map[0][0] =  map[0][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
                    map[MAP_SIZE - 1][0] =  map[MAP_SIZE - 1][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;

               // map 외곽 초기화(WALL)
               for(int i = 1; i < MAP_SIZE - 1; i++){
                    map[0][i] =  map[i][0] =  map[MAP_SIZE - 1][i] =  map[i][MAP_SIZE - 1] = ELEMENT_KIND::WALL;
               }
                    //S
               for(int i = 0; i < 4; i++){
                    map[2][2 + i] =  map[8][2 + i] = ELEMENT_KIND::WALL;
               }
               for(int i = 0; i < 3; i++){
                    map[3 + i][2] =  map[5 + i][5] = ELEMENT_KIND::WALL; 
               }
                    map[5][4] = ELEMENT_KIND::WALL;

               //N
               for(int i = 0; i < 7; i++){
                    map[2 + i][7] =  map[2 + i][12] =  ELEMENT_KIND::WALL;
               }

               for(int i = 0; i < 2; i++){
                    map[2 + i][8] =  map[6 + i][10] =  ELEMENT_KIND::WALL;
                    map[3 + i][9] =  map[7 + i][11] =  ELEMENT_KIND::WALL;
               }

               //A
               for(int i = 0; i < 7; i++){
                    map[2 + i][14] =  map[2 + i][18] =  ELEMENT_KIND::WALL;
                    map[22 + i][9] =  map[22 + i][13] =  ELEMENT_KIND::WALL;
               }

               for(int i = 0; i < 3; i++){
                    map[2][15 + i] =  map[22][10 + i] =  ELEMENT_KIND::WALL;
               }
                    map[5][16] =  map[25][11] =  ELEMENT_KIND::WALL;

               //K
               for(int i = 0; i < 3; i++){
                    map[2 + i][23 - i] =  map[6 + i][21 + i] = ELEMENT_KIND::WALL;
                    map[2 + i][20] =  map[6 + i][20] =ELEMENT_KIND::WALL;
               }    

               //E
               for(int i = 0; i < 4; i++){
                    map[2][25 + i] =  map[5][25 + i] =  map[8][25 + i] = ELEMENT_KIND::WALL;
                    map[22][25 + i] =  map[25][25 + i] =  map[28][25 + i] = ELEMENT_KIND::WALL;
               }

               for(int i = 0; i < 2; i++){
                    map[3 + i][25] =  map[6 + i][25] = ELEMENT_KIND::WALL;
                    map[23 + i][25] =  map[26 + i][25] = ELEMENT_KIND::WALL;
               }

               //G
               for(int i = 0; i < 6; i++){
                    map[22][2 + i] =  map[28][2 + i] = ELEMENT_KIND::WALL;
               }
               
               for(int i = 0; i < 5; i++){
                    map[23 + i][2] = ELEMENT_KIND::WALL;
               }

                    map[23][7] =  map[27][7] =  map[25][5] = ELEMENT_KIND::WALL;
                    map[25][6] =  map[25][7] =  map[26][5] = ELEMENT_KIND::WALL;

               //M
               for(int i = 0; i < 7; i++){
                    map[22 + i][15] =  map[22 + i][23] =  ELEMENT_KIND::WALL;
               }

                    map[23][16] =  map[23][22] =  map[25][17] =  map[25][21] = ELEMENT_KIND::WALL;
                    map[27][18] =  map[27][19] =  map[27][20] =  map[28][19] = ELEMENT_KIND::WALL;

               //SNAKE
               for(int i = 0; i < 3; i++){
                    for(int j = 0; j < 9; j++){
                         map[10 + i][2 + j] =  map[10 + i][14 + j] = ELEMENT_KIND::WALL;
                         map[18 + i][8 + j] =  map[18 + i][20 + j] = ELEMENT_KIND::WALL;
                    }
               }

               for(int i = 0; i < 11; i++){
                    for(int j = 0; j < 3; j++){
                         map[10 + i][2 + j] =  map[10 + i][14 + j] = ELEMENT_KIND::WALL;
                         map[10 + i][8 + j] =  map[10 + i][20 + j] = ELEMENT_KIND::WALL;
                         map[10 + i][26 + j] = ELEMENT_KIND::WALL;
                    }
               }

               for(int i = 0; i < 3; i++){
                    map[17][8 + i] =  map[13][20 + i] = ELEMENT_KIND::BOARD;
               }
          break;
          
          default:
               this->gameStatus = GAME_STATUS::WIN;
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

