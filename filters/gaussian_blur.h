#pragma once

#include "../filter.h"

class GaussianBlurFilter : public Filter {
public:
    explicit GaussianBlurFilter(double sigma);

    Image Apply(const Image& image) const override;

private:
    double sigma_;
};
