#include "cells.h"
#include "constants.h"

inline bool Cells::rangeCheck(uint64_t x, uint64_t y) {
    if (BOUNDS_ASSERTIONS) {
        if (!isInRange(x,y)) {
            std::cerr << "x: " << x << ", y: " << y << "\n";    
            return false;
        }
    }

    return true;
}

uint64_t Cells::checked_xy_to_pos(XY coords) {
    if (!rangeCheck(coords.x,coords.y)) {
        TODO();
    }

    uint64_t pos = (coords.y * COLUMNS_X) + coords.x;

    return pos;
}

Cells::Cells() {
    cellsHaveSandPhysics = std::vector<bool>(GRID_WIDTH * GRID_HEIGHT);
    cellsArePopulated = std::vector<bool>(GRID_WIDTH * GRID_HEIGHT);
}

inline bool Cells::isInRange(uint64_t x, uint64_t y) {
    if (y >= ROWS_Y || x >= COLUMNS_X || y < 0 || x < 0) {
        return false;
    }

    return true;
}

bool Cells::cellHasSandPhysics(uint64_t cell_index) {
    bool result = false;
    if (BOUNDS_ASSERTIONS) {
        result = cellsHaveSandPhysics.at(cell_index);
    } else {
        result = cellsHaveSandPhysics[cell_index];
    }

    return result;
}

bool Cells::cellIsPopulated(uint64_t cell_index) {
    bool result = false;
    if (BOUNDS_ASSERTIONS) {
        result = cellsArePopulated.at(cell_index);
    } else {
        result = cellsArePopulated[cell_index];
    }

    return result;
}
