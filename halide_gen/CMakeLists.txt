cmake_minimum_required(VERSION 3.16)
project(halide-gen)

set(CMAKE_CXX_STANDARD 17)

# -lHalide -L ../../../../Halide-11.0.1-x86-64-linux/lib/ -std=c++17 -Wl,-rpath ../../../../Halide-11.0.1-x86-64-linux/lib
# ./gen -g input_task target=wasm-32-wasmrt-wasm_simd128-wasm_threads-wasm_bulk_memory -o ../lib

find_package(HalideHelpers)
find_package(Halide)

add_executable(visualize_gen visualize_gen.cc)
target_link_libraries(visualize_gen PRIVATE Halide::Generator)

add_executable(gameoflife_gen gameoflife_gen.cc)
target_link_libraries(gameoflife_gen PRIVATE Halide::Generator)

# add_halide_library(visualize FROM visualize_gen GENERATOR visualize_task)
# add_halide_library(gameoflife FROM gameoflife_gen GENERATOR gameoflife_task)

