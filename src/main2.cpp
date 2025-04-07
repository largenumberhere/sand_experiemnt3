#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdint.h>
#include <iostream>
#include <vector>

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

void initWindow() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "sand experiment 3");
    BeginDrawing();
    {
        ClearBackground(BLACK);
        DrawText("Loading sand...", 10, 10, 20, RAYWHITE);
    }
    EndDrawing();
}

const int COLUMN_WIDTH = 800;
const int ROW_HEIGHT = 600;

class XY {
    public :

        uint64_t x;
        uint64_t y;

        XY(uint64_t x, uint64_t y) {
            this->x = x;
            this->y = y;
        }

        XY subtractParts(XY pos) {
            uint64_t newx = x - pos.x;
            uint64_t newy = y - pos.y;

            return XY(newx, newy);
        }

        XY addParts(XY pos) {
            uint64_t newx = x + pos.x;
            uint64_t newy = y + pos.y;

            return XY(newx, newy);
        }
};


enum  IterDirection : uint8_t {
    DIRECTION_ITER_ROWS,
};
class CellHandle;

struct CellIter {
    // tag dispatch mechanism from before C++17  
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = CellHandle;
    using pointer = CellHandle;
    using refrence = CellHandle;

    CellIter(pointer start, IterDirection direction) : _ptr(start), _direction(direction) {} 

    // derefrence
    refrence operator *() const {return _ptr; }
    pointer operator ->() {return _ptr;}

    // prefix operators
    CellIter& operator ++() {
        if (_direction == DIRECTION_ITER_ROWS) {
            _ptr.incrementRowWise();
        } else {
            TODO();
        }
        return *this;
    }

    CellIter& operator --() {
        if (_direction == DIRECTION_ITER_ROWS) {
            _ptr.decrementRowWise();
        } else {
            TODO();
        }
        _ptr.decrementRowWise(); 
        return *this;
    }

    // postfix operators
    CellIter operator++(int) {CellIter tmp = *this; ++(*this); return tmp;}
    CellIter operator--(int) {CellIter tmp = *this; --(*this); return tmp;}

    // equality
    friend bool operator == (const CellIter& a, const CellIter& b) {return a._ptr.equals(b._ptr);}
    friend bool operator != (const CellIter& a, const CellIter& b) {return !(a._ptr.equals(b._ptr));}


    private: 
        pointer _ptr;
        IterDirection _direction;
};

class Cells {
    private:
        std::vector<bool> cellsHaveSandPhysics = std::vector<bool>();
        std::vector<bool> cellsArePopulated = std::vector<bool>();



        inline static bool rangeCheck(uint64_t x, uint64_t y) {
            if (BOUNDS_ASSERTIONS) {
                if (!isInRange(x,y)) {
                    std::cerr << "x: " << x << ", y: " << y << "\n";    
                    return false;
                }
            }

            return true;
        }

        uint64_t checked_xy_to_pos(XY coords) {
            if (!rangeCheck(coords.x,coords.y)) {
                TODO();
            }

            uint64_t pos = (coords.y * COLUMNS_X) + coords.x;

            return pos;
        }



    public:
        CellHandle getCell(uint64_t x, uint64_t y);
        

        Cells() {
            cellsHaveSandPhysics = std::vector<bool>(GRID_WIDTH * GRID_HEIGHT);
            cellsArePopulated = std::vector<bool>(GRID_WIDTH * GRID_HEIGHT);
        }

        inline static bool isInRange(uint64_t x, uint64_t y) {
            if (y >= ROWS_Y || x >= COLUMNS_X || y < 0 || x < 0) {
                return false;
            }
        
            return true;
        }
        
        bool cellHasSandPhysics(uint64_t cell_index) {
            bool result = false;
            if (BOUNDS_ASSERTIONS) {
                result = cellsHaveSandPhysics.at(cell_index);
            } else {
                result = cellsHaveSandPhysics[cell_index];
            }

            return result;
        }

        bool cellIsPopulated(uint64_t cell_index) {
            bool result = false;
            if (BOUNDS_ASSERTIONS) {
                result = cellsArePopulated.at(cell_index);
            } else {
                result = cellsArePopulated[cell_index];
            }

            return result;
        }

};


