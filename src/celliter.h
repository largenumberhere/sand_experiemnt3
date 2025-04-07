#pragma once

#include <cstdint>
#include <iterator>
#include "forward.h"
#include "cellhandle.h"

enum  IterDirection : uint8_t {
    DIRECTION_ITER_ROWS,
};

struct CellIter {
    // tag dispatch mechanism from before C++17  
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = CellHandle;
    using pointer = CellHandle;
    using refrence = CellHandle;

    CellIter(pointer start, IterDirection direction) : _ptr(start), _direction(direction) {} 

    // derefrence
    refrence operator *() const;
    pointer operator ->();

    // prefix operators
    CellIter& operator ++();

    CellIter& operator --();

    // postfix operators
    CellIter operator++(int);
    CellIter operator--(int);

    // equality
    //friend bool operator == (const CellIter& a, const CellIter& b);
    //friend bool operator != (const CellIter& a, const CellIter& b);
    // equality
    friend bool operator == (const CellIter& a, const CellIter& b) {return a._ptr.equals(b._ptr);}
    friend bool operator != (const CellIter& a, const CellIter& b) {return !(a._ptr.equals(b._ptr));}

    private: 
        pointer _ptr;
        IterDirection _direction;
};
