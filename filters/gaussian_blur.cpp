#include "gaussian_blur.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../exceptions.h"

namespace {

std::vector<double> BuildKernel(const double sigma) {
    const int radius = std::max(1, static_cast<int>(std::ceil(3.0 * sigma)));
    const int size = radius * 2 + 1;

    std::vector<double> kernel(static_cast<size_t>(size));
    double sum = 0.0;

    for (int i = 0; i < size; ++i) {
        const int offset = i - radius;
        const double value = std::exp(-(offset * offset) / (2.0 * sigma * sigma));
        kernel[static_cast<size_t>(i)] = value;
        sum += value;
    }

    for (double& value : kernel) {
        value /= sum;
    }

    return kernel;
}

Image BlurHorizontal(const Image& image, const std::vector<double>& kernel) {
    Image result(image.GetWidth(), image.GetHeight());

    const int radius = static_cast<int>(kernel.size() / 2);
    const int max_x = static_cast<int>(image.GetWidth() - 1);

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            double red = 0.0;
            double green = 0.0;
            double blue = 0.0;

            for (int offset = -radius; offset <= radius; ++offset) {
                const int source_x = std::clamp(static_cast<int>(x) + offset, 0, max_x);
                const Color& pixel = image.At(static_cast<size_t>(source_x), y);

                const double weight = kernel[static_cast<size_t>(offset + radius)];
                red += static_cast<double>(pixel.GetRed()) * weight;
                green += static_cast<double>(pixel.GetGreen()) * weight;
                blue += static_cast<double>(pixel.GetBlue()) * weight;
            }

            result.At(x, y) = Color(Color::ClampToByte(red), Color::ClampToByte(green), Color::ClampToByte(blue));
        }
    }

    return result;
}

Image BlurVertical(const Image& image, const std::vector<double>& kernel) {
    Image result(image.GetWidth(), image.GetHeight());

    const int radius = static_cast<int>(kernel.size() / 2);
    const int max_y = static_cast<int>(image.GetHeight() - 1);

    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            double red = 0.0;
            double green = 0.0;
            double blue = 0.0;

            for (int offset = -radius; offset <= radius; ++offset) {
                const int source_y = std::clamp(static_cast<int>(y) + offset, 0, max_y);
                const Color& pixel = image.At(x, static_cast<size_t>(source_y));

                const double weight = kernel[static_cast<size_t>(offset + radius)];
                red += static_cast<double>(pixel.GetRed()) * weight;
                green += static_cast<double>(pixel.GetGreen()) * weight;
                blue += static_cast<double>(pixel.GetBlue()) * weight;
            }

            result.At(x, y) = Color(Color::ClampToByte(red), Color::ClampToByte(green), Color::ClampToByte(blue));
        }
    }

    return result;
}

}  // namespace

GaussianBlurFilter::GaussianBlurFilter(const double sigma) : sigma_(sigma) {
    if (sigma_ <= 0.0) {
        throw InvalidArgumentsException("blur sigma must be positive");
    }
}

Image GaussianBlurFilter::Apply(const Image& image) const {
    if (image.IsEmpty()) {
        return image;
    }

    const std::vector<double> kernel = BuildKernel(sigma_);
    const Image horizontal = BlurHorizontal(image, kernel);
    return BlurVertical(horizontal, kernel);
}
