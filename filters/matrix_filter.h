#pragma once

#include <array>

#include "../filter.h"

class MatrixFilter : public Filter {
protected:
    Image ApplyMatrix(const Image& image, const std::array<std::array<double, 3>, 3>& kernel) const;
};
