#include "raylib.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <optional>

const int WIDTH = 1024;
const int HEIGHT = 768; 
const int COLUMNS_X = WIDTH;
const int ROWS_Y = HEIGHT;

const int BOUNDS_ASSERTIONS = true;

static void todoImpl(uint64_t line, const char* file) {
    std::cerr << file << ":" << line << " is todo" << std::endl;
    exit(1);
}


#define TODO() \
    todoImpl(__LINE__, __FILE__)



// static void setBit(uint8_t* val, int n, bool state) {
//     *val ^= *val & (1 << n);
//     *val |= state << n;
// }

// static bool getBit(uint8_t value, int index) {
//     return (value >> index) & 1;
// }






class Grid {
    

    private:
        /*
            Each cell is encoded in as bits. Each bit has a specific purpose
            | Offset    | Description                               | Name          |
            |-----------|-------------------------------------------|---------------|
            | 0         | true if the cell has a sand-like material | _SAND_FLAG    |
            | 1         | temporary boolean for use in simulation   | _MOVED_FLAG   |
        */
        typedef uint64_t Cell;
   
        static const int _LEN =  (COLUMNS_X * ROWS_Y);
        static const int _SAND_FLAG = (0);
        static const int _MOVED_FLAG = (1);


        std::vector<Cell> _cells = std::vector<Cell>();
        static const int _CELL_SIZE = sizeof(_cells[0]);

        
        inline static void setCellBit(uint64_t* val, int n, bool state) {
            *val ^= *val & (1 << n);
            *val |= state << n;
        }

        inline static bool getCellBit(uint64_t value, int n) {
            return (value >> n) & 1;
        }

        inline static bool rangeCheck(uint64_t x, uint64_t y) {
            if (BOUNDS_ASSERTIONS) {
                if (!isInRange(x,y)) {
                    std::cerr << "x: " << x << ", y: " << y << "\n";    
                    return false;
                }
            }

            return true;
        }

        inline Cell* getCell(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }

            uint64_t pos = (y * COLUMNS_X) + x;
            
            Cell* item_ptr = NULL;
            if (BOUNDS_ASSERTIONS) {
                auto& item = _cells.at(pos);
                item_ptr = (Cell*)&item;
            } else {
                auto& item = _cells[pos];
                item_ptr = (Cell*)&item;
            }
            return item_ptr;
        }
        
    public:
        inline Grid() {
            for (int i = 0; i < WIDTH * HEIGHT; i++) {
                _cells.push_back(0);
            }
        }

        // bounds checks
        inline static bool isInRange(uint64_t x, uint64_t y) {
            if (y >= ROWS_Y || x >= COLUMNS_X || y < 0 || x < 0) {
                return false;
            }
        
            return true;
        }
        
        inline static bool isInRangeVec2(Vector2 vec) {
            return isInRange(vec.x, vec.y);
        }

        // if the cell is a sand-like material
        inline bool cellHasSand(uint64_t x, uint64_t y){
            if (!rangeCheck(x,y)) {
                TODO();
            }
            auto * cell = getCell(x,y);
            bool material_flag = getCellBit(*cell, _SAND_FLAG);
            
            return material_flag == 1;
        }

        inline bool cellHasSandVec2(Vector2 pos) {
            if (!rangeCheck(pos.x,pos.y)) {
                TODO();
            }
            return cellHasSand(pos.x, pos.y);
        }

        inline void moveCellVec2(Vector2 from, Vector2 to) {
            if (!rangeCheck(from.x,from.y)) {
                TODO();
            }

            if (!rangeCheck(to.x,to.y)) {
                TODO();
            }

            Cell* from_cell =  getCell(from.x, from.y);
            Cell* to_cell = getCell(to.x, to.y);
            
            auto from_copy = *from_cell;
            *from_cell = 0;
            *to_cell = from_copy;

            setCellMoved(to.x, to.y);
        }

        // set the sand bit to true
        inline void setCellSand(uint64_t x, uint64_t y){
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            setCellBit(cell, _SAND_FLAG, true);
        }

        inline void resetCell(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            *cell = 0;
        }

        inline void setCellMoved(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            setCellBit(cell, _MOVED_FLAG, true);
        }

        // only call once per loop
        inline void setCellsNotMoved() {
            for (uint64_t y = 0; y < ROWS_Y; y++) {
                for (uint64_t x = 0; x < COLUMNS_X; x+=8) {
                    if (!rangeCheck(x,y) || !(rangeCheck(x+7, y))) {
                        TODO();
                    }

                    
                    Cell* eight_cells = getCell(x,y);

                    for (uint64_t i = 0; i < 8; i++) {
                        setCellBit(eight_cells + i, _MOVED_FLAG, false);
                    }
    
                }
            }
        }

        inline bool isCellMoved(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell*cell =getCell(x,y);
            bool moved = getCellBit(*cell, _MOVED_FLAG);
            
            return moved;
        }
};



