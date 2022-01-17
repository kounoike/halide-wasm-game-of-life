cmake_minimum_required(VERSION 3.16)
project(halide-wasm-game-of-life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

OPTION(BUILD_TESTS "Build unit tests" ON)
set(Halide_DIR "/Halide/lib/cmake/Halide")
set(HalideHelpers_DIR "/Halide/lib/cmake/HalideHelpers")

set(halide_lib_list visualize visualize1 gameoflife visualize.wasm visualize1.wasm gameoflife.wasm)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/host_tools)
file(TO_NATIVE_PATH ${CMAKE_COMMAND} native_cmake_command)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/halide_gen native_cmake_current_source_dir)
execute_process(
    COMMAND "sh" "-c" "CC= CXX= CMAKE_CROSSCOMPILING= ${native_cmake_command} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DHalideHelpers_DIR=${HalideHelpers_DIR} -DHalide_DIR=${Halide_DIR} ${native_cmake_current_source_dir}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/host_tools
)

add_custom_target(generator
    COMMAND ${CMAKE_COMMAND} --build . --config $<CONFIG>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/host_tools
)
include(${CMAKE_CURRENT_BINARY_DIR}/host_tools/host_tools.cmake)

foreach(tgt IN ITEMS ${halide_lib_list})
    add_dependencies(${tgt} generator)
endforeach()

if(CMAKE_CROSSCOMPILING)
    add_executable(gol wasm.cc)
    target_link_libraries(gol visualize.wasm gameoflife.wasm visualize.wasm.runtime gameoflife.wasm.runtime)
    target_link_options(gol PRIVATE 
    "SHELL:-s INITIAL_MEMORY=1310720000"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s USE_PTHREADS=0"
    "SHELL:-s MODULARIZE=1"
    "SHELL:-s EXPORT_NAME=createWasmModule"
    )

    add_executable(gol_sdl wasm_sdl.cc)
    target_link_libraries(gol_sdl PRIVATE visualize1.wasm gameoflife.wasm visualize1.wasm.runtime gameoflife.wasm.runtime)
    target_link_options(gol_sdl PRIVATE 
        "SHELL:-s INITIAL_MEMORY=36175872"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s USE_PTHREADS=0"
        "SHELL:-s USE_SDL=2"
        "SHELL:-s EXPORTED_FUNCTIONS='[\"_getCurrentFps\", \"_getLastDuration\", \"_main\"]'"
        "SHELL:-s EXPORTED_RUNTIME_METHODS='[\"ccall\"]'"
    )
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.6.0
)
set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(googletest googlebenchmark)


enable_testing()
include(GoogleTest)

add_executable(halide_test halide_gen/halide_test.cc)
# gtest_discover_tests(halide_test)

if(CMAKE_CROSSCOMPILING)
  target_link_libraries(halide_test gtest_main visualize.wasm gameoflife.wasm)
  file(COPY "${CMAKE_SOURCE_DIR}/run_test.sh" DESTINATION ${CMAKE_BINARY_DIR} USE_SOURCE_PERMISSIONS)
  target_link_options(halide_test PRIVATE "--emrun")
  set_property(TARGET halide_test PROPERTY SUFFIX ".html")
  set_property(TARGET halide_test PROPERTY CROSSCOMPILING_EMULATOR "./run_test.sh")
else()
  target_link_libraries(halide_test gtest_main visualize gameoflife)
endif()

add_executable(halide_perf halide_gen/halide_perf.cc)
if(CMAKE_CROSSCOMPILING)
  target_link_libraries(halide_perf benchmark_main visualize.wasm gameoflife.wasm)
  target_link_options(halide_perf PRIVATE "--emrun")
  set_property(TARGET halide_perf PROPERTY SUFFIX ".html")
else()
  target_link_libraries(halide_perf benchmark_main visualize gameoflife)
endif()
