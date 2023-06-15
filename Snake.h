#include <deque>

// pair를 사용할 때 first는 Y, second는 X
#define Y first
#define X second

using namespace std;

// SNAKE 머리 방향
namespace SNAKE_HEAD_DIRECTION {
    const int LEFT = 0;
    const int UP = 1;
    const int RIGHT = 2;
    const int DOWN = 3;
};

typedef pair<int, int> pos;

const pos dPos[4] = {
    {0, -1}, 
    {-1, 0}, 
    {0, 1}, 
    {1, 0}
};

class Snake {
    deque<pos> snake;
    int direction;

public:
    Snake(const int& y, const int& x);
    const pos& head();
    const pos new_head();
    void move_head(const pos new_pos);
    const pos& tail();
    
    void set_head_direction(int direction);
    int get_head_direction();

    int get_snake_length();
    
    void grow();
    void grow(const pos new_pos);

    void shrink();
};
