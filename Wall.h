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
    pos portal1, portal2;
    int remain_length;

public:
    Wall(int map_size, int stage);

    const vector<pos>& get_wall_info();

    void initPortal();

    const pos& getPortal1();
    const pos& getPortal2();

    bool isUsed();
    void setUsed(int snake_length);

    void update_remain_length();
};
