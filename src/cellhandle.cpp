#include "cellhandle.h"
#include "cells.h"
#include "constants.h"

XY CellHandle::pos_to_xy(int64_t pos) {
    uint64_t x = (pos % COLUMNS_X);
    uint64_t y = (pos / COLUMNS_X);

    return XY(x, y);
}


// copy-asign constructor
CellHandle& CellHandle::operator=(const CellHandle& rhs) {
    this->index = rhs.index;
    this->parent = rhs.parent; 
}

// copy constructor
CellHandle::CellHandle(const CellHandle& rhs) {
    index = rhs.index;
    parent = rhs.parent;
}

// copy 
CellHandle CellHandle::clone() {
    return CellHandle(*this);
}

// equality
inline bool CellHandle::operator==(const CellHandle& rhs) {
    return rhs.index == this->index;
}

constexpr inline bool CellHandle::equals(const CellHandle& rhs) {
    return *this == rhs;
}

inline void CellHandle::incrementRowWise() {
    this->index +=1;
}

inline void CellHandle::decrementRowWise() {
    this->index -=1;
}

inline CellHandle CellHandle::create(uint64_t cell_position, Cells *all_cells) {
    CellHandle handle;
    handle.index = cell_position;
    handle.parent = all_cells;
    return handle;
}

inline bool CellHandle::isPopulated() {
    return parent->cellIsPopulated(index);
}

inline bool CellHandle::hasSandPhysics() {
    return parent->cellHasSandPhysics(index);
}

inline bool CellHandle::hasLeft() {
    XY pos = pos_to_xy(index).subtractParts(XY(1, 0));
    return parent->isInRange(pos.x, pos.y);
}

inline CellHandle CellHandle::getLeft() {
    if (BOUNDS_ASSERTIONS) {
        if (!hasLeft()) {
            TODO();
        }
    }

    XY pos = pos_to_xy(index).subtractParts(XY(1, 0));
    return parent->getCell(pos.x, pos.y);

}

inline bool CellHandle::hasRight() {
    XY pos = pos_to_xy(index).addParts(XY(1, 0));
    return parent->isInRange(pos.x, pos.y);
}

inline CellHandle CellHandle::getRight() {
    if (BOUNDS_ASSERTIONS) {
        if (!hasRight()) {
            TODO();
        }
    }

    XY pos = pos_to_xy(index).addParts(XY(1, 0));
    return parent->getCell(pos.x, pos.y);
}

inline bool CellHandle::hasUp() {
    XY pos = pos_to_xy(index).subtractParts(XY(0, 1));
    return parent->isInRange(pos.x, pos.y);
}

inline CellHandle CellHandle::getUp() {
    if (BOUNDS_ASSERTIONS) {
        if (!hasUp()) {
            TODO();
        }
    }

    XY pos = pos_to_xy(index).subtractParts(XY(0, 1));
    return parent->getCell(pos.x, pos.y);
}

inline bool CellHandle::hasDown() {
    XY pos = pos_to_xy(index).addParts(XY(0, 1));
    return parent->isInRange(pos.x, pos.y);
}

inline CellHandle CellHandle::getDown() {
    if (BOUNDS_ASSERTIONS) {
        if (!hasDown()) {
            TODO();
        }
    }

    XY pos = pos_to_xy(index).addParts(XY(0, 1));
    return parent->getCell(pos.x, pos.y);
}

inline bool CellHandle::hasRightUp() {
    TODO();
    return false;
}

inline CellHandle CellHandle::getRightUp() {
    TODO();
    return CellHandle {};
}

inline bool CellHandle::hasRightDown() {
    TODO();
    return false;
}

inline CellHandle CellHandle::getRightDown() {
    TODO();
    return CellHandle {};
}

inline bool CellHandle::hasLeftUp() {
    TODO();
    return false;
}

inline CellHandle CellHandle::getLeftUp() {
    TODO();
    return CellHandle {};
}

inline bool CellHandle::hasLeftDown() {
    return false;
}

inline CellHandle CellHandle::getLeftDown() {
    TODO();
    return CellHandle {};
}

CellHandle Cells::getCell(uint64_t x, uint64_t y) {
    uint64_t pos = this->checked_xy_to_pos(XY(x,y));
    CellHandle handle = CellHandle::create(pos, this);

    return handle;
}
