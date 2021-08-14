#include <emscripten.h>
#include <chrono>
#include <random>
#include <iostream>
#include "visualize.h"
#include "gameoflife.h"
#include "HalideBuffer.h"

const int MAX_WIDTH = 2048;
const int MAX_HEIGHT = 2048;
const int MAX_SIZE = 8;

std::uint8_t inputBuffer[MAX_WIDTH * MAX_HEIGHT];
std::uint8_t outputBuffer[MAX_WIDTH * MAX_HEIGHT];
std::uint8_t visualizeBuffer[MAX_WIDTH * MAX_SIZE * MAX_HEIGHT * MAX_SIZE * 4];

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    std::uint8_t* getInputBufferOffset() {
        return inputBuffer;
    }

    EMSCRIPTEN_KEEPALIVE
    std::uint8_t* getOutputBufferOffset() {
        return outputBuffer;
    }

    EMSCRIPTEN_KEEPALIVE
    std::uint8_t* getVisualizeBufferOffset() {
        return visualizeBuffer;
    }

    EMSCRIPTEN_KEEPALIVE
    void initialize(int width, int height, int size, int seed) {
        std::cout << "initialize " << width << "x" << height << " * " << size << " : " << seed << std::endl;
        std::mt19937 mt(seed);
        for(int i = 0; i < width * height; ++i) {
            inputBuffer[i] = mt() & 0x80000000 ? 0 : 1;
        }
        Halide::Runtime::Buffer<uint8_t> input{inputBuffer, width, height};
        auto visualizeOutput = Halide::Runtime::Buffer<uint8_t>::make_interleaved(visualizeBuffer, width * size, height * size, 4);
        visualize(input, size, visualizeOutput);
    }

    EMSCRIPTEN_KEEPALIVE
    int64_t exec(int width, int height, int size) {
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
