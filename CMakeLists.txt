cmake_minimum_required(VERSION 3.10)
project(TelemetryEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# Main executable
add_executable(telemetry_engine
    src/main.cpp
    # src/core/telemetry_data.cpp
    # src/core/data_ingestor.cpp
    # src/core/plugin_manager.cpp
    # src/plugins/noise_filter_plugin.cpp
    # src/plugins/track_correlator_plugin.cpp
    # src/output/logger.cpp
    # src/utils/config.cpp
)

# # Tests
# if(BUILD_TESTS)
#     enable_testing()
#     add_subdirectory(tests)
# endif()

# # Static analysis targets
# add_custom_target(static-analysis
#     COMMAND echo "Run: cppcheck --enable=all --std=c++11 --suppress=missingInclude ${CMAKE_SOURCE_DIR}/src"
#     COMMAND echo "Run: clang-tidy -checks='*' ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/**/*.cpp"
#     COMMENT "Static analysis commands"
# )