cmake_minimum_required(VERSION 3.19)

project(sortings
    VERSION 0.1.0
    DESCRIPTION "The project demonstrates a bunch of various sorting algorithms"
    HOMEPAGE_URL "https://github.com/mohterbaord/sortings.git"
    LANGUAGES C)

set(CMAKE_C_FLAGS "-std=c17")

include_directories(src/main/c)

# targets
set(TARGET_MAIN src/main/c/main.c)

add_executable(main ${TARGET_MAIN})
