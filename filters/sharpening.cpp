#include "sharpening.h"

Image SharpeningFilter::Apply(const Image& image) const {
    const std::array<std::array<double, 3>, 3> kernel = {
        std::array<double, 3>{0.0, -1.0, 0.0},
        std::array<double, 3>{-1.0, 5.0, -1.0},
        std::array<double, 3>{0.0, -1.0, 0.0},
    };

    return ApplyMatrix(image, kernel);
}
