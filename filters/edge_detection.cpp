#include "edge_detection.h"

#include "../exceptions.h"
#include "grayscale.h"

#include <vector>

EdgeDetectionFilter::EdgeDetectionFilter(const double threshold) : threshold_(threshold) {
    if (threshold_ < 0.0 || threshold_ > 1.0) {
        throw InvalidArgumentsException("edge threshold must be in [0, 1]");
    }
}

Image EdgeDetectionFilter::Apply(const Image& image) const {
    const Image grayscale = GrayscaleFilter().Apply(image);

    static const std::vector<std::vector<double>> KERNEL = {
        {0.0, -1.0, 0.0},
        {-1.0, 4.0, -1.0},
        {0.0, -1.0, 0.0},
    };

    const Image edges = ApplyMatrix(grayscale, KERNEL);

    Image result(edges.GetWidth(), edges.GetHeight());
    for (size_t y = 0; y < edges.GetHeight(); ++y) {
        for (size_t x = 0; x < edges.GetWidth(); ++x) {
            const double normalized = static_cast<double>(edges.At(x, y).GetRed()) / 255.0;
            const uint8_t value = normalized > threshold_ ? 255 : 0;
            result.At(x, y) = Color(value, value, value);
        }
    }

    return result;
}