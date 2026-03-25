#include "sharpening.h"

Image SharpeningFilter::Apply(const Image& image) const {
    static const std::vector<std::vector<double>> KERNEL = {
        {0.0, -1.0, 0.0},
        {-1.0, 5.0, -1.0},
        {0.0, -1.0, 0.0},
    };

    return ApplyMatrix(image, KERNEL);
}