class CellHandle {
    private:
        int64_t index;
        Cells* parent;

        CellHandle() {}

        XY pos_to_xy(int64_t pos) {
            uint64_t x = (pos % COLUMNS_X);
            uint64_t y = (pos / COLUMNS_X);

            return XY(x, y);
        }


    public:
        // copy-asign constructor
        CellHandle& operator=(const CellHandle& rhs) {
            this->index = rhs.index;
            this->parent = rhs.parent; 
        }

        // copy constructor
        CellHandle(const CellHandle& rhs) {
            index = rhs.index;
            parent = rhs.parent;
        }

        // copy 
        CellHandle clone() {
            return CellHandle(*this);
        }

        // equality
        inline bool operator==(const CellHandle& rhs) {
            return rhs.index == this->index;
        }

        constexpr inline bool equals(const CellHandle& rhs) {
            return *this == rhs;
        }

        inline void incrementRowWise() {
            this->index +=1;
        }

        inline void decrementRowWise() {
            this->index -=1;
        }

        static inline CellHandle create(uint64_t cell_position, Cells *all_cells) {
            CellHandle handle;
            handle.index = cell_position;
            handle.parent = all_cells;
            return handle;
        }

        inline bool isPopulated() {
            return parent->cellIsPopulated(index);
        }

        inline bool hasSandPhysics() {
            return parent->cellHasSandPhysics(index);
        }

        inline bool hasLeft() {
            XY pos = pos_to_xy(index).subtractParts(XY(1, 0));
            return parent->isInRange(pos.x, pos.y);
        }
        
        inline CellHandle getLeft() {
            if (BOUNDS_ASSERTIONS) {
                if (!hasLeft()) {
                    TODO();
                }
            }

            XY pos = pos_to_xy(index).subtractParts(XY(1, 0));
            return parent->getCell(pos.x, pos.y);

        }

        inline bool hasRight() {
            XY pos = pos_to_xy(index).addParts(XY(1, 0));
            return parent->isInRange(pos.x, pos.y);
        }
        
        inline CellHandle getRight() {
            if (BOUNDS_ASSERTIONS) {
                if (!hasRight()) {
                    TODO();
                }
            }

            XY pos = pos_to_xy(index).addParts(XY(1, 0));
            return parent->getCell(pos.x, pos.y);
        }

        inline bool hasUp() {
            XY pos = pos_to_xy(index).subtractParts(XY(0, 1));
            return parent->isInRange(pos.x, pos.y);
        }
        
        inline CellHandle getUp() {
            if (BOUNDS_ASSERTIONS) {
                if (!hasUp()) {
                    TODO();
                }
            }

            XY pos = pos_to_xy(index).subtractParts(XY(0, 1));
            return parent->getCell(pos.x, pos.y);
        }

        inline bool hasDown() {
            XY pos = pos_to_xy(index).addParts(XY(0, 1));
            return parent->isInRange(pos.x, pos.y);
        }
        
        inline CellHandle getDown() {
            if (BOUNDS_ASSERTIONS) {
                if (!hasDown()) {
                    TODO();
                }
            }

            XY pos = pos_to_xy(index).addParts(XY(0, 1));
            return parent->getCell(pos.x, pos.y);
        }

        inline bool hasRightUp() {
            TODO();
            return false;
        }
        
        inline CellHandle getRightUp() {
            TODO();
            return CellHandle {};
        }

        inline bool hasRightDown() {
            TODO();
            return false;
        }

        inline CellHandle getRightDown() {
            TODO();
            return CellHandle {};
        }

        inline bool hasLeftUp() {
            TODO();
            return false;
        }

        inline CellHandle getLeftUp() {
            TODO();
            return CellHandle {};
        }

        inline bool hasLeftDown() {
            return false;
        }

        inline CellHandle getLeftDown() {
            TODO();
            return CellHandle {};
        }

};

CellHandle Cells::getCell(uint64_t x, uint64_t y) {
    uint64_t pos = this->checked_xy_to_pos(XY(x,y));
    CellHandle handle = CellHandle::create(pos, this);

    return handle;
}

