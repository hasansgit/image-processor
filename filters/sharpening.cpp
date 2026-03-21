#include "sharpening.h"

#include <math.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <limits>
#include <stdexcept>

void Sharpening::Apply(bmp::BMPImage& image) {
    const auto width = image.infoHeader.biWidth;
    const auto height = image.infoHeader.biHeight;

    auto apply_matrix = [&](auto i, auto j) {
        const auto m_coef = 5.0;

        const double i_mx = std::numeric_limits<uint8_t>::max();

        double res_b{};
        double res_g{};
        double res_r{};

        res_b = (image.image[i][j].B / i_mx) * m_coef;
        res_g = (image.image[i][j].G / i_mx) * m_coef;
        res_r = (image.image[i][j].R / i_mx) * m_coef;

        auto x = i;
        auto y = j;

        if (x > 0) {
            --x;
        }
        res_b -= (image.image[x][j].B / i_mx);
        res_g -= (image.image[x][j].G / i_mx);
        res_r -= (image.image[x][j].R / i_mx);

        if (i + 1 < height) {
            x = i + 1;
        } else {
            x = i;
        }
        res_b -= (image.image[x][j].B / i_mx);
        res_g -= (image.image[x][j].G / i_mx);
        res_r -= (image.image[x][j].R / i_mx);

        if (y > 0) {
            --y;
        }
        res_b -= (image.image[i][y].B / i_mx);
        res_g -= (image.image[i][y].G / i_mx);
        res_r -= (image.image[i][y].R / i_mx);

        if (j + 1 < width) {
            y = j + 1;
        } else {
            y = j;
        }
        res_b -= (image.image[i][y].B / i_mx);
        res_g -= (image.image[i][y].G / i_mx);
        res_r -= (image.image[i][y].R / i_mx);

        res_b = std::min(1.0, std::max(0.0, res_b));
        res_g = std::min(1.0, std::max(0.0, res_g));
        res_r = std::min(1.0, std::max(0.0, res_r));

        res_b = std::round(res_b * i_mx);
        res_g = std::round(res_g * i_mx);
        res_r = std::round(res_r * i_mx);

        return bmp::Pixel(static_cast<uint8_t>(res_b), static_cast<uint8_t>(res_g), static_cast<uint8_t>(res_r));
    };

    auto tmp_image = image.image;

    for (auto i = 0; i < height; ++i) {
        for (auto j = 0; j < width; ++j) {
            tmp_image[i][j] = apply_matrix(i, j);
        }
    }

    image.image = tmp_image;
}

void Sharpening::ParseArgs(const std::vector<std::string>& args) {
    if (!args.empty()) {
        throw std::runtime_error("Sharpening don't need arguments.");
    }
}
