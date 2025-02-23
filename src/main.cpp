#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <cstdint>
#include <iostream>
#include <vector>
#include <optional>

const int WIDTH = 1024;
const int HEIGHT = 768; 
const int COLUMNS_X = WIDTH;
const int ROWS_Y = HEIGHT;

const int BOUNDS_ASSERTIONS = false;

static void todoImpl(uint64_t line, const char* file) {
    std::cerr << file << ":" << line << " is todo" << std::endl;
    exit(1);
}


inline static void setBit(uint8_t* val, int n, bool state) {
    *val ^= *val & (1 << n);
    *val |= state << n;
}

inline static bool getBit(uint8_t value, int n) {
    return (value >> n) & 1;
}




#define TODO() \
    todoImpl(__LINE__, __FILE__)


class Grid {
    

    private:
        /*
            Each cell is encoded in as bits. Each bit has a specific purpose
            | Offset    | Name                  |
            |-----------|-----------------------|
            | 0         | _SAND_FLAG            |               
            | 1         | _MOVED_FLAG           |
            | 2         |                       |
            | 3         |                       |
            | 4         |                       |
        
            */
        typedef uint64_t Cell;

        static const int _LEN =  (COLUMNS_X * ROWS_Y);
        static const int _SAND_PHYSICS_FLAG = (0);
        static const int _MOVED_FLAG = (1);
        static const int _WATER_PHYSICS_FLAG = (2);
        static const int _SAND_TYPE = (3);
        static const int _WATER_TYPE = (4);


        std::vector<Cell> _cells = std::vector<Cell>();
        static const int _CELL_SIZE = sizeof(_cells[0]);

