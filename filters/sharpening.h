#pragma once

#include "matrix_filter.h"

class SharpeningFilter : public MatrixFilter {
public:
    Image Apply(const Image& image) const override;
};
