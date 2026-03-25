#include "image.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

Image::Image() : width_(0), height_(0), pixels_() {
}

Image::Image(const size_t width, const size_t height) : width_(width), height_(height), pixels_() {
    if (width_ != 0 && height_ > std::numeric_limits<size_t>::max() / width_) {
        throw std::length_error("Image size is too large");
    }

    pixels_.resize(width_ * height_);
}

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return height_;
}

bool Image::IsEmpty() const {
    return width_ == 0 || height_ == 0;
}

Color& Image::At(const size_t x, const size_t y) {
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinates are out of range");
    }
    return pixels_[GetIndex(x, y)];
}

const Color& Image::At(const size_t x, const size_t y) const {
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinates are out of range");
    }
    return pixels_[GetIndex(x, y)];
}

const Color& Image::AtClamped(const int x, const int y) const {
    if (IsEmpty()) {
        throw std::logic_error("Cannot access pixels of an empty image");
    }

    const int max_x = static_cast<int>(width_ - 1);
    const int max_y = static_cast<int>(height_ - 1);

    const size_t clamped_x = static_cast<size_t>(std::clamp(x, 0, max_x));
    const size_t clamped_y = static_cast<size_t>(std::clamp(y, 0, max_y));

    return pixels_[GetIndex(clamped_x, clamped_y)];
}

size_t Image::GetIndex(const size_t x, const size_t y) const {
    return y * width_ + x;
}
