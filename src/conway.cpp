#include "conway.h"

#include <iostream>
#include <random>

Conway::Conway(int w, int h, SDL_Window *hWin, SDL_Renderer *hRenderer, bool infiniteModeGrid) : useInfiniteModeGrid(infiniteModeGrid),
                                                                                                 gridWidth(w),
                                                                                                 gridHeight(h)
{
    auto size = w * h;
    grid = std::make_unique<int[]>(size);
    initGrid();
    // printGrid();

    SDL_GetWindowSize(hWin, &windowWidth, &windowHeight);
    gridCellSize = {
        .w = windowWidth / gridWidth,
        .h = windowHeight / gridHeight};

    sdlWindow = std::shared_ptr<SDL_Window>(hWin, SDL_DestroyWindow);
    sdlRenderer = std::shared_ptr<SDL_Renderer>(hRenderer, SDL_DestroyRenderer);
}

void Conway::reset()
{
    auto size = gridWidth * gridHeight;
    grid = std::make_unique<int[]>(size);
    initGrid();
}

void Conway::initGrid()
{
    int maxInitCells = 150; // fixme: whats a good inital value?
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, gridWidth * gridHeight);

    for (auto i = 0; i < maxInitCells; i++)
    {
        grid[dist(rng)] = 1;
    }
}

void Conway::printGrid()
{
    for (auto y = 0; y < gridHeight; y++)
    {
        for (auto x = 0; x < gridWidth; x++)
        {
            std::cout << grid[y * gridWidth + x] << " ";
        }
        std::cout << std::endl;
    }
}

void Conway::update()
{
    auto newGrid = std::make_unique<int[]>(gridWidth * gridHeight);
    for (auto y = 0; y < gridHeight; y++)
    {
        for (auto x = 0; x < gridWidth; x++)
        {
            newGrid[y * gridWidth + x] = checkCell(x, y);
        }
    }
    grid = std::move(newGrid);
    draw();
}

int Conway::checkCell(int x, int y)
{
    auto cell = grid[y * gridWidth + x];
    auto neighbors = cellAt(x - 1, y)        // left
                     + cellAt(x - 1, y - 1)  // top left
                     + cellAt(x - 1, y + 1)  // bot left
                     + cellAt(x, y - 1)      // top
                     + cellAt(x, y + 1)      // bottom
                     + cellAt(x + 1, y)      // right
                     + cellAt(x + 1, y - 1)  // top right
                     + cellAt(x + 1, y + 1); // bot right

    if (cell == 0 && neighbors == 3)
    {
        return 1;
    }

    if (cell == 1 && (neighbors == 2 || neighbors == 3))
    {
        return 1;
    }
    return 0;
}

int Conway::cellAt(int x, int y)
{
    if (useInfiniteModeGrid)
    {
        x = x < 0 ? gridWidth - x : x;
        x = x >= gridWidth ? x - gridWidth : x;
        y = y < 0 ? gridHeight - y : y;
        y = y >= gridHeight ? y - gridHeight : y;
    }
    else
    {
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridWidth)
        {
            return 0;
        }
    }

    int index = y * gridWidth + x;
    return grid[index];
}

void Conway::draw()
{
    auto hRenderer = sdlRenderer.get();
    SDL_SetRenderDrawColor(hRenderer, grid_background.r, grid_background.g, grid_background.b, grid_background.a);
    SDL_RenderClear(hRenderer);

    SDL_SetRenderDrawColor(hRenderer, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);

    for (auto x = 0; x < 1 + gridWidth * gridCellSize.w; x += gridCellSize.w)
    {
        SDL_RenderDrawLine(hRenderer, x, 0, x, windowHeight);
    }
    for (auto y = 0; y < 1 + gridWidth * gridCellSize.h; y += gridCellSize.h)
    {
        SDL_RenderDrawLine(hRenderer, 0, y, windowWidth, y);
    }

    // Draw cells
    SDL_SetRenderDrawColor(hRenderer, grid_cell_color.r, grid_cell_color.g, grid_cell_color.b, grid_cell_color.a);
    for (auto y = 0; y < gridHeight; y++)
    {
        for (auto x = 0; x < gridWidth; x++)
        {
            if (grid[y * gridWidth + x] == 1)
            {
                SDL_Rect cell = {
                    .x = x * gridCellSize.w + 1,
                    .y = y * gridCellSize.h + 1,
                    .w = gridCellSize.w - 2,
                    .h = gridCellSize.h - 2};
                SDL_RenderFillRect(hRenderer, &cell);
            }
        }
    }

    SDL_RenderPresent(hRenderer);
}