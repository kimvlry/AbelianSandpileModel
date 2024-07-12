add_library(
        sandpile_lib
        argument_parser.h
        argument_parser.cpp

        sandpile.h
        sandpile.cpp

        image.h
        image.cpp
)

target_include_directories(sandpile_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
