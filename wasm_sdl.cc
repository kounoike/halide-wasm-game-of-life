#include <SDL/SDL.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include <random>
#include <chrono>
#include <HalideBuffer.h>
#include "gameoflife.h"
#include "visualize1.h"

const int WIDTH = 1024;
const int HEIGHT = 1024;
const int SIZE = 1;

std::uint8_t universeBuffer[WIDTH * HEIGHT];
std::uint8_t tmpBuffer[WIDTH * HEIGHT];
std::uint8_t visualizeBuffer[WIDTH * SIZE * HEIGHT * SIZE * 3 / 2];

struct context
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int iteration;
};

extern "C" {

void mainloop(void *arg)
{
    context *ctx = static_cast<context *>(arg);

    Halide::Runtime::Buffer<uint8_t> input{universeBuffer, WIDTH, HEIGHT};
    Halide::Runtime::Buffer<uint8_t> output{tmpBuffer, WIDTH, HEIGHT};

    gameoflife(input, output);
    input.copy_from(output);

    Halide::Runtime::Buffer<uint8_t> visualizeOutput(visualizeBuffer, WIDTH * SIZE, HEIGHT * SIZE);
    visualize1(input, SIZE, visualizeOutput);

    int ret;
    ret = SDL_UpdateTexture(ctx->texture, NULL, visualizeBuffer, WIDTH * SIZE);
    SDL_SetRenderDrawColor(ctx->renderer, 255, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);
    ret = SDL_RenderCopy(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);

    ctx->iteration++;
}

int main()
{
    std::cout << "Init" << std::endl;
    SDL_Init(SDL_INIT_VIDEO);
    context ctx;
    ctx.window = SDL_CreateWindow("universe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * SIZE, HEIGHT * SIZE, SDL_WINDOW_SHOWN);
    ctx.renderer = SDL_CreateRenderer(ctx.window, -1, 0);
    ctx.texture = SDL_CreateTexture(ctx.renderer, SDL_PIXELFORMAT_YV12, SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, WIDTH * SIZE, HEIGHT * SIZE);
    std::cout << SDL_GetError() << std::endl;
    std::cout << "window: " << ctx.window << " renderer: " << ctx.renderer << " texture: " << ctx.texture << std::endl;
    ctx.iteration = 0;

    auto now = std::chrono::system_clock::now();
    std::mt19937 mt(now.time_since_epoch().count());
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
    {
        universeBuffer[i] = mt() & 0x80000000 ? 0 : 1;
    }
    for (int i = 0; i < WIDTH * SIZE * HEIGHT * SIZE * 3 / 2; ++i) {
        visualizeBuffer[i] = 128;
    }

    const int simulate_infinite_loop = 1;
    const int fps = -1;
    std::cout << "before set_main_loop" << std::endl;
    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);
    std::cout << "after set_main_loop" << std::endl;

    // SDL_DestroyRenderer(ctx.renderer);
    // SDL_DestroyWindow(ctx.window);
    // SDL_Quit();

    return EXIT_SUCCESS;
}
}
