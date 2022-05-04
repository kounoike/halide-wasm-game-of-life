cmake_minimum_required(VERSION 3.16)
project(halide-wasm-game-of-life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

OPTION(BUILD_TESTS "Build unit tests" ON)

set(host_tools_list visualize_gen gameoflife_gen)

if(CMAKE_CROSSCOMPILING)
    add_compile_options("-matomics" "-mbulk-memory" "-pthread")

    # Pawn off the creation of the host utilities into its own dedicated space
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/host_tools)
    file(TO_NATIVE_PATH ${CMAKE_COMMAND} native_cmake_command)
    file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} native_cmake_current_source_dir)
    execute_process(
        COMMAND "sh" "-c" "CC= CXX= ${native_cmake_command} -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DHalideHelpers_DIR=${HalideHelpers_DIR} -DHalide_DIR=${Halide_DIR} ${native_cmake_current_source_dir}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/host_tools
    )
    message("*******************************Host Build CMake Done***********************************")

    add_custom_target(host_tools
        COMMAND ${CMAKE_COMMAND} --build . --config $<CONFIG>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/host_tools
    )
    include(${CMAKE_CURRENT_BINARY_DIR}/host_tools/host_tools.cmake)

    message("TARGET: ${host_tools_list}")
    foreach(tgt IN ITEMS ${host_tools_list})
        add_dependencies(host${tgt} host_tools)
    endforeach()

    find_package(HalideHelpers REQUIRED)
    set(HALIDE_TARGET_WASM "wasm-32-wasmrt-wasm_bulk_memory-wasm_simd128-wasm_sat_float_to_int-wasm_signext-wasm_threads")
    # set(HALIDE_TARGET_WASM "wasm-32-wasmrt-wasm_bulk_memory-wasm_simd128-wasm_sat_float_to_int-wasm_signext")
    add_halide_library(gameoflife FROM hostgameoflife_gen GENERATOR gameoflife_task TARGETS ${HALIDE_TARGET_WASM} PARAMS -p /Halide/lib/libautoschedule_adams2019.so -s Adams2019 auto_schedule=true)
    add_halide_library(visualize FROM hostvisualize_gen GENERATOR visualize_task  TARGETS ${HALIDE_TARGET_WASM} PARAMS -p /Halide/lib/libautoschedule_adams2019.so -s Adams2019 auto_schedule=true)
    add_halide_library(visualize1 FROM hostvisualize_gen GENERATOR visualize1_task  TARGETS ${HALIDE_TARGET_WASM} PARAMS -p /Halide/lib/libautoschedule_adams2019.so -s Adams2019 auto_schedule=true)

    add_executable(gol wasm.cc)
    target_link_libraries(gol visualize gameoflife)

    target_link_options(gol PRIVATE 
        "SHELL:-s INITIAL_MEMORY=1310720000"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s USE_PTHREADS=1"
        "SHELL:-s MODULARIZE=1"
        "SHELL:-s ASSERTIONS=2"
        "SHELL:-s EXPORT_NAME=createWasmModule"
    )

    add_executable(gol_sdl wasm_sdl.cc)
    target_link_libraries(gol_sdl PRIVATE visualize1 gameoflife)
    target_link_options(gol_sdl PRIVATE 
        "SHELL:-s INITIAL_MEMORY=36175872"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s USE_PTHREADS=0"
        "SHELL:-s USE_SDL=2"
        "SHELL:-s EXPORTED_FUNCTIONS='[\"_getCurrentFps\", \"_getLastDuration\", \"_main\"]'"
        "SHELL:-s EXPORTED_RUNTIME_METHODS='[\"ccall\"]'"
    )
    file(GENERATE OUTPUT hogehoge.txt CONTENT "$<CONFIG> $<$<CONFIG:Debug>:-O0;-g>")
    target_compile_options(gol_sdl PRIVATE
        "$<$<CONFIG:DEBUG>:-O0;-g>"
        "$<$<CONFIG:Release>:-O3>"
    )

else()
    message("*******************************Host Build CMake****************************************")
    find_package(HalideHelpers REQUIRED)
    find_package(Halide REQUIRED)

    add_executable(visualize_gen halide_gen/visualize_gen.cc)
    target_link_libraries(visualize_gen PRIVATE Halide::Generator)

    add_executable(gameoflife_gen halide_gen/gameoflife_gen.cc)
    target_link_libraries(gameoflife_gen PRIVATE Halide::Generator)

    find_package(HalideHelpers REQUIRED)
    add_halide_library(visualize FROM hostvisualize_gen GENERATOR visualize_task)
    add_halide_library(visualize1 FROM hostvisualize_gen GENERATOR visualize1_task)
    add_halide_library(gameoflife FROM hostgameoflife_gen GENERATOR gameoflife_task)

    enable_testing()
    find_package(GTest)

    add_executable(halide_test halide_gen/halide_test.cc)
    target_link_libraries(halide_test GTest::GTest GTest::Main visualize gameoflife)
    gtest_discover_tests(halide_test)

    foreach(tgt IN ITEMS ${host_tools_list})
        add_executable(host${tgt} ALIAS ${tgt})
    endforeach()

    export(TARGETS visualize_gen gameoflife_gen NAMESPACE host FILE host_tools.cmake)
endif()
