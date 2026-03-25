#pragma once

#include <string>

#include "image.h"

class BmpReader {
public:
    Image Read(const std::string& path) const;
};

Image BMPReader(const std::string& path);
