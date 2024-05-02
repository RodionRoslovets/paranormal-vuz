#include "item.h"

struct Point {
    int x;
    int y;
};

struct FurnitureItem{
    Point coords;
    Item item;
};

struct ItemImages{
    const char* normal;
    const char* fallen;
};