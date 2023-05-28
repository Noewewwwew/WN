#include <deque>

// 맵 사이즈
#define MAP_SIZE 21

// pair를 사용할 때 first는 Y, second는 X
#define Y first
#define X second

// snake 의 머리와 몸통에 대한 정보
typedef std::pair<int, int> pos;

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

// SNAKE 머리 방향
namespace SNAKE_HEAD_DIRECTION {
    const int LEFT = 0;
    const int UP = 1;
    const int RIGHT = 2;
    const int DOWN = 3;
};

class SnakeGame {
    // 맵 데이터
    int map[MAP_SIZE][MAP_SIZE];

    // 게임 상태는 게임 중
    int gameStatus = GAME_STATUS::GAMING;

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

    // 초기화
    void init();

    // 화면에 그리기
    void draw();
};
