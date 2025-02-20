#include "raylib.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <optional>

const int WIDTH = 1024;
const int HEIGHT = 768; 
const int COLUMNS_X = WIDTH;
const int ROWS_Y = HEIGHT;

static void todoImpl(uint64_t line, const char* file) {
    std::cerr << file << ":" << line << " is todo" << std::endl;
    exit(1);
}


#define TODO() \
    todoImpl(__LINE__, __FILE__)



static void setBit(uint8_t* val, int n, bool state) {
    *val ^= *val & (1 << n);
    *val |= state << n;
}

static bool getBit(uint8_t value, int index) {
    return (value >> index) & 1;
}


static bool isInRange(uint64_t x, uint64_t y) {
    if (y >= ROWS_Y || x >= COLUMNS_X || y < 0 || x < 0) {
        return false;
    }

    return true;
}

static bool isInRangeVec2(Vector2 vec) {
    return isInRange(vec.x, vec.y);
}



class Grid {


    private:
   
        static const int _LEN =  (COLUMNS_X * ROWS_Y);
        static const int _MATERIAL_FLAG = (0);
        static const int _MOVED_FLAG = (1);
        /*
            Each cell is encoded in uin8_t, this has 8 bits. Each bit has a specific purpose
            | offset    | description                               | name          |
            |-----------|-------------------------------------------|---------------|
            | 0         | true if the cell has a sand-like material | material      |
            | 1         | temporary boolean for use in simulation   | moved flag    |
        */


        std::vector<uint8_t> _cells = std::vector<uint8_t>();
        
        bool rangeCheck(uint64_t x, uint64_t y) {
            if (!isInRange(x,y)) {
                std::cerr << "x: " << x << ", y: " << y << "";    
                return false;
            }

            return true;
        }

        uint8_t* getCell(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }

            uint64_t pos = (y * COLUMNS_X) + x;
            uint8_t& item = _cells.at(pos);
            return (uint8_t*) &item;
        }
        
    public:
        Grid() {
            for (int i = 0; i < WIDTH * HEIGHT; i++) {
                _cells.push_back(0);
            }
        }

        bool cellHasMaterial(uint64_t x, uint64_t y){
            if (!rangeCheck(x,y)) {
                TODO();
            }
            auto * cell = getCell(x,y);
            bool material_flag = getBit(*cell, _MATERIAL_FLAG);
            
            return material_flag == 1;
        }

        bool cellHasMaterialVec2(Vector2 pos) {
            if (!rangeCheck(pos.x,pos.y)) {
                TODO();
            }
            return cellHasMaterial(pos.x, pos.y);
        }

        void moveCellVec2(Vector2 from, Vector2 to) {
            if (!rangeCheck(from.x,from.y)) {
                TODO();
            }

            if (!rangeCheck(to.x,to.y)) {
                TODO();
            }

            auto from_cell =  getCell(from.x, from.y);
            auto to_cell = getCell(to.x, to.y);
            
            auto from_copy = *from_cell;
            *from_cell = 0;
            *to_cell = from_copy;

            setCellMoved(to.x, to.y);
        }

        
        void setCellMaterial(uint64_t x, uint64_t y){
            if (!rangeCheck(x,y)) {
                TODO();
            }
            uint8_t* cell = getCell(x,y);
            setBit(cell, _MATERIAL_FLAG, true);
        }

        void resetCell(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            uint8_t* cell = getCell(x,y);
            *cell = 0x00;
        }

        void setCellMoved(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            uint8_t* cell = getCell(x,y);
            setBit(cell, _MOVED_FLAG, true);
        }

        void setCellsNotMoved() {
            for (uint64_t y = 0; y < ROWS_Y; y++) {
                for (uint64_t x = 0; x < COLUMNS_X; x++) {
                    if (!rangeCheck(x,y)) {
                        TODO();
                    }
                    auto* cell = getCell(x,y);
                    setBit(cell, _MOVED_FLAG, false);
                }
            }
        }

        bool isCellMoved(uint64_t x, uint64_t y) {
            if (!rangeCheck(x,y)) {
                TODO();
            }
            uint8_t*cell =getCell(x,y);
            bool moved = getBit(*cell, _MOVED_FLAG);
            
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
            if (!(grid->cellHasMaterial(x,y))) {
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
            if (isInRangeVec2(bellow_pos)) {
                if (!grid->cellHasMaterialVec2(bellow_pos)) {
                    is_room_bellow = true;
                }
            }

            if (is_room_bellow) {
                grid->moveCellVec2(pos, bellow_pos);
                if (!grid->cellHasMaterialVec2(bellow_pos)) {
                    TODO();
                }

                
            }

        }
    }
}

static Texture2D genTexture2d() {
    auto image = GenImageColor(WIDTH, HEIGHT,RED);
    auto texture = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture;
}

static void updateTexture(Texture2D* texture, std::vector<int>* backing, Grid* grid) {
    for (int x = 0; x < COLUMNS_X; x++) {
        for (int y = 0; y < ROWS_Y; y++) {
            int& backing_ref = (*backing).at((y * COLUMNS_X) + x);
            
            bool has_material = (*grid).cellHasMaterial(x,y);

            if (has_material) {
                backing_ref = INT32_MAX;
            } else {
                backing_ref = 0;
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
                    grid.setCellMaterial(x,y);
                }
            }
        }

        // draw
        BeginDrawing();
        {
            ClearBackground(BLACK);
            DrawFPS(10, 10);

            DrawTexture(texture , 0, 0, RED);
            
        }
        EndDrawing();
    }
    
    UnloadTexture(texture);

}