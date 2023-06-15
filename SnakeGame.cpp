#include <ncurses/ncurses.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <deque>
#include "SnakeGame.h"

using namespace std;

SnakeGame::SnakeGame() {
    init();
}

void generateMap(int map[MAP_SIZE][MAP_SIZE]) {
    // Generate a border around the map
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1) {
                map[i][j] = ELEMENT_KIND::WALL;
            }
            else {
                map[i][j] = ELEMENT_KIND::BOARD;
            }
        }
    }
}
 
pos findValidPortalPosition(int map[MAP_SIZE][MAP_SIZE]) {
    pos portalPosition;
    // Find a valid position for the portal
    do {
        portalPosition = {rand() % (MAP_SIZE - 2) + 1, rand() % (MAP_SIZE - 2) + 1};
    } while (map[portalPosition.Y][portalPosition.X] != ELEMENT_KIND::BOARD);

    return portalPosition;
}

void SnakeGame::init() {
    // Initialize the map
    generateMap(this->map);

    // Initialize the snake
    this->snake.clear();
    int snakeStartX = MAP_SIZE / 2;
    int snakeStartY = MAP_SIZE / 2;
    this->snake.push_back({snakeStartY, snakeStartX});
    this->map[snakeStartY][snakeStartX] = ELEMENT_KIND::SNAKE_HEAD;

    // Create initial items
    createItems();
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

void SnakeGame::draw() {
    // Clear the screen
    clear();

    // Draw the map
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            switch (map[i][j]) {
                case ELEMENT_KIND::BOARD:
                    // Print a blank space for the board
                    printw(" ");
                    break;
                case ELEMENT_KIND::IMMU_WALL:
                case ELEMENT_KIND::WALL:
                    // Print a wall character for walls
                    printw("#");
                    break;
                case ELEMENT_KIND::PORTAL:
                    // Print a portal character for the portal
                    printw("@");
                    break;
                case ELEMENT_KIND::SNAKE_HEAD:
                    // Print the snake head character for the snake head
                    printw("O");
                    break;
                case ELEMENT_KIND::SNAKE_BODY:
                    // Print the snake body character for the snake body
                    printw("o");
                    break;
                case ELEMENT_KIND::GROWTH_ITEM:
                    // Print a growth item character for growth items
                    printw("+");
                    break;
                case ELEMENT_KIND::POISON_ITEM:
                    // Print a poison item character for poison items
                    printw("-");
                    break;
                default:
                    break;
            }
        }
        printw("\n");
    }
    refresh();
}

void SnakeGame::update() {
    // 게임이 종료되었는지 확인합니다.
    if (snake.size() == (MAP_SIZE - 2) * (MAP_SIZE - 2)) {
        // 뱀이 맵을 모두 채우면 플레이어 승리입니다.
        setGameStatus(GAME_STATUS::WIN);
        return;
    }

    // 현재 뱀 머리의 위치를 가져옵니다.
    pos currentHead = snake.back();

    // 현재 방향에 따라 뱀 머리의 새 위치를 계산합니다.
    int newHeadY = currentHead.Y + dPos[snake_direction][0];
    int newHeadX = currentHead.X + dPos[snake_direction][1];

    // 새 위치가 유효한지 확인합니다.
    if (newHeadY >= 1 && newHeadY < MAP_SIZE - 1 && newHeadX >= 1 && newHeadX < MAP_SIZE - 1) {
        int& newHeadElement = map[newHeadY][newHeadX];

        // 뱀이 벽이나 자신에게 충돌하는지 확인합니다.
        if (newHeadElement == ELEMENT_KIND::WALL || newHeadElement == ELEMENT_KIND::SNAKE_BODY) {
            setGameStatus(GAME_STATUS::LOSE);
            return;
        }

        // 뱀이 성장 아이템이나 독 아이템에 충돌하는지 확인합니다.
        if (newHeadElement == ELEMENT_KIND::GROWTH_ITEM || newHeadElement == ELEMENT_KIND::POISON_ITEM) {
            // 아이템의 효과를 처리합니다.
            handleItem({newHeadY, newHeadX}, newHeadElement);
            newHeadElement = ELEMENT_KIND::SNAKE_HEAD;
        }
        else {
            // 뱀 머리를 새 위치로 이동시킵니다.
            newHeadElement = ELEMENT_KIND::SNAKE_HEAD;

            // 뱀 꼬리를 제거합니다.
            map[snake.front().Y][snake.front().X] = ELEMENT_KIND::BOARD;
            snake.pop_front();
        }

        // 뱀에 새 머리를 추가합니다.
        snake.push_back({newHeadY, newHeadX});
        
        // 뱀이 포탈에 충돌하는지 확인합니다.
        if (newHeadElement == ELEMENT_KIND::PORTAL) {
            // 새로운 유효한 포탈 위치를 찾습니다.
            pos portalPosition = findValidPortalPosition(map);
            map[portalPosition.Y][portalPosition.X] = ELEMENT_KIND::PORTAL;
        }
        
        // 랜덤하게 아이템을 출현시킵니다.
        if (newHeadElement == ELEMENT_KIND::BOARD) {
            // 아이템 출현 여부를 결정합니다.
            if (rand() % ITEM_APPEAR_PROBABILITY == 0) {
                // 랜덤하게 아이템 종류를 선택합니다.
                int itemKind = rand() % 2;

                // 빈 공간 중에서 아이템을 출현시킬 위치를 선택합니다.
                pos itemPosition = findRandomEmptySpace(map);
                if (itemPosition.Y != -1 && itemPosition.X != -1) {
                    map[itemPosition.Y][itemPosition.X] = (itemKind == 0) ? ELEMENT_KIND::GROWTH_ITEM : ELEMENT_KIND::POISON_ITEM;
                }
            }
        }
    }
    else {
        
        setGameStatus(GAME_STATUS::LOSE);
        return;
    }
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

    // Clean up ncurses
    endwin();

    // Print game over message
    printGameOver();
}

void printGameOver() {
    cout << "Game Over!" << endl;
}

int main() {
    srand(time(NULL));

    SnakeGame game;
    game.play();

    return 0;
}
