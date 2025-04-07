#pragma once

#include <cstdint>

class XY {
    public :
        uint64_t x;
        uint64_t y;

        XY(uint64_t x, uint64_t y);
        XY subtractParts(XY pos);
        XY addParts(XY pos);
};
