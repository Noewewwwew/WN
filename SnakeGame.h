#include <deque>

// 맵 사이즈
#define MAP_SIZE 21

// 게임 상태 FLAG
#define LOSE 0
#define GAMING 1
#define WIN 2

// map 데이터 별 이름 지정
#define BOARD 0
#define IMMU_WALL 1
#define WALL 2
#define PORTAL 3
#define SNAKE_HEAD 4
#define SNAKE_BODY 5
#define GROWTH_ITEM 6
#define POISON_ITEM 7

// pair를 사용할 때 first는 Y, second는 X
#define Y first
#define X second

// SNAKE 머리 방향
#define LEFT 0
#define UP 1
#define DOWN 2
#define RIGHT 3

// snake 의 머리와 몸통에 대한 정보
typedef std::pair<int, int> pos;

class SnakeGame {
    // 맵 데이터
    int map[MAP_SIZE][MAP_SIZE];

    // 게임 상태는 게임 중
    int gameStatus = GAMING;

public:
    // 생성자
    SnakeGame();
    
    // 게임 상태 get, set 메서드
    int getGameStatus() { return this->gameStatus; }
    void setGameStatus(int gameStatus) {
        this->gameStatus = gameStatus;
    }
    
    // 게임 상태가 GAMING, LOSE, WIN 인지 각각 반환
    bool isGaming() { return getGameStatus() == GAMING; }
    bool isLose() { return getGameStatus() == LOSE; }
    bool isWin() { return getGameStatus() == WIN; }

    // 초기화
    void init();

    // 화면에 그리기
    void draw();
};
