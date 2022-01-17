#include <benchmark/benchmark.h>
#include "gameoflife.h"
#include <HalideBuffer.h>

static void GoL_Logic(benchmark::State &state)
{
    const int width = state.range(0);
    const int height = state.range(0);
    Halide::Runtime::Buffer<uint8_t> input(width, height);
    Halide::Runtime::Buffer<uint8_t> output(width, height);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            input(i, j) = i == j ? 1 : 0;
        }
    }
    for (auto _ : state)
    {
        gameoflife(input, output);
        input.copy_from(output);
    }
}
// Register the function as a benchmark
BENCHMARK(GoL_Logic)->RangeMultiplier(2)->Range(128, 16 * 1024);
