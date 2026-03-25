#include "crop.h"

#include <algorithm>

CropFilter::CropFilter(const size_t width, const size_t height) : width_(width), height_(height) {
}

Image CropFilter::Apply(const Image& image) const {
    const size_t result_width = std::min(width_, image.GetWidth());
    const size_t result_height = std::min(height_, image.GetHeight());

    Image result(result_width, result_height);

    for (size_t y = 0; y < result_height; ++y) {
        for (size_t x = 0; x < result_width; ++x) {
            result.At(x, y) = image.At(x, y);
        }
    }

    return result;
}
