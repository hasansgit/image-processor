#include "negative.h"
#include <cmath>
#include <stdexcept>

void Negative::Apply(bmp::BMPImage& image) {

    for (auto& row : image.image) {
        for (auto& pixel : row) {
            pixel.B = -pixel.B - 1;
            pixel.G = -pixel.G - 1;
            pixel.R = -pixel.R - 1;
        }
    }
}

void Negative::ParseArgs(const std::vector<std::string>& args) {
    if (!args.empty()) {
        throw std::runtime_error("Negative don't need arguments.");
    }
}
