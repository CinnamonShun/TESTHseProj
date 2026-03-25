#include "filter_factory.h"

#include <cmath>
#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>

#include "../exceptions.h"
#include "../filters/crop.h"
#include "../filters/edge_detection.h"
#include "../filters/gaussian_blur.h"
#include "../filters/grayscale.h"
#include "../filters/negative.h"
#include "../filters/sharpening.h"
#include "../filters/vignette.h"

namespace {

using FilterCreator = std::function<std::unique_ptr<Filter>(const std::vector<std::string>&)>;

void RequireParameterCount(const std::vector<std::string>& parameters, const std::size_t expected_count,
                           const std::string& filter_name) {
    if (parameters.size() != expected_count) {
        throw InvalidArgumentsException("filter -" + filter_name + " expects " + std::to_string(expected_count) +
                                        " parameter(s)");
    }
}

std::size_t ParsePositiveSize(const std::string& value, const std::string& parameter_name) {
    std::size_t parsed_length = 0;
    unsigned long long parsed_value = 0;

    try {
        parsed_value = std::stoull(value, &parsed_length);
    } catch (const std::exception&) {
        throw InvalidArgumentsException("invalid value for " + parameter_name + ": " + value);
    }

    if (parsed_length != value.size()) {
        throw InvalidArgumentsException("invalid value for " + parameter_name + ": " + value);
    }

    if (parsed_value == 0) {
        throw InvalidArgumentsException(parameter_name + " must be positive");
    }

    if (parsed_value > std::numeric_limits<std::size_t>::max()) {
        throw InvalidArgumentsException(parameter_name + " is too large");
    }

    return static_cast<std::size_t>(parsed_value);
}

double ParseDouble(const std::string& value, const std::string& parameter_name) {
    std::size_t parsed_length = 0;
    double parsed_value = 0.0;

    try {
        parsed_value = std::stod(value, &parsed_length);
    } catch (const std::exception&) {
        throw InvalidArgumentsException("invalid value for " + parameter_name + ": " + value);
    }

    if (parsed_length != value.size()) {
        throw InvalidArgumentsException("invalid value for " + parameter_name + ": " + value);
    }

    if (!std::isfinite(parsed_value)) {
        throw InvalidArgumentsException("invalid value for " + parameter_name + ": " + value);
    }

    return parsed_value;
}

const std::unordered_map<std::string, FilterCreator>& GetCreators() {
    static const std::unordered_map<std::string, FilterCreator> CREATORS = {
        {
            "crop",
            [](const std::vector<std::string>& parameters) {
                RequireParameterCount(parameters, 2, "crop");

                const std::size_t width = ParsePositiveSize(parameters[0], "crop width");
                const std::size_t height = ParsePositiveSize(parameters[1], "crop height");

                return std::make_unique<CropFilter>(width, height);
            },
        },
        {
            "gs",
            [](const std::vector<std::string>& parameters) {
                RequireParameterCount(parameters, 0, "gs");
                return std::make_unique<GrayscaleFilter>();
            },
        },
        {
            "neg",
            [](const std::vector<std::string>& parameters) {
                RequireParameterCount(parameters, 0, "neg");
                return std::make_unique<NegativeFilter>();
            },
        },
        {
            "sharp",
            [](const std::vector<std::string>& parameters) {
                RequireParameterCount(parameters, 0, "sharp");
                return std::make_unique<SharpeningFilter>();
            },
        },
        {
            "edge",
            [](const std::vector<std::string>& parameters) {
                RequireParameterCount(parameters, 1, "edge");
                const double threshold = ParseDouble(parameters[0], "edge threshold");
                return std::make_unique<EdgeDetectionFilter>(threshold);
            },
        },
        {
            "blur",
            [](const std::vector<std::string>& parameters) {
                RequireParameterCount(parameters, 1, "blur");
                const double sigma = ParseDouble(parameters[0], "blur sigma");
                return std::make_unique<GaussianBlurFilter>(sigma);
            },
        },
        {
            "vignette",
            [](const std::vector<std::string>& parameters) {
                RequireParameterCount(parameters, 1, "vignette");
                const double radius = ParseDouble(parameters[0], "vignette radius");
                return std::make_unique<VignetteFilter>(radius);
            },
        },
    };

    return CREATORS;
}

}  // namespace

std::vector<std::unique_ptr<Filter>> FilterFactory::Create(const std::vector<FilterRequest>& filter_requests) const {
    std::vector<std::unique_ptr<Filter>> filters;
    filters.reserve(filter_requests.size());

    const auto& creators = GetCreators();

    for (const FilterRequest& request : filter_requests) {
        const auto iterator = creators.find(request.name);
        if (iterator == creators.end()) {
            throw UnknownFilterException(request.name);
        }

        filters.push_back(iterator->second(request.parameters));
    }

    return filters;
}