static void simulateSand(Grid* grid) {
    sizeof(Grid);
    (*grid).setCellsNotMoved();

    // right to left, bottom to top
    // move cells down
    for (int64_t y = ROWS_Y-1; y >= 0; y--) {
        for (int64_t x = COLUMNS_X-1; x >=0; x--) {
            if (!(grid->cellHasSand(x,y))) {
                continue;
            }

            if (grid->isCellMoved(x,y)) {
                continue;
            }

            
            Vector2 bellow_pos;
            bellow_pos.x = (float)x;
            bellow_pos.y = (float)(y+1);
            
            Vector2 pos;
            pos.x = (float)x;
            pos.y = (float)y;

            bool is_room_bellow = false;
            if (grid->isInRangeVec2(bellow_pos)) {
                if (!grid->cellHasSandVec2(bellow_pos)) {
                    is_room_bellow = true;
                }
            }

            if (is_room_bellow) {
                grid->moveCellVec2(pos, bellow_pos);            
            }

        }
    }

    // right to left, top to bottom
    // move sand left
    for (int64_t y = ROWS_Y-1; y>=0; y--) {
        for (int64_t x = COLUMNS_X-1; x >=0; x--) {
            // skip empty cells
            if (!grid->cellHasSand(x,y)) {
                continue;
            }

            // skip moved cells
            if (grid->isCellMoved(x,y)) {
                continue;
            }
            
            // move left
            Vector2 pos;
            pos.x = x;
            pos.y = y;

            Vector2 left_pos;
            left_pos.x = x-1;
            left_pos.y = y+1;

            Vector2 right_pos;
            right_pos.x = x+1;
            right_pos.y = y+1;


            bool is_room_left = false;
            if (grid->isInRangeVec2(left_pos)) {
                if (!grid->cellHasSandVec2(left_pos)) {
                    is_room_left = true;
                }
            }

            bool is_room_right = false;
            if (grid->isInRangeVec2(right_pos)) {
                if (!grid->cellHasSandVec2(right_pos)) {
                    is_room_right = true;
                }
            }

            bool can_left = false;
            bool can_right = false;
            if (is_room_left && is_room_right) {
                int value = GetRandomValue(0,1);
                if (value == 0) {
                    can_left = true;
                    can_right = false;
                } else if (value == 1) {
                    can_left = false;
                    can_right = true;
                } else {
                    if (BOUNDS_ASSERTIONS) {
                        std::cerr << "Out of range " << value << "\n";
                        TODO();
                    }
                }
                
            } else {
                can_left = true;
                can_right = true;
            }

            if (is_room_left && can_left) {
                grid->moveCellVec2(pos, left_pos);
            }
        }
    }

    // left to right top to bottom
    // move sand right
    for (int64_t y=ROWS_Y-1; y>=0; y--) {
        for (int64_t x=0; x < COLUMNS_X; x++) {
            if (!grid->cellHasSand(x,y)) {
                continue;
            }
            if (grid->isCellMoved(x,y)) {
                continue;
            }

            Vector2 pos;
            pos.x = x;
            pos.y = y;

            Vector2 left_pos;
            left_pos.x = x-1;
            left_pos.y = y+1;

            Vector2 right_pos;
            right_pos.x = x+1;
            right_pos.y = y+1;

            bool is_room_left = false;
            if (grid->isInRangeVec2(left_pos)) {
                if (!grid->cellHasSandVec2(left_pos)) {
                    is_room_left = true;
                }
            }

            bool is_room_right = false;
            if (grid->isInRangeVec2(right_pos)) {
                if (!grid->cellHasSandVec2(right_pos)) {
                    is_room_right = true;
                }
            }

            // if the previous loop doesn't move the cell left, this one will move it right
            bool can_right = true;  
            if (is_room_right && can_right) {
                grid->moveCellVec2(pos, right_pos);
            }

        }
    }
}

