#include "matrix_filter.h"

#include "../exceptions.h"

Image MatrixFilter::ApplyMatrix(const Image& image, const std::vector<std::vector<double>>& kernel) const {
    if (kernel.size() != 3) {
        throw InvalidArgumentsException("kernel must contain 3 rows");
    }

    for (const auto& row : kernel) {
        if (row.size() != 3) {
            throw InvalidArgumentsException("kernel must be 3x3");
        }
    }

    Image result(image.GetWidth(), image.GetHeight());

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            double red = 0.0;
            double green = 0.0;
            double blue = 0.0;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    const int source_x = static_cast<int>(x) + dx;
                    const int source_y = static_cast<int>(y) + dy;

                    const Color& pixel = image.AtClamped(source_x, source_y);
                    const double coefficient = kernel[dy + 1][dx + 1];

                    red += pixel.GetRed() * coefficient;
                    green += pixel.GetGreen() * coefficient;
                    blue += pixel.GetBlue() * coefficient;
                }
            }

            result.At(x, y) = Color(Color::ClampToByte(red), Color::ClampToByte(green), Color::ClampToByte(blue));
        }
    }

    return result;
}
