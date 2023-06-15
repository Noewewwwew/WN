#include <deque>
#include <iostream>

#include "Snake.h"
#include "Wall.h"

using namespace std;

// 맵 사이즈
#define MAP_SIZE 31

// Score Board 사이즈
#define BOARD_SIZE 20

// 타임아웃 시간
#define UPDATE_DURATION 300

// 벽 갱신 시간: 15초
#define PORTAL_DURATION 15000

// pair를 사용할 때 first는 Y, second는 X
#define Y first
#define X second

// 게임 상태 FLAG
namespace GAME_STATUS {
    const int LOSE = 0;
    const int GAMING = 1;
    const int WIN = 2;
};

// map 데이터 별 이름 지정
namespace ELEMENT_KIND {
    const int BOARD = 0;
    const int IMMU_WALL = 1;
    const int WALL = 2;
    const int PORTAL = 3;
    const int SNAKE_HEAD = 4;
    const int SNAKE_BODY = 5;
    const int GROWTH_ITEM = 6;
    const int POISON_ITEM = 7;
    const int SCOREBOARD = 8;
};

class SnakeGame {
    // 맵 데이터
    int map[MAP_SIZE][MAP_SIZE];
    
    int board[BOARD_SIZE][BOARD_SIZE];

    // 게임 상태는 게임 중
    int gameStatus = GAME_STATUS::GAMING;

    Snake snake;

public:
    Wall wall;

    // 생성자
    SnakeGame();

    // 게임 상태 get, set 메서드
    int CurrentStage;
    int B; // Current lengrh / Max length
    int GI; // Growth Item
    int PI; // Poison Item
    int G; // Gate 사용 횟수
    int mission[5][4] = { // B, GI, PI, G  // 이후 미션 조정
        {6,1,1,1},
        {7,2,2,2},
        {8,3,3,3},
        {9,4,4,4},
        {10,5,5,5}
    };
    
    int getGameStatus() { return this->gameStatus; }
    void setGameStatus(int gameStatus) {
        this->gameStatus = gameStatus;
    }

    // 게임 상태가 GAMING, LOSE, WIN 인지 각각 반환
    bool isGaming() { return getGameStatus() == GAME_STATUS::GAMING; }
    bool isLose() { return getGameStatus() == GAME_STATUS::LOSE; }
    bool isWin() { return getGameStatus() == GAME_STATUS::WIN; }

    void setDirection(int direction) { this->snake.set_head_direction(direction); }

    // 초기화
    void init();

    // 화면에 그리기
    void draw();

    // 뱀 움직임(2단계) / 아이템 등장(3단계) / 포탈 등장(4단계) 등등 연산
    void update();
    
    // 스테이지 변경
    void changemap();

    // 맵 업데이트(5단계)
    void mapupdate();

    // Score Board
    void ScoreBoard();

    void changePortal();

    int& getElement(const int& y, const int& x){
        // 잘못된 맵 접근
        if(y < 0 || y >= MAP_SIZE || x < 0 || x >= MAP_SIZE) throw;
        return this->map[y][x];
    }

    int& getElement(const pos& _pos){
        if(_pos.Y < 0 || _pos.Y >= MAP_SIZE || _pos.X < 0 || _pos.X >= MAP_SIZE) throw;
        return this->map[_pos.Y][_pos.X];
    }

    void setElement(const pos& _pos, const int& value){
        if(_pos.Y < 0 || _pos.Y >= MAP_SIZE || _pos.X < 0 || _pos.X >= MAP_SIZE) throw;
        this->map[_pos.Y][_pos.X] = value;
    }
};
