#include "color.h"

#include <algorithm>
#include <cmath>

Color::Color() : red_(0), green_(0), blue_(0) {
}

Color::Color(const uint8_t red, const uint8_t green, const uint8_t blue) : red_(red), green_(green), blue_(blue) {
}

uint8_t Color::GetRed() const {
    return red_;
}

uint8_t Color::GetGreen() const {
    return green_;
}

uint8_t Color::GetBlue() const {
    return blue_;
}

void Color::SetRed(const uint8_t red) {
    red_ = red;
}

void Color::SetGreen(const uint8_t green) {
    green_ = green;
}

void Color::SetBlue(const uint8_t blue) {
    blue_ = blue;
}

uint8_t Color::ClampToByte(const double value) {
    const double clamped_value = std::clamp(value, 0.0, 255.0);
    return static_cast<uint8_t>(std::lround(clamped_value));
}
