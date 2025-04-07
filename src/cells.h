#pragma once

#include <vector>
#include "forward.h"
#include "xy.h"

class Cells {
    private:
        std::vector<bool> cellsHaveSandPhysics = std::vector<bool>();
        std::vector<bool> cellsArePopulated = std::vector<bool>();

        inline static bool rangeCheck(uint64_t x, uint64_t y);
        uint64_t checked_xy_to_pos(XY coords);

    public:
        Cells();
        
        CellHandle getCell(uint64_t x, uint64_t y);
        static bool isInRange(uint64_t x, uint64_t y);
        bool cellHasSandPhysics(uint64_t cell_index);
        bool cellIsPopulated(uint64_t cell_index);
};
