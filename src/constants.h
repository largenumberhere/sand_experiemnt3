#pragma once

#include <iostream>

const int GRID_WIDTH = 1024;
const int GRID_HEIGHT = 768;
const int COLUMNS_X = GRID_WIDTH;
const int ROWS_Y = GRID_HEIGHT;

const int DEFAULT_WIDTH = 800;
const int DEFAULT_HEIGHT = 600; 


const int BOUNDS_ASSERTIONS = true;

static void todoImpl(uint64_t line, const char* file) {
    std::cerr << file << ":" << line << " is todo" << std::endl;
    exit(1);
}

#define TODO() \
    todoImpl(__LINE__, __FILE__)
