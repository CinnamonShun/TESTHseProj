#pragma once

#include <cstddef>

#include "../filter.h"

class CropFilter : public Filter {
public:
    CropFilter(size_t width, size_t height);

    Image Apply(const Image& image) const override;

private:
    size_t width_;
    size_t height_;
};
