#include "vignette.h"

#include <algorithm>
#include <cmath>

#include "../exceptions.h"

VignetteFilter::VignetteFilter(const double radius) : radius_(radius) {
    if (radius_ <= 0.0) {
        throw InvalidArgumentsException("vignette radius must be positive");
    }
}

Image VignetteFilter::Apply(const Image& image) const {
    if (image.IsEmpty()) {
        return image;
    }

    Image result(image.GetWidth(), image.GetHeight());

    const double center_x = (static_cast<double>(image.GetWidth()) - 1.0) / 2.0;
    const double center_y = (static_cast<double>(image.GetHeight()) - 1.0) / 2.0;

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            const Color& source = image.At(x, y);

            const double dx = static_cast<double>(x) - center_x;
            const double dy = static_cast<double>(y) - center_y;
            const double distance = std::sqrt(dx * dx + dy * dy);

            const double linear_factor = std::clamp(1.0 - distance / radius_, 0.0, 1.0);
            const double factor = linear_factor * linear_factor;

            const double red = static_cast<double>(source.GetRed()) * factor;
            const double green = static_cast<double>(source.GetGreen()) * factor;
            const double blue = static_cast<double>(source.GetBlue()) * factor;

            result.At(x, y) = Color(Color::ClampToByte(red), Color::ClampToByte(green), Color::ClampToByte(blue));
        }
    }

    return result;
}
