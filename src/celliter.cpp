#include "celliter.h"
#include "cellhandle.h"
#include "constants.h"

// tag dispatch mechanism from before C++17  
using iterator_category = std::bidirectional_iterator_tag;
using difference_type = std::ptrdiff_t;
using value_type = CellHandle;
using pointer = CellHandle;
using refrence = CellHandle;


// derefrence
refrence CellIter::operator *() const {return _ptr; }
pointer CellIter::operator ->() {return _ptr;}

// prefix operators
CellIter& CellIter::operator ++() {
    if (_direction == DIRECTION_ITER_ROWS) {
        _ptr.incrementRowWise();
    } else {
        TODO();
    }
    return *this;
}

CellIter& CellIter::operator --() {
    if (_direction == DIRECTION_ITER_ROWS) {
        _ptr.decrementRowWise();
    } else {
        TODO();
    }
    _ptr.decrementRowWise(); 
    return *this;
}

// postfix operators
CellIter CellIter::operator++(int) {CellIter tmp = *this; ++(*this); return tmp;}
CellIter CellIter::operator--(int) {CellIter tmp = *this; --(*this); return tmp;}



