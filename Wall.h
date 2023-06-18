#include <vector>

#define Y first
#define X second

#define HORIZONTAL_WALL 2147483647
#define BLANK_WALL 1073741825

using namespace std;

typedef pair<int, int> pos;

class Wall {
    static int stage[5][31];
    vector<pos> wall;
    pos nowGate1, nowGate2, nextGate1, nextGate2;
    int remain_length;

public:
    Wall(int map_size, int stage);

    const vector<pos>& get_wall_info();

    void initGate();

    const pos& getNowGate1();
    const pos& getNowGate2();

    const pos& getNextGate1();
    const pos& getNextGate2();

    bool isUsed();
    void setUsed(int snake_length);

    void update_remain_length();
};
