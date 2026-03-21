#pragma once

#include <string>
#include <vector>
#include "../bmp/bmp.h"

class Filter {
public:
    virtual void Apply(bmp::BMPImage& image) = 0;

    virtual void ParseArgs(const std::vector<std::string>& args) = 0;

    virtual ~Filter() = default;
};
