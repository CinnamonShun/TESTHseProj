#pragma once

#include "matrix_filter.h"

class EdgeDetectionFilter : public MatrixFilter {
public:
    explicit EdgeDetectionFilter(double threshold);

    Image Apply(const Image& image) const override;

private:
    double threshold_;
};
