#pragma once

#include <memory>

#include <SDL2/SDL.h>

const SDL_Color grid_background = {22, 22, 22, 255};    // Blackish
const SDL_Color grid_line_color = {44, 44, 44, 255};    // Dark gray
const SDL_Color grid_cell_color = {255, 255, 255, 255}; // White

struct CellSize
{
    int w, h;
};

class Conway
{
public:
    Conway(int w, int h, SDL_Window *hWin, SDL_Renderer *hRenderer, bool infiniteMode = true);
    void update();

private:
    int gridWidth, gridHeight;
    int windowWidth, windowHeight;
    bool useInfiniteModeGrid;
    CellSize gridCellSize;

    std::unique_ptr<int[]> grid;
    std::shared_ptr<SDL_Window> sdlWindow;
    std::shared_ptr<SDL_Renderer> sdlRenderer;

    int checkCell(int x, int y);
    int cellAt(int x, int y);

    void initGrid();
    void printGrid();

    void draw();
    SDL_Rect drawLifeCell();
};