#include "negative.h"

Image NegativeFilter::Apply(const Image& image) const {
    Image result(image.GetWidth(), image.GetHeight());

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            const Color& pixel = image.At(x, y);

            const uint8_t red = static_cast<uint8_t>(255 - pixel.GetRed());
            const uint8_t green = static_cast<uint8_t>(255 - pixel.GetGreen());
            const uint8_t blue = static_cast<uint8_t>(255 - pixel.GetBlue());

            result.At(x, y) = Color(red, green, blue);
        }
    }

    return result;
}
