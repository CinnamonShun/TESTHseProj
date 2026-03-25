#pragma once

#include "../filter.h"

class NegativeFilter : public Filter {
public:
    Image Apply(const Image& image) const override;
};
