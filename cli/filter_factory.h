#pragma once

#include <memory>
#include <vector>

#include "../filter.h"
#include "arguments_parser.h"

class FilterFactory {
public:
    std::vector<std::unique_ptr<Filter>> Create(const std::vector<FilterRequest>& filter_requests) const;
};
