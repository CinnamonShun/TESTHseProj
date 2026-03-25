cmake_minimum_required(VERSION 3.10)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)

add_executable(image_processor
    image_processor.cpp
    bmp_reader.cpp
    bmp_writer.cpp
)
