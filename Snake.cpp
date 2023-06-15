#include "Snake.h"

Snake::Snake(const int& y, const int& x): direction(SNAKE_HEAD_DIRECTION::LEFT) {
    this->snake.push_back(pos(y, x));
    this->snake.push_back(pos(y, x + 1));
    this->snake.push_back(pos(y, x + 2));
    this->snake.push_back(pos(y, x + 3));
}

void Snake::set_head_direction(int direction){
    this->direction = direction;
}

int Snake::get_head_direction(){
    return this->direction;
}

void Snake::grow(){
    this->grow(this->new_head());
}

void Snake::grow(const pos new_pos){
    this->snake.push_front(new_pos);
}

void Snake::shrink(){
    this->snake.pop_back();
}

const pos& Snake::head() {
    return this->snake.front();
}

const pos& Snake::tail() {
    return this->snake.back();
}

const pos Snake::new_head(){
    const pos& head = this->snake.front();
    return pos(head.Y + dPos[this->direction].Y, head.X + dPos[this->direction].X);
}

void Snake::move_head(const pos new_pos){
    this->snake.front() = new_pos;
}

int Snake::get_snake_length(){
    return this->snake.size();
}

void Snake::reverse(){
    deque<pos>(this->snake.rbegin(), this->snake.rend()).swap(this->snake); 
}
