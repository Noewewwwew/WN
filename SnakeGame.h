#include <deque>
#include <iostream>

#include "kmu_curses.h"
#include "Snake.h"
#include "Wall.h"

using namespace std;

//item appear percentage
const int ITEM_APPEAR_PROBABILITY = 30;  

// 맵 사이즈
#define MAP_SIZE 31

// Score Board 사이즈
#define BOARD_SIZE 20

// 타임아웃 시간: 0.3초
#define UPDATE_DURATION 300

// 아이템 갱신 시간: 8초
#define ITEM_DURATION 8000

#define CHANGE_DURATION 2000

#define NEXT_GATE_DURATION 3000

// 벽 갱신 시간: 15초
#define GATE_DURATION 15000

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
    const int GATE = 3;
    const int NEXT_GATE = 4;
    const int SNAKE_HEAD = 5;
    const int SNAKE_BODY = 6;
    const int GROWTH_ITEM = 7;
    const int POISON_ITEM = 8;
    const int REVERSE_ITEM = 9;
};

struct MissionCnt {
    int maxSnakeLength, gate, poisonItem, growthItem;

    MissionCnt(int maxSnakeLength = 0, int growthItem = 0, int poisonItem = 0, int gate = 0) {
        this->maxSnakeLength = maxSnakeLength;
        this->growthItem = growthItem;
        this->poisonItem = poisonItem;
        this->gate = gate;
    }
};

class SnakeGame {
    // 맵 데이터
    int map[MAP_SIZE][MAP_SIZE];

    bool nextGateShowFlag = false;

    // 게임 상태는 게임 중
    int gameStatus = GAME_STATUS::GAMING;
  
    const int NUM_ITEMS = 3;
  
    // Function to find a random empty space on the map
    pos findRandomEmptySpace();

    Snake snake;

    WINDOW* scoreBoard;
    WINDOW* missionBoard;
    WINDOW* gameBoard;
    WINDOW* noticeText;

    MissionCnt totalCnt, currCnt;

public:
    Wall wall;

    // 생성자
    SnakeGame();

    int currStage;

    const MissionCnt mission[5] = {
        MissionCnt(6, 2, 5, 1),
        MissionCnt(7, 3, 4, 2),
        MissionCnt(8, 4, 3, 3),
        MissionCnt(9, 5, 2, 4),
        MissionCnt(10, 6, 1, 5),
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
    void draw(const string& msg="", bool clear = false);
    void drawScoreBoard(int64_t time);

    // 뱀 움직임(2단계) / 아이템 등장(3단계) / 포탈 등장(4단계) 등등 연산
    void update(int64_t time);
    
    // 스테이지 변경
    void changeMap();

    // 맵 업데이트(5단계)
    void mapUpate();

    void changeNoticeMessage(const char* msg);

    void changeGate();

    bool isMissionClear();

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

    bool getNextGateShowFlag();
    void setNextGateShowFlag(bool gateFlag);
  
    // item arange
    void createItems();
    void removeExpiredItems();
    int getItem(const pos& position);
};