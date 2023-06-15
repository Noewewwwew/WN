#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <deque>
#include "SnakeGame.h"

using namespace std;

SnakeGame::SnakeGame() {
    // 뱀 초기화
    snake.push_back(pos(MAP_SIZE / 2, MAP_SIZE / 2));
    this->map[MAP_SIZE / 2][MAP_SIZE / 2] = ELEMENT_KIND::SNAKE_HEAD;

    for(int i = 1; i < 3; i++){
        snake.push_back(pos(MAP_SIZE / 2, MAP_SIZE / 2 + i));
        this->map[MAP_SIZE / 2][MAP_SIZE / 2 + i] = ELEMENT_KIND::SNAKE_BODY;
    }
    
    // map 외곽 초기화(IMMU_WALL)
    this->map[0][0] = this->map[0][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
    this->map[MAP_SIZE - 1][0] = this->map[MAP_SIZE - 1][MAP_SIZE - 1] = ELEMENT_KIND::IMMU_WALL;
    // map 외곽 초기화(WALL)
    for(int i = 1; i < MAP_SIZE - 1; i++){
        this->map[0][i] = this->map[i][0] = this->map[MAP_SIZE - 1][i] = this->map[i][MAP_SIZE - 1] = ELEMENT_KIND::WALL;
    }
}

void SnakeGame::init() {
    // Screen 초기화
    initscr();
    // CMD 색상 사용
    start_color();
    noecho();
    nodelay(stdscr, TRUE);
    // 칸 별 색상 정의
    // 그런데, pair는 1부터 시작할 수 있기 때문에 + 1해서 증가
    // 이제 굳이 board 전체를 순회해서 1로 만들어줄 필요는 없기 때문
    init_pair(ELEMENT_KIND::BOARD + 1, COLOR_WHITE, COLOR_WHITE);
    
    init_pair(ELEMENT_KIND::IMMU_WALL + 1, COLOR_BLUE, COLOR_BLUE);
    init_pair(ELEMENT_KIND::WALL + 1, COLOR_CYAN, COLOR_CYAN);
    init_pair(ELEMENT_KIND::SNAKE_HEAD + 1, COLOR_GREEN, COLOR_GREEN);
    init_pair(ELEMENT_KIND::SNAKE_BODY + 1, COLOR_YELLOW, COLOR_YELLOW);
    
    // 화면에 그리기
    this->draw();

    createItems();
}

void SnakeGame::draw() {
    // 반각문자를 출력하기 때문에
    // 가로의 좌표를 출력할 때는 2칸씩 건너뛰어야 함
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE * 2; j += 2){
            // 2칸씩 좌표를 건너뛰지만, map의 길이는 MAP_SIZE 만큼이기 때문에
            // j / 2 를 해서 올바른 접근을 하게 함
            attron(COLOR_PAIR(this->map[i][j / 2] + 1));
            // 반각문자이기 때문에 공백 2칸 출력
            mvprintw(i, j, "  ");
        }
    }
    // ncurses 함수 / 화면 갱신

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            switch (map[i][j]) {
    case ELEMENT_KIND::GROWTH_ITEM:
                    // Print a growth item character for growth items
                    printw("+");
                    break;
                case ELEMENT_KIND::POISON_ITEM:
                    // Print a poison item character for poison items
                    printw("-");
                    break;
                default:
                    break;}
        }
    refresh();
}
}

void SnakeGame::update(){
    // 뱀 움직임
    const pos& old_snake_head = this->snake.front();
    const auto& now_direction = dPos[this->snake_direction];
    const pos new_snake_pos(old_snake_head.Y + now_direction[0], old_snake_head.X + now_direction[1]);
    int& element = getElement(new_snake_pos);
    switch(element){
    // 앞으로 이동
    case ELEMENT_KIND::BOARD:{
        // 현재 맨 앞에 들어있는 좌표 == 머리의 좌표는 SNAKE::BODY로 변경
        this->map[old_snake_head.Y][old_snake_head.X] = ELEMENT_KIND::SNAKE_BODY;
        // 새로운 머리의 위치를 맨 앞에 삽입
        this->snake.push_front(new_snake_pos);
        // 새로운 머리의 위치에 맞게 BOARD->SNAKE_HEAD로 변경 
        element = ELEMENT_KIND::SNAKE_HEAD;
        
        // 현재 꼬리는 ELEMENT_KIND::BOARD로 변경
        this->map[this->snake.back().Y][this->snake.back().X] = ELEMENT_KIND::BOARD;
        
        // 꼬리 제거(한칸 이동이니까)
        this->snake.pop_back();
        break;
    }
    case ELEMENT_KIND::GROWTH_ITEM:
    case ELEMENT_KIND::POISON_ITEM:
        break;
    case ELEMENT_KIND::PORTAL:
        break;
    case ELEMENT_KIND::SNAKE_BODY:
    case ELEMENT_KIND::IMMU_WALL:
    case ELEMENT_KIND::WALL:
        this->gameStatus = GAME_STATUS::LOSE;
        break;
    }
    draw();
}

