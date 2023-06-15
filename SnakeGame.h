#pragma once

//item appear percentage
const int ITEM_APPEAR_PROBABILITY = 30;  
#include <deque>
#include <iostream>

//map size
#define MAP_SIZE 31

// 타임아웃 시간
#define TIMEOUT 300

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
};

// SNAKE 
namespace SNAKE_HEAD_DIRECTION {
    const int LEFT = 0;
    const int UP = 1;
    const int RIGHT = 2;
    const int DOWN = 3;
};

// snake 의 머리와 몸통에 대한 정보
typedef std::pair<int, int> pos;

const int dPos[4][2] = {
    {0, -1}, 
    {-1, 0}, 
    {0, 1}, 
    {1, 0}
};

class SnakeGame {
    private:
    static const int NUM_ITEMS = 10;  //
    
    // 맵 데이터
    int map[MAP_SIZE][MAP_SIZE];

    // 게임 상태는 게임 중
    int gameStatus = GAME_STATUS::GAMING;

    // snake
    std::deque<pos> snake;

    int snake_direction = SNAKE_HEAD_DIRECTION::LEFT;

    // Function to find a random empty space on the map
    pos findRandomEmptySpace(int map[MAP_SIZE][MAP_SIZE]);

public:
    // 생성자
    SnakeGame();
    
    // 게임 상태 get, set 메서드
    int getGameStatus() { return this->gameStatus; }
    void setGameStatus(int gameStatus) {
        this->gameStatus = gameStatus;
    }
    
    // 게임 상태가 GAMING, LOSE, WIN 인지 각각 반환
    bool isGaming() { return getGameStatus() == GAME_STATUS::GAMING; }
    bool isLose() { return getGameStatus() == GAME_STATUS::LOSE; }
    bool isWin() { return getGameStatus() == GAME_STATUS::WIN; }

    void setDirection(int direction) { this->snake_direction = direction; }

    // 초기화
    void init();

    // 화면에 그리기
    void draw();

    // 뱀 움직임(2단계) / 아이템 등장(3단계) / 포탈 등장(4단계) 등등 연산
    void update();

    int& getElement(const int& y, const int& x){
        // 잘못된 맵 접근
        if(y < 0 || y >= MAP_SIZE || x < 0 || x >= MAP_SIZE) throw;
        return this->map[y][x];
    }

    int& getElement(const pos& _pos){
        if(_pos.Y < 0 || _pos.Y >= MAP_SIZE || _pos.X < 0 || _pos.X >= MAP_SIZE) throw;
        return this->map[_pos.Y][_pos.X];
    }

    // item arange
    void createItems();
    void removeExpiredItems();
    void handleItem(const pos& position, int element);
    int getItem(const pos& position);

    // game loop
    void play();
};

// 맵 생성
void generateMap(int map[MAP_SIZE][MAP_SIZE]);

// end of the game
void printGameOver();

// approval item
pos findValidPortalPosition(int map[MAP_SIZE][MAP_SIZE]);