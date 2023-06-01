#include <vector>

#define Y first
#define X second

using namespace std;

typedef pair<int, int> pos;

class Wall {
    vector<pos> wall;
    pos portal1, portal2;
    int remain_length;

public:
    Wall(int map_size);

    const vector<pos>& get_wall_info();

    void initPortal();

    const pos& getPortal1();
    const pos& getPortal2();

    bool isUsed();
    void setUsed(int snake_length);

    void update_remain_length();
};