void SnakeGame::createItems() {
    // Generate new items at random positions
    for (int i = 0; i < NUM_ITEMS; i++) {
        pos itemPosition;
        do {
            // Find a random empty space on the map
            itemPosition = findRandomEmptySpace(map);
        } while (itemPosition.X == -1 || itemPosition.Y == -1); // Retry if no empty space is found

        // Determine item type
        int itemType = (rand() % 2 == 0) ? ELEMENT_KIND::GROWTH_ITEM : ELEMENT_KIND::POISON_ITEM;

        // Set item on the map
        map[itemPosition.Y][itemPosition.X] = itemType;
    }
}


void SnakeGame::removeExpiredItems() {
    // Remove expired items from the map
    for (int i = 1; i < MAP_SIZE - 1; i++) {
        for (int j = 1; j < MAP_SIZE - 1; j++) {
            if (map[i][j] == ELEMENT_KIND::GROWTH_ITEM || map[i][j] == ELEMENT_KIND::POISON_ITEM) {
                map[i][j] = ELEMENT_KIND::BOARD;
            }
        }
    }
}

void SnakeGame::handleItem(const pos& position, int element) {
    // Handle the effect of an item
    if (element == ELEMENT_KIND::GROWTH_ITEM) {
        // If it's a growth item, extend the snake by 1 unit
        snake.push_back(position);
    }
    else if (element == ELEMENT_KIND::POISON_ITEM) {
        // If it's a poison item, shrink the snake by 1 unit
        if (snake.size() > 1) {
            map[snake.front().Y][snake.front().X] = ELEMENT_KIND::BOARD;
            snake.pop_front();
        }
    }
}

int SnakeGame::getItem(const pos& position) {
    // Get the item at the given position
    return map[position.Y][position.X];
}


pos SnakeGame::findRandomEmptySpace(int map[MAP_SIZE][MAP_SIZE]) {
    pos emptySpace = {-1, -1};

    // Randomly choose a starting position and find an empty space
    int startX = rand() % (MAP_SIZE - 2) + 1;
    int startY = rand() % (MAP_SIZE - 2) + 1;

    for (int i = 0; i < MAP_SIZE - 2; i++) {
        int x = (startX + i) % (MAP_SIZE - 2) + 1;
        for (int j = 0; j < MAP_SIZE - 2; j++) {
            int y = (startY + j) % (MAP_SIZE - 2) + 1;
            if (map[y][x] == ELEMENT_KIND::BOARD) {
                emptySpace = {y, x};
                return emptySpace;
            }
        }
    }

    return emptySpace;
}

void SnakeGame::play() {
    // Set up ncurses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);

    // Main game loop
    while (isGaming()) {
        // Draw the current state of the game
        draw();

        // Wait for user input
        int ch = getch();

        // Process user input
        switch (ch) {
            case KEY_LEFT:
                setDirection(SNAKE_HEAD_DIRECTION::LEFT);
                break;
            case KEY_UP:
                setDirection(SNAKE_HEAD_DIRECTION::UP);
                break;
            case KEY_RIGHT:
                setDirection(SNAKE_HEAD_DIRECTION::RIGHT);
                break;
            case KEY_DOWN:
                setDirection(SNAKE_HEAD_DIRECTION::DOWN);
                break;
            case 'q':
            case 'Q':
                setGameStatus(GAME_STATUS::LOSE);
                break;
            default:
                break;
        }

        // Update the game state
        update();

        // Delay to control the game speed
        napms(TIMEOUT);
    }
}

int main() {
    srand(time(NULL));

    SnakeGame game;
    game.play();

    return 0;
}