CellHandle Cells::rowsBegin() {

}



static int CAMERA_ZOOM = 1;
float screen_ratio(int current_size, int default_size) {
    return (float)current_size / (float)default_size;
}

static Texture2D genTexture2d() {


    auto image = GenImageColor(GRID_WIDTH, GRID_HEIGHT, WHITE);
    auto texture = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture;
}


static void updateTexture(Texture2D* texture, std::vector<Color>* backing, Cells* grid) {
    

    // for (int x = 0; x < COLUMNS_X; x++) {
    //     for (int y = 0; y < ROWS_Y; y++) {
    //         int* backing_ptr = NULL;
    //         if (BOUNDS_ASSERTIONS) {
    //             int& backing_ref = (*backing).at((y * COLUMNS_X) + x);
    //             backing_ptr = &backing_ref;
    //         } else {
    //             int& backing_ref = (*backing)[((y * COLUMNS_X) + x)];
    //             backing_ptr = &backing_ref;
    //         }
            
            
    //         if (grid->cellHasSand(x,y)) {
    //             Color light_yellow = Color{0xFB,0xF1,0xB9, 255};
    //             *backing_ptr = *(int*)(Color*)(&light_yellow);
    //         } else if (grid->cellHasWater(x,y)) {
    //             Color blue = Color{0x00,0x6E,0xE6, 255};
    //             *backing_ptr = *(int*)(Color*)(&blue);
    //         } else {
    //             Color black = BLACK;
    //             *backing_ptr = *(int*)(Color*)(&black);
    //         }
    //     } 
    // }
    for (int64_t i = 0; i < backing->size(); i++) {
        backing->at(i) = WHITE;
    }

    UpdateTexture(*texture, backing->data());
}

enum : uint8_t {
    MOUSEMODE_SAND,
    MOUSEMODE_WATER,
    MOUSEMODE_REDSAND,
} MouseMode;

const Color SAND_YELLOW = Color{0xFB,0xF1,0xB9, 255};
const Color WATER_BLUE = Color{0x00,0x6E,0xE6, 255};
int main() {
    auto mouse_mode = MOUSEMODE_SAND;
    Color mouse_mode_accent = SAND_YELLOW;
    
    initWindow();

    Texture2D texture = genTexture2d();
    std::vector<Color> backing = std::vector<Color>(ROWS_Y * COLUMNS_X);
    Cells cells = Cells();

    SetWindowSize(COLUMNS_X + 50, ROWS_Y + 50);

    // bool firstRun = true;
    while (!WindowShouldClose())
    {
        updateTexture(&texture, &backing, &cells);

        if(GuiButton(Rectangle{40, 10, 90, 50}, "water") == true) {
            mouse_mode = MOUSEMODE_WATER;   
        }
        if (GuiButton(Rectangle{40+(100*1), 10, 90, 50}, "sand") == true) {
            mouse_mode = MOUSEMODE_SAND;
        }
        if (GuiButton(Rectangle{40+(100*2), 10, 90, 50}, "red sand") == true) {
            mouse_mode = MOUSEMODE_REDSAND;
        }


        switch (mouse_mode)
        {
        case MOUSEMODE_WATER:
            mouse_mode_accent = WATER_BLUE;
            break;
        case MOUSEMODE_SAND:
            mouse_mode_accent = SAND_YELLOW;
            break;
        
        case MOUSEMODE_REDSAND:
            mouse_mode_accent = MAROON;
            break;

        default:
            mouse_mode_accent = PURPLE;
            break;
        }
        
        DrawRectangle(10, 25, 20, 20, mouse_mode_accent);
        
        BeginDrawing();
        {
            ClearBackground(BLACK);
            DrawTexturePro(texture, Rectangle{0,0, (float)texture.width, (float)texture.width}, Rectangle{0, 70, (float)texture.width, (float)texture.height}, Vector2{0,0}, (float)0, Color{0xFF,0xFF,0xFF,0xFF});

        }
        EndDrawing();
    }

    CloseWindow();
}