        inline static void setCellBit(Cell* val, int n, bool state) {
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
        
        inline static bool isInRangeV(Vector2 vec) {
            return isInRange(vec.x, vec.y);
        }

        // if the cell is a sand-like material
        inline bool cellHasSandPhysics(uint64_t x, uint64_t y){
            if (!rangeCheck(x,y)) {
                TODO();
            }
            auto * cell = getCell(x,y);
            bool material_flag = getCellBit(*cell, _SAND_PHYSICS_FLAG);
            
            return material_flag == 1;
        }

        inline bool cellHasSandPhysicsV(Vector2 pos) {
            if (!rangeCheck(pos.x,pos.y)) {
                TODO();
            }
            return cellHasSandPhysics(pos.x, pos.y);
        }

        inline void cellMoveV(Vector2 from, Vector2 to) {
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

            cellSetMoved(to.x, to.y);
        }

        inline void cellSwapV(Vector2 from, Vector2 to) {
            if (!rangeCheck(from.x, to.y)) {
                TODO();
            }

            if (!rangeCheck(from.x, to.y)) {
                TODO();
            }

            Cell* cell_a = getCell(from.x, from.y);
            Cell* cell_b = getCell(to.x, to.y);

            // the actual swap
            Cell cell_b_copy = (*cell_b);
            *cell_b = *cell_a;
            *cell_a = cell_b_copy;

            // cellSetMoved(from.x, from.y);
            // cellSetMoved(b.x, b.y);
        }

        // set the sand bit to true
        inline void cellSetSandPhysics(uint64_t x, uint64_t y){
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            setCellBit(cell, _SAND_PHYSICS_FLAG, true);
        }

        inline void cellSetWaterPhysics(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            setCellBit(cell, _SAND_PHYSICS_FLAG, true);
            setCellBit(cell, _WATER_PHYSICS_FLAG, true);
        }

        inline void cellSetWaterV(Vector2 pos) {
            if (!rangeCheck(pos.x,pos.y)) {
                TODO();
            }
            cellReset(pos.x, pos.y);
            Cell* cell = getCell(pos.x,pos.y);

            cellSetWaterPhysics(pos.x, pos.y);
            setCellBit(cell, _WATER_TYPE, true);
        }

        inline void cellSetSandV(Vector2 pos) {
            cellReset(pos.x, pos.y);
            cellSetSandPhysics(pos.x, pos.y);
            Cell* cell = getCell(pos.x, pos.y);
            setCellBit(cell, _SAND_TYPE, true);
        }

        inline bool cellHasWater(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            return getCellBit(*cell, _WATER_TYPE);
        }

        inline bool cellHasWaterV(Vector2 pos) {
            return cellHasWater(pos.x, pos.y);
        }

        inline bool cellHasSand(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            return getCellBit(*cell, _SAND_TYPE);
        }

        inline bool cellHasSandV(Vector2 pos) {
            return cellHasSand(pos.x, pos.y);
        }

        inline bool cellHasMaterial(uint64_t x, uint64_t y) {
            return cellHasWater(x,y) || cellHasSand(x,y);
        }

        inline bool cellHasMaterialV(Vector2 pos) {
            int x = pos.x;
            int y = pos.y;
            return cellHasWater(x,y) || cellHasSand(x,y);
        }


        inline void cellReset(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            *cell = 0;
        }

        inline void cellSetMoved(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell* cell = getCell(x,y);
            setCellBit(cell, _MOVED_FLAG, true);
        }

        // only call once per loop
        inline void clearTempMovement() {
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

        inline bool cellIsMoved(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            Cell*cell =getCell(x,y);
            bool moved = getCellBit(*cell, _MOVED_FLAG);
            
            return moved;
        }

        inline int64_t cellDensityV(Vector2 pos) {
            // kg / m3

            int density = -1;
            
            if (cellHasSandV(pos)) {
                density = 1420;
            } else if (cellHasWaterV(pos)) {
                density = 998;
            } else {
                density = -1;
            }

            if (density == -1) {
                TODO();
            }

            return density;
        }
};



static void simulateGrid(Grid* grid) {
    sizeof(Grid);
    (*grid).clearTempMovement();

    // bottom to top, left to right
    // density swapping
    for (int64_t y = ROWS_Y-1; y >= 0; y--) {
        for (int64_t x = 0; x < COLUMNS_X; x++) {
            Vector2 pos;
            pos.x = x;
            pos.y = y;
            
            Vector2 pos_up;
            pos_up.x = x;
            pos_up.y = y-1;

            if (grid->cellHasMaterialV(pos)) {
                if (grid->isInRangeV(pos_up) && grid->cellHasMaterialV(pos_up)) {
                    int64_t density = grid->cellDensityV(pos);
                    int64_t density_up = grid->cellDensityV(pos_up);

                    if (density_up > density) {
                        grid->cellSwapV(pos, pos_up);
                    }            
                }
            }
        }
    }

    // right to left, bottom to top
    // for (int64_t y = ROWS_Y-1; y >= 0; y--) {
    //     for (int64_t x = COLUMNS_X-1; x >=0; x--) {
    //         if (!grid->isInRange(x,y)) {
    //             TODO();
    //         }

    //         Vector2 pos;
    //         pos.x = x;
    //         pos.y = y;
            
    //         Vector2 right_pos;
    //         right_pos.x = x+1;
    //         right_pos.y = y;
    //         if (!grid->isInRangeV(right_pos)) {
    //             continue;
    //         }

    //         Vector2 bottom_right_pos;
    //         bottom_right_pos.x = x+1;
    //         bottom_right_pos.y = y+1;
    //         if(!grid->isInRangeV(bottom_right_pos)) {
    //             bottom_right_pos = right_pos;
    //         }

    //         if (grid->cellHasWaterV(pos) && grid->cellHasMaterialV(right_pos) && grid->cellHasMaterialV(bottom_right_pos)) {
    //             grid->cellMoveV(pos, right_pos);
    //         }

    //     }
    // }


    // right to left, bottom to top
    for (int64_t y = ROWS_Y-1; y >= 0; y--) {
        for (int64_t x = COLUMNS_X-1; x >=0; x--) {
            
            /*
                [ ]
                 v
                [ ]     
                move cells down
            */
            if ((grid->cellHasSandPhysics(x,y)) && ! grid->cellIsMoved(x,y)) {
                Vector2 bellow_pos;
                bellow_pos.x = (float)x;
                bellow_pos.y = (float)(y+1);
                
                Vector2 pos;
                pos.x = (float)x;
                pos.y = (float)y;

                bool is_room_bellow = false;
                if (grid->isInRangeV(bellow_pos)) {
                    if (!grid->cellHasMaterialV(bellow_pos)) {
                        is_room_bellow = true;
                    }
                }

                if (is_room_bellow) {
                    grid->cellMoveV(pos, bellow_pos);            
                }
            }
        }
    }

    // right to left, top to bottom
    for (int64_t y = ROWS_Y-1; y>=0; y--) {
        for (int64_t x = COLUMNS_X-1; x >=0; x--) {
            /*
                move diagonal left
                
                    <- [ ]
                [ ]
            */
            if (grid->cellHasSandPhysics(x,y) && !grid->cellIsMoved(x,y)) {
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
                if (grid->isInRangeV(left_pos)) {
                    if (!grid->cellHasMaterialV(left_pos)) {
                        is_room_left = true;
                    }
                }

                bool is_room_right = false;
                if (grid->isInRangeV(right_pos)) {
                    if (!grid->cellHasMaterialV(right_pos)) {
                        is_room_right = true;
                    }
                }

                bool can_left = false;
                bool can_right = false;
                if (is_room_left && is_room_right) {
                    int value = -1;
                    
                    value = GetRandomValue(0,1);

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
                    grid->cellMoveV(pos, left_pos);
                }
            }
        }
    }



    // left to right top to bottom
    for (int64_t y=ROWS_Y-1; y>=0; y--) {
  
        for (int64_t x=0; x < COLUMNS_X; x++) {

        //     move diagonal right
                
        // [ ] ->
        //       [ ]
            if (grid->cellHasSandPhysics(x,y) && !grid->cellIsMoved(x,y)) {
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
                if (grid->isInRangeV(left_pos)) {
                    if (!grid->cellHasMaterialV(left_pos)) {
                        is_room_left = true;
                    }
                }

                bool is_room_right = false;
                if (grid->isInRangeV(right_pos)) {
                    if (!grid->cellHasMaterialV(right_pos)) {
                        is_room_right = true;
                    }
                }

                // if the previous loop doesn't move the cell left, this one will move it right
                bool can_right = true;  
                if (is_room_right && can_right) {
                    grid->cellMoveV(pos, right_pos);
                }
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
            
            
            if (grid->cellHasSand(x,y)) {
                Color light_yellow = Color{0xFB,0xF1,0xB9, 255};
                *backing_ptr = *(int*)(Color*)(&light_yellow);
            } else if (grid->cellHasWater(x,y)) {
                Color blue = Color{0x00,0x6E,0xE6, 255};
                *backing_ptr = *(int*)(Color*)(&blue);
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

enum CursorMode : uint8_t{
    CUR_SAND,
    CUR_WATER,
};

int main() {

    InitWindow(WIDTH, HEIGHT, "sand experiment 3");
    double time = GetTime();
    auto texture = genTexture2d();
    std::vector<int> texture_backing = std::vector<int>(COLUMNS_X * ROWS_Y);
    
    Grid grid = Grid();
    CursorMode cursor_mode = CUR_SAND;

    while (!WindowShouldClose())
    {
        double time_now = GetTime();
        if (time + 0.02 <= time_now) {
            time = time_now;
            simulateGrid(&grid);
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
                    if (!grid.cellHasMaterial(x, y)) {   
                        if (cursor_mode == CUR_SAND) {
                            grid.cellSetSandV(Vector2{(float)x, (float)y});
                        } else if (cursor_mode == CUR_WATER) {
                            grid.cellSetWaterV(Vector2{(float)x, (float)y});
                        }
                    }
                }
            }
        }


        // draw
        BeginDrawing();
        {
            ClearBackground(BLACK);
            if (GuiButton(Rectangle{0,0, 50, 20}, "Water")) {
                cursor_mode = CUR_WATER;
            } else if(GuiButton(Rectangle{60, 0, 50, 20}, "Sand")) {
                cursor_mode = CUR_SAND;
            }

            DrawFPS(10, 10);
            DrawTexture(texture , 0, 0, Color{0xFF,0xFF,0xFF,0xFF});
      
        }
        EndDrawing();
    }
    
    UnloadTexture(texture);

}