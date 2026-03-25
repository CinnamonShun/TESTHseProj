#pragma once

#include <stdexcept>
#include <string>

class ImageProcessorException : public std::runtime_error {
public:
    explicit ImageProcessorException(const std::string& message) : std::runtime_error(message) {
    }
};

class FileReadException : public ImageProcessorException {
public:
    explicit FileReadException(const std::string& path) : ImageProcessorException("Cannot open input file: " + path) {
    }
};

class FileWriteException : public ImageProcessorException {
public:
    explicit FileWriteException(const std::string& path) : ImageProcessorException("Cannot open output file: " + path) {
    }
};

class InvalidFormatException : public ImageProcessorException {
public:
    explicit InvalidFormatException(const std::string& message)
        : ImageProcessorException("Invalid BMP format: " + message) {
    }
};

class InvalidArgumentsException : public ImageProcessorException {
public:
    explicit InvalidArgumentsException(const std::string& message)
        : ImageProcessorException("Invalid arguments: " + message) {
    }
};

class UnknownFilterException : public ImageProcessorException {
public:
    explicit UnknownFilterException(const std::string& filter_name)
        : ImageProcessorException("Unknown filter: -" + filter_name) {
    }
};
