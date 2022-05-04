cmake_minimum_required(VERSION 3.16)
project(halide-gen)


add_compile_options("-pthreads")

set(CMAKE_CXX_STANDARD 17)
# -lHalide -L ../../../../Halide-11.0.1-x86-64-linux/lib/ -std=c++17 -Wl,-rpath ../../../../Halide-11.0.1-x86-64-linux/lib
# ./gen -g input_task target=wasm-32-wasmrt-wasm_simd128-wasm_threads-wasm_bulk_memory -o ../lib

find_package(Halide)
find_package(HalideHelpers)

add_executable(visualize_gen visualize_gen.cc)
target_link_libraries(visualize_gen PRIVATE Halide::Generator)

add_executable(gameoflife_gen gameoflife_gen.cc)
target_link_libraries(gameoflife_gen PRIVATE Halide::Generator)

add_halide_library(gameoflife FROM gameoflife_gen GENERATOR gameoflife_task TARGETS ${HALIDE_TARGET} AUTOSCHEDULER Halide::Adams2019 PARAMS auto_schedule=true)
add_halide_library(visualize FROM visualize_gen GENERATOR visualize_task USE_RUNTIME gameoflife.runtime TARGETS ${HALIDE_TARGET} AUTOSCHEDULER Halide::Adams2019 PARAMS auto_schedule=true)
add_halide_library(visualize1 FROM visualize_gen GENERATOR visualize1_task USE_RUNTIME gameoflife.runtime TARGETS ${HALIDE_TARGET} AUTOSCHEDULER Halide::Adams2019 PARAMS auto_schedule=true)

add_halide_library(gameoflife.wasm FROM gameoflife_gen GENERATOR gameoflife_task TARGETS ${HALIDE_TARGET} AUTOSCHEDULER Halide::Adams2019 PARAMS auto_schedule=true)
add_halide_library(visualize.wasm FROM visualize_gen GENERATOR visualize_task TARGETS ${HALIDE_TARGET} AUTOSCHEDULER Halide::Adams2019 PARAMS auto_schedule=true)
add_halide_library(visualize1.wasm FROM visualize_gen GENERATOR visualize1_task TARGETS ${HALIDE_TARGET} AUTOSCHEDULER Halide::Adams2019 PARAMS auto_schedule=true)


export(
  TARGETS
    visualize visualize.runtime
    visualize1 visualize1.runtime
    gameoflife gameoflife.runtime
    visualize.wasm visualize.wasm.runtime
    visualize1.wasm visualize1.wasm.runtime
    gameoflife.wasm gameoflife.wasm.runtime
  FILE host_tools.cmake)

# add_halide_library(visualize FROM visualize_gen GENERATOR visualize_task)
# add_halide_library(gameoflife FROM gameoflife_gen GENERATOR gameoflife_task)

