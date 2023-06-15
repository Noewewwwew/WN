#include "Item.h"

Item::Item(ItemType type)
    : type(type), appeared(false), x(0), y(0), appearanceTime(0) {}

void Item::setRandomPosition(int maxX, int maxY) {
    x = rand() % maxX;
    y = rand() % maxY;
}

bool Item::isAppeared() const {
    return appeared;
}

void Item::updateAppearanceTime() {
    appearanceTime = std::time(nullptr);
}

Item::ItemType Item::getType() const {
    return type;
}

int Item::getX() const {
    return x;
}

int Item::getY() const {
    return y;
}
