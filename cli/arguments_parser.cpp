#include "arguments_parser.h"

#include <cctype>

#include "../exceptions.h"

ProgramOptions ArgumentsParser::Parse(const int argc, char* argv[]) const {
    ProgramOptions options;

    if (argc == 1) {
        options.show_help = true;
        return options;
    }

    int index = 1;
    while (index < argc) {
        const std::string token = argv[index];
        if (IsHelpToken(token)) {
            options.show_help = true;
            return options;
        }
        ++index;
    }

    if (argc < 3) {
        throw InvalidArgumentsException("input and output files are required");
    }

    options.input_path = argv[1];
    options.output_path = argv[2];

    index = 3;
    while (index < argc) {
        const std::string token = argv[index];
        if (!IsFilterToken(token)) {
            throw InvalidArgumentsException("expected filter name, got: " + token);
        }

        FilterRequest request;
        request.name = token.substr(1);

        ++index;

        while (index < argc) {
            const std::string parameter = argv[index];
            if (IsFilterToken(parameter)) {
                break;
            }
            request.parameters.push_back(parameter);
            ++index;
        }

        options.filter_requests.push_back(request);
    }

    return options;
}

std::string ArgumentsParser::GetHelp() const {
    return "image_processor\n"
           "BMP image filter tool\n\n"
           "Usage:\n"
           "  image_processor <input.bmp> <output.bmp> [filters]\n"
           "  image_processor -h\n"
           "  image_processor --help\n\n"
           "Filters:\n"
           "  -crop width height    Crop from top-left corner\n"
           "  -gs                   Grayscale\n"
           "  -neg                  Negative\n"
           "  -sharp                Sharpening\n"
           "  -edge threshold       Edge detection, threshold in [0, 1]\n"
           "  -blur sigma           Gaussian blur, sigma > 0\n"
           "  -vignette radius      Vignette, radius > 0\n\n"
           "Example:\n"
           "  image_processor in.bmp out.bmp -crop 800 600 -gs -blur 0.7\n";
}

bool ArgumentsParser::IsFilterToken(const std::string& token) const {
    if (token.size() < 2) {
        return false;
    }

    if (token[0] != '-') {
        return false;
    }

    return std::isalpha(static_cast<unsigned char>(token[1])) != 0;
}

bool ArgumentsParser::IsHelpToken(const std::string& token) const {
    return token == "-h" || token == "--help" || token == "-help" || token == "help";
}
