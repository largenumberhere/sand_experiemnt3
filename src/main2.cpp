#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include "cells.h"
#include "cellhandle.h"
#include "celliter.h"
#include "constants.h"

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
