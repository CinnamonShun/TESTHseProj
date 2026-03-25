#pragma once

#include <cstddef>
#include <vector>

#include "color.h"

class Image {
public:
    Image();
    Image(size_t width, size_t height);

    size_t GetWidth() const;
    size_t GetHeight() const;

    bool IsEmpty() const;

    Color& At(size_t x, size_t y);
    const Color& At(size_t x, size_t y) const;
    const Color& AtClamped(int x, int y) const;

private:
    size_t GetIndex(size_t x, size_t y) const;

    size_t width_;
    size_t height_;
    std::vector<Color> pixels_;
};
