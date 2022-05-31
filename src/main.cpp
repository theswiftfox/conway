
#include <iostream>
#include <chrono>

#include <SDL.h>
#include "conway.h"

auto getClockInMs()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

const int UPDATE_INTERVAL = 250; // ms

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                     SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_CreateWindowAndRenderer(900, 600, 0, &window,
                                    &renderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Create window and renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_SetWindowTitle(window, "Conway's Game of Life");

    Conway conway(30, 30, window, renderer);

    bool quit = false;
    bool pause = false;
    auto lt = getClockInMs();
    auto timeUntilUpdate = 0ll;
    while (!quit)
    {
        auto ts = getClockInMs();
        auto dt = ts - lt;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_SPACE:
                    pause = !pause;
                    break;
                case SDLK_F8:
                    conway.reset();
                    break;
                }
            }
        }

        if (!pause)
        {
            timeUntilUpdate -= dt;
            if (timeUntilUpdate <= 0)
            {
                conway.update();
                timeUntilUpdate = UPDATE_INTERVAL;
            }
        }

        lt = ts;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
