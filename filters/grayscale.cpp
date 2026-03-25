#include "grayscale.h"

Image GrayscaleFilter::Apply(const Image& image) const {
    Image result(image.GetWidth(), image.GetHeight());

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            const Color& pixel = image.At(x, y);

            const double red = pixel.GetRed();
            const double green = pixel.GetGreen();
            const double blue = pixel.GetBlue();

            const uint8_t gray = Color::ClampToByte(0.299 * red + 0.587 * green + 0.114 * blue);
            result.At(x, y) = Color(gray, gray, gray);
        }
    }

    return result;
}
