#include "xy.h"

XY::XY(uint64_t x, uint64_t y) {
    this->x = x;
    this->y = y;
}

XY XY::subtractParts(XY pos) {
    uint64_t newx = x - pos.x;
    uint64_t newy = y - pos.y;

    return XY(newx, newy);
}

XY XY::addParts(XY pos) {
    uint64_t newx = x + pos.x;
    uint64_t newy = y + pos.y;

    return XY(newx, newy);
}
