#include "gauss.h"
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<double> CreateKernel(double sigma) {
    int radius = std::ceil(3 * sigma);
    int sze = radius * 2 + 1;

    std::vector<double> ker(sze);
    double sum{};

    for (int i = 0; i < sze; ++i) {
        int x = i - radius;
        ker[i] = std::exp(-(x * x) / (2 * sigma * sigma));
        sum += ker[i];
    }

    for (auto& i : ker) {
        i /= sum;
    }

    return ker;
}

void Gauss::Apply(bmp::BMPImage& image) {
    const auto width = image.infoHeader.biWidth;
    const auto height = image.infoHeader.biHeight;
    const double magic_number = 255;

    auto nearest_pixel = [&](const auto& row, const auto& col, const bmp::PixelMap& pm) {
        auto t_row = std::max(0, std ::min(row, width - 1));
        auto t_col = std::max(0, std::min(col, height - 1));

        return pm[t_row][t_col];
    };

    auto ker = CreateKernel(sigma_);
    bmp::PixelMap tmp = image.image;

    const int radius = std::ceil(3 * sigma_);

    // horizontal
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            double r{};
            double g{};
            double b{};
            for (int k = 0; k < ker.size(); ++k) {
                int t_col = col + (k - radius);
                bmp::Pixel tp = nearest_pixel(row, t_col, image.image);

                r += static_cast<double>(tp.R) / magic_number * ker[k];
                g += static_cast<double>(tp.G) / magic_number * ker[k];
                b += static_cast<double>(tp.B) / magic_number * ker[k];
            }
            tmp[row][col].R = static_cast<uint8_t>(r * magic_number);
            tmp[row][col].G = static_cast<uint8_t>(g * magic_number);
            tmp[row][col].B = static_cast<uint8_t>(b * magic_number);
        }
    }
    // vertical
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            double r{};
            double g{};
            double b{};
            for (int k = 0; k < ker.size(); ++k) {
                int t_row = row + (k - radius);
                bmp::Pixel tp = nearest_pixel(t_row, col, tmp);

                r += static_cast<double>(tp.R) / magic_number * ker[k];
                g += static_cast<double>(tp.G) / magic_number * ker[k];
                b += static_cast<double>(tp.B) / magic_number * ker[k];
            }
            image.image[row][col].R = static_cast<uint8_t>(r * magic_number);
            image.image[row][col].G = static_cast<uint8_t>(g * magic_number);
            image.image[row][col].B = static_cast<uint8_t>(b * magic_number);
        }
    }
}

void Gauss::ParseArgs(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("Gaussian blur needs only 1 argument.");
    }

    try {
        sigma_ = std::stod(args[0]);
    } catch (...) {
        throw std::runtime_error("Error while parsing Edge-detecting arguments.");
    }
}
