#ifndef ITEM_H
#define ITEM_H

#include <cstdlib>
#include <ctime>

class Item {
public:
    enum class ItemType {
        GROWTH,
        POISON
    };

    Item(ItemType type);

    void setRandomPosition(int maxX, int maxY);

    bool isAppeared() const;

    void updateAppearanceTime();

    ItemType getType() const;

    int getX() const;

    int getY() const;

private:
    ItemType type;
    bool appeared;
    int x;
    int y;
    time_t appearanceTime;
};

#endif // ITEM_H
