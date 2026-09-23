cmake_minimum_required(VERSION 4.3)

project(nebula_lang)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_executable(nebula_lang
        include/lexer.hpp
        include/parser.hpp
        include/preprocessor.hpp
        src/lexer.cpp
        src/main.cpp
        src/preprocessor.cpp
)

set_target_properties(nebula_lang PROPERTIES OUTPUT_NAME "nebula")