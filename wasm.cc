#include <emscripten.h>
#include <chrono>
#include <random>
#include <iostream>
#include "visualize.h"
#include "gameoflife.h"
#include "HalideBuffer.h"

const int MAX_WIDTH = 4096;
const int MAX_HEIGHT = 4096;
const int MAX_SIZE = 4;

std::uint8_t inputBuffer[MAX_WIDTH * MAX_HEIGHT];
std::uint8_t outputBuffer[MAX_WIDTH * MAX_HEIGHT];
std::uint8_t visualizeBuffer[MAX_WIDTH * MAX_SIZE * MAX_HEIGHT * MAX_SIZE * 4];

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    std::uint8_t *getInputBufferOffset()
    {
        return inputBuffer;
    }

    EMSCRIPTEN_KEEPALIVE
    std::uint8_t *getOutputBufferOffset()
    {
        return outputBuffer;
    }

    EMSCRIPTEN_KEEPALIVE
    std::uint8_t *getVisualizeBufferOffset()
    {
        return visualizeBuffer;
    }

    EMSCRIPTEN_KEEPALIVE
    void initialize(int width, int height, int size, int seed)
    {
        std::cout << "initialize " << width << "x" << height << " * " << size << " : " << seed << std::endl;
        std::mt19937 mt(seed);
        for (int i = 0; i < width * height; ++i)
        {
            inputBuffer[i] = mt() & 0x80000000 ? 0 : 1;
        }
        std::cout << "define buffer." << std::endl;
        Halide::Runtime::Buffer<uint8_t> input{inputBuffer, width, height};
        std::cout << "define buffer done. make_interleaved" << std::endl;
        auto visualizeOutput = Halide::Runtime::Buffer<uint8_t>::make_interleaved(visualizeBuffer, width * size, height * size, 4);
        std::cout << "make_interleaved0 done. do visualize" << std::endl;
        // visualize(input, size, visualizeOutput);
        std::cout << "visualize done." << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    int64_t exec(int width, int height, int size)
    {
        std::cout << "exec " << width << "x" << height << " * " << size << std::endl;
        auto start = std::chrono::system_clock::now();
        Halide::Runtime::Buffer<uint8_t> input{inputBuffer, width, height};
        Halide::Runtime::Buffer<uint8_t> output{outputBuffer, width, height};
        auto visualizeOutput = Halide::Runtime::Buffer<uint8_t>::make_interleaved(visualizeBuffer, width * size, height * size, 4);

        gameoflife(input, output);
        input.copy_from(output);
        visualize(output, size, visualizeOutput);
        auto stop = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        return duration;
    }
}
