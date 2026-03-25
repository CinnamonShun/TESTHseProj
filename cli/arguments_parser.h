#pragma once

#include <string>
#include <vector>

struct FilterRequest {
    std::string name;
    std::vector<std::string> parameters;
};

struct ProgramOptions {
    std::string input_path;
    std::string output_path;
    std::vector<FilterRequest> filter_requests;
    bool show_help = false;
};

class ArgumentsParser {
public:
    ProgramOptions Parse(int argc, char* argv[]) const;
    std::string GetHelp() const;

private:
    bool IsFilterToken(const std::string& token) const;
    bool IsHelpToken(const std::string& token) const;
};
