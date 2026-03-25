#pragma once

#include <string>

#include "image.h"

class BmpWriter {
public:
    void Write(const std::string& path, const Image& image) const;
};

void BMPWriter(const std::string& path, const Image& image);
