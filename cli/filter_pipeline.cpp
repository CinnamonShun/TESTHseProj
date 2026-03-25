#include "filter_pipeline.h"

#include <utility>

FilterPipeline::FilterPipeline(std::vector<std::unique_ptr<Filter>> filters) : filters_(std::move(filters)) {
}

Image FilterPipeline::Apply(const Image& image) const {
    Image result = image;

    for (const std::unique_ptr<Filter>& filter : filters_) {
        result = filter->Apply(result);
    }

    return result;
}
