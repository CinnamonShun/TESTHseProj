#pragma once

#include <memory>
#include <vector>

#include "../filter.h"

class FilterPipeline {
public:
    explicit FilterPipeline(std::vector<std::unique_ptr<Filter>> filters);

    Image Apply(const Image& image) const;

private:
    std::vector<std::unique_ptr<Filter>> filters_;
};
