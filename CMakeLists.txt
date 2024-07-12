cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)


project(AbelianSandpileModel
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 labwork3"
        LANGUAGES CXX)


add_subdirectory(lib)

add_executable(app bin/main.cpp)
target_link_libraries(app PRIVATE sandpile_lib)


