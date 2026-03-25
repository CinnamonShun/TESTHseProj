#pragma once

#include <vector>

#include "../filter.h"

class MatrixFilter : public Filter {
protected:
    Image ApplyMatrix(const Image& image, const std::vector<std::vector<double>>& kernel) const;
};
