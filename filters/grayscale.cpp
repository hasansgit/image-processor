#include "grayscale.h"
#include <cmath>
#include <cstdint>
#include <stdexcept>

void GrayScale::Apply(bmp::BMPImage& image) {
    for (auto& row : image.image) {
        for (auto& pixel : row) {
            const auto r_coef = 0.299;
            const auto g_coef = 0.587;
            const auto b_coef = 0.114;
            uint8_t gray_scale =
                static_cast<uint8_t>(std::round((pixel.R) * r_coef + (pixel.G) * g_coef + (pixel.B) * b_coef));

            pixel = {gray_scale, gray_scale, gray_scale};
        }
    }
}

void GrayScale::ParseArgs(const std::vector<std::string>& args) {
    if (!args.empty()) {
        throw std::runtime_error("Grayscale don't need arguments.");
    }
}
