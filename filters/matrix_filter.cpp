#include "matrix_filter.h"

Image MatrixFilter::ApplyMatrix(const Image& image, const std::array<std::array<double, 3>, 3>& kernel) const {
    Image result(image.GetWidth(), image.GetHeight());

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            double red = 0.0;
            double green = 0.0;
            double blue = 0.0;

            for (int kernel_y = -1; kernel_y <= 1; ++kernel_y) {
                for (int kernel_x = -1; kernel_x <= 1; ++kernel_x) {
                    const int source_x = static_cast<int>(x) + kernel_x;
                    const int source_y = static_cast<int>(y) + kernel_y;

                    const Color& source = image.AtClamped(source_x, source_y);
                    const double weight = kernel[kernel_y + 1][kernel_x + 1];

                    red += static_cast<double>(source.GetRed()) * weight;
                    green += static_cast<double>(source.GetGreen()) * weight;
                    blue += static_cast<double>(source.GetBlue()) * weight;
                }
            }

            result.At(x, y) = Color(Color::ClampToByte(red), Color::ClampToByte(green), Color::ClampToByte(blue));
        }
    }

    return result;
}
