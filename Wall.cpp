#include <ctime>
#include <cstdlib>

#include "Wall.h"

Wall::Wall(int map_size): remain_length(0) {
    // 기본 테두리
    for(int i = 1; i < map_size - 1; i++){
        wall.push_back(pos(0, i));
        wall.push_back(pos(map_size - 1, i));
        wall.push_back(pos(i, 0));
        wall.push_back(pos(i, map_size - 1));
    }
}

const vector<pos>& Wall::get_wall_info(){
    return this->wall;
}

void Wall::initPortal() {
    srand(time(NULL));

    int rnd1 = rand() % this->wall.size(), rnd2;
    for(rnd2 = rnd1; rnd1 == rnd2; rnd2 = rand() % this->wall.size());

    portal1 = this->wall[rnd1];
    portal2 = this->wall[rnd2];
}

const pos& Wall::getPortal1() {
    return this->portal1;
}

const pos& Wall::getPortal2() {
    return this->portal2;
}

bool Wall::isUsed() {
    return this->remain_length != 0;
}

void Wall::setUsed(int remain_snake_length) {
    this->remain_length = remain_snake_length;
}

void Wall::update_remain_length() {
    if(!isUsed()) return;
    remain_length -= 1;
}
