#include <ctime>
#include <cstdlib>

#include "Wall.h"

Wall::Wall(int map_size, int stage): remain_length(0) {
    for(int row = 0; row < map_size; row++){
        for(int col = 0; col < map_size; col++){
            if(row == 0 && col == 0) continue;
            if(row == 0 && col == map_size - 1) continue;
            if(row == map_size - 1 && col == 0) continue;
            if(row == map_size - 1 && col == map_size - 1) continue;
            
            if((Wall::stage[stage - 1][row] >> col) & 1)
                this->wall.push_back(pos(row, map_size - col - 1));
        }
    }

    srand(time(NULL));
    int rnd1 = rand() % this->wall.size(), rnd2;
    for(rnd2 = rnd1; rnd1 == rnd2; rnd2 = rand() % this->wall.size());
    nextGate1 = this->wall[rnd1];
    nextGate2 = this->wall[rnd2];
}

const vector<pos>& Wall::get_wall_info(){
    return this->wall;
}

void Wall::initGate() {
    srand(time(NULL) + 3);

    nowGate1 = pos(nextGate1);
    nowGate2 = pos(nextGate2);

    int rnd1 = rand() % this->wall.size(), rnd2;
    for(rnd2 = rnd1; rnd1 == rnd2; rnd2 = rand() % this->wall.size());
    nextGate1 = this->wall[rnd1];
    nextGate2 = this->wall[rnd2];
}

const pos& Wall::getNowGate1() {
    return this->nowGate1;
}

const pos& Wall::getNowGate2() {
    return this->nowGate2;
}

const pos& Wall::getNextGate1() {
    return this->nextGate1;
}

const pos& Wall::getNextGate2() {
    return this->nextGate2;
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

int Wall::stage[5][31] = {
    { 
      HORIZONTAL_WALL,
      BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL,
      HORIZONTAL_WALL
    },

    { 
      HORIZONTAL_WALL,
      BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL | 134103024,
      BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL | 134103024,
      BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL | 134103024, 
      BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL, BLANK_WALL, 
      BLANK_WALL | 134103024, 
      BLANK_WALL, BLANK_WALL, BLANK_WALL, 
      BLANK_WALL | 134103024, 
      BLANK_WALL, BLANK_WALL, BLANK_WALL, 
      BLANK_WALL | 134103024, 
      BLANK_WALL, BLANK_WALL, BLANK_WALL,
      HORIZONTAL_WALL
    },

    { 
      HORIZONTAL_WALL,
      BLANK_WALL, BLANK_WALL, 
      BLANK_WALL | 1281766169, 
      BLANK_WALL | 1281766169, 
      BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL | 1281766169, 
      BLANK_WALL | 1281766169,
      BLANK_WALL, BLANK_WALL, 
      BLANK_WALL | 1281766169, 
      BLANK_WALL | 1281766169, 
      BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL | 1281766169,
      BLANK_WALL | 1281766169,
      BLANK_WALL, BLANK_WALL,
      BLANK_WALL | 1281766169,
      BLANK_WALL | 1281766169,
      BLANK_WALL, BLANK_WALL, BLANK_WALL,
      BLANK_WALL | 1281766169, BLANK_WALL | 1281766169,
      BLANK_WALL, BLANK_WALL,
      HORIZONTAL_WALL
    },

    { 
      HORIZONTAL_WALL,
      BLANK_WALL, BLANK_WALL, BLANK_WALL, 
      BLANK_WALL | 1207959537, 
      BLANK_WALL | 1073741841, 
      BLANK_WALL | 1073741841, 
      BLANK_WALL | 1073741841, 
      BLANK_WALL | 1149235473, 
      BLANK_WALL | 1145045265, 
      BLANK_WALL | 1145045265, 
      BLANK_WALL | 1145045265, 
      BLANK_WALL | 1145569041, 
      BLANK_WALL | 1145307409, 
      BLANK_WALL | 1145307409, 
      BLANK_WALL | 1145311505, 
      BLANK_WALL | 1145049361, 
      BLANK_WALL | 1145049361, 
      BLANK_WALL | 1149235473, 
      BLANK_WALL | 1145045265, 
      BLANK_WALL | 1145045265, 
      BLANK_WALL | 1145045265, 
      BLANK_WALL | 1145569041, 
      BLANK_WALL | 1140850689,
      BLANK_WALL | 1140850689, 
      BLANK_WALL | 1140850689, 
      BLANK_WALL | 1207959537, 
      BLANK_WALL, BLANK_WALL, BLANK_WALL,
      HORIZONTAL_WALL
    },

    { 
      HORIZONTAL_WALL,
      BLANK_WALL, 
      BLANK_WALL | 1590031549,
      BLANK_WALL | 1357190433,
      BLANK_WALL | 1352996385,
      BLANK_WALL | 1451577405,
      BLANK_WALL | 1117066785,
      BLANK_WALL | 1117590817,
      BLANK_WALL | 1586304189,
      BLANK_WALL, 
      BLANK_WALL | 1609695005,
      BLANK_WALL | 1343291669,
      BLANK_WALL | 1402027797,
      BLANK_WALL | 1414610965,
      BLANK_WALL | 1414612757,
      BLANK_WALL | 1414612245,
      BLANK_WALL | 1416709397,
      BLANK_WALL | 1409369365,
      BLANK_WALL | 1417561317,
      BLANK_WALL | 1413497861,
      BLANK_WALL | 1551878141,
      BLANK_WALL, 
      BLANK_WALL | 1606320317, 
      BLANK_WALL | 1352843681, 
      BLANK_WALL | 1344438433, 
      BLANK_WALL | 1403691709, 
      BLANK_WALL | 1377992865, 
      BLANK_WALL | 1352834209, 
      BLANK_WALL | 1604487357, 
      BLANK_WALL,
      HORIZONTAL_WALL
    },
};