static Texture2D genTexture2d() {
    auto image = GenImageColor(WIDTH, HEIGHT, WHITE);
    auto texture = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture;
}

static void updateTexture(Texture2D* texture, std::vector<int>* backing, Grid* grid) {
    for (int x = 0; x < COLUMNS_X; x++) {
        for (int y = 0; y < ROWS_Y; y++) {
            int* backing_ptr = NULL;
            if (BOUNDS_ASSERTIONS) {
                int& backing_ref = (*backing).at((y * COLUMNS_X) + x);
                backing_ptr = &backing_ref;
            } else {
                int& backing_ref = (*backing)[((y * COLUMNS_X) + x)];
                backing_ptr = &backing_ref;
            }
            
            
            bool has_sand = (*grid).cellHasSand(x,y);
            
            
            if (has_sand) {
                Color light_yellow = Color{0xFB,0xF1,0xB9, 255};
                *backing_ptr = *(int*)(Color*)(&light_yellow);
            } else {
                *backing_ptr = 0;
            }
        } 
    }

    UpdateTexture(*texture, backing->data());
}


static Vector2 mouseToGrid(Vector2 mousePos) {
    Vector2 gridPos;
    gridPos.x = mousePos.x *1.0;
    gridPos.y = mousePos.y *1.0;

    return gridPos;
}

static Vector2 clampGridPos(Vector2 pos) {
    if (pos.x >= COLUMNS_X) {
        pos.x = COLUMNS_X -1;
    }
    if (pos.y >= ROWS_Y) {
        pos.y = ROWS_Y -1;
    }

    if (pos.x < 0) {
        pos.x = 0;
    }
    if (pos.y < 0) {
        pos.y = 0;
    }

    return pos;
}



int main() {

    InitWindow(WIDTH, HEIGHT, "sand experiment 3");
    double time = GetTime();
    auto texture = genTexture2d();
    std::vector<int> texture_backing = std::vector<int>(COLUMNS_X * ROWS_Y);
    
    Grid grid = Grid();

    while (!WindowShouldClose())
    {
        double time_now = GetTime();
        if (time + 0.02 <= time_now) {
            time = time_now;
            simulateSand(&grid);
            updateTexture(&texture, &texture_backing, &grid);
        }


        // create new pixels that are clicked
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 position = GetMousePosition();
            

            Vector2 positionMax = position;
            positionMax.x += 12.5;
            positionMax.y += 12.5;
            positionMax = clampGridPos(mouseToGrid(positionMax));
            


            Vector2 positionMin = position;
            positionMin.x -=12.5;
            positionMin.y -=12.5;
            positionMin = clampGridPos(mouseToGrid(positionMin));
            

            for (double x = positionMin.x; x < positionMax.x; x++) {
                for (double y = positionMin.y; y < positionMax.y; y++) {
                    grid.setCellSand(x,y);
                }
            }
        }

        // draw
        BeginDrawing();
        {
            ClearBackground(BLACK);
            DrawFPS(10, 10);

            DrawTexture(texture , 0, 0, Color{0xFF,0xFF,0xFF,0xFF});
            
        }
        EndDrawing();
    }
    
    UnloadTexture(texture);

}