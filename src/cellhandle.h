#pragma once

#include "forward.h"
#include "xy.h"

class CellHandle {
    private:
        int64_t index;
        Cells* parent;

        CellHandle() {}

        XY pos_to_xy(int64_t pos);

    public:
        // copy-asign constructor
        CellHandle& operator=(const CellHandle& rhs);

        // copy constructor
        CellHandle(const CellHandle& rhs);

        // copy 
        CellHandle clone();
        // equality
        inline bool operator==(const CellHandle& rhs) const;

        const inline bool equals(const CellHandle& rhs) const;

        void incrementRowWise();
        void decrementRowWise();

        static inline CellHandle create(uint64_t cell_position, Cells *all_cells);
        inline bool isPopulated();
        inline bool hasSandPhysics();
        inline bool hasLeft();        
        inline CellHandle getLeft();
        inline bool hasRight();        
        inline CellHandle getRight();
        inline bool hasUp();        
        inline CellHandle getUp();
        inline bool hasDown();        
        inline CellHandle getDown();
        inline bool hasRightUp();        
        inline CellHandle getRightUp();
        inline bool hasRightDown();
        inline CellHandle getRightDown();
        inline bool hasLeftUp();
        inline CellHandle getLeftUp();
        inline bool hasLeftDown();
        inline CellHandle getLeftDown();
};
