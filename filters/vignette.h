#pragma once

#include "../filter.h"

class VignetteFilter : public Filter {
public:
    explicit VignetteFilter(double radius);

    Image Apply(const Image& image) const override;

private:
    double radius_;
};
