#pragma once

#include <cstdint>

#pragma pack(push, 1)

struct BmpFileHeader {
    uint16_t signature;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixel_data_offset;
};

struct BmpInfoHeader {
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;
};

#pragma pack(pop)
