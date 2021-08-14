#include <SDL/SDL.h>
#include <emscripten.h>
#include <iostream>
#include <random>
#include <chrono>
#include <HalideBuffer.h>
#include "gameoflife.h"

const int WIDTH = 1024;
const int HEIGHT = 1024;
const int SIZE = 1;

std::uint8_t universeBuffer[WIDTH * HEIGHT];
std::uint8_t tmpBuffer[WIDTH * HEIGHT];

struct context {
    SDL_Renderer *renderer;
    int iteration;
};

void mainloop(void *arg) {
    context *ctx = static_cast<context*>(arg);
    SDL_Renderer* renderer = ctx->renderer;

    Halide::Runtime::Buffer<uint8_t> input{universeBuffer, WIDTH, HEIGHT};
    Halide::Runtime::Buffer<uint8_t> output{tmpBuffer, WIDTH, HEIGHT};

    gameoflife(input, output);
    input.copy_from(output);

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    SDL_Rect r;
    r.w = (SIZE > 2) ? SIZE - 1 : SIZE;
    r.h = (SIZE > 2) ? SIZE - 1 : SIZE;
    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) {
            r.x = j * SIZE;
            r.y = i * SIZE;
            int color = (tmpBuffer[i * WIDTH + j] == 1) ? 5 : 250;
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderFillRect(renderer, &r);
        }
    }
    SDL_RenderPresent(renderer);

    ctx->iteration++;
}

int main() {
    auto now = std::chrono::system_clock::now();
    std::mt19937 mt(now.time_since_epoch().count());
    for(int i = 0; i < WIDTH * HEIGHT; ++i) {
        universeBuffer[i] = mt() & 0x80000000 ? 0 : 1;
    }


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    window = SDL_CreateWindow("universe", 0, 0, WIDTH * SIZE, HEIGHT * SIZE, 0);
    renderer = SDL_CreateRenderer(window, 0, 0);

    context ctx;
    ctx.renderer = renderer;
    ctx.iteration = 0;

    const int simulate_infinite_loop = 1;
    const int fps = -1;
    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
