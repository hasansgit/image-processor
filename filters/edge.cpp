#include "edge.h"
#include "grayscale.h"

#include <math.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <limits>
#include <stdexcept>
#include <string>

void Edge::Apply(bmp::BMPImage& image) {
    auto filter = GrayScale();
    filter.Apply(image);

    const auto width = image.infoHeader.biWidth;
    const auto height = image.infoHeader.biHeight;

    auto apply_matrix = [&](auto i, auto j) {
        const auto m_coef = 4.0;

        const double i_mx = std::numeric_limits<uint8_t>::max();

        double res_r{};

        res_r = (image.image[i][j].R / i_mx) * m_coef;

        auto x = i;
        auto y = j;

        if (x > 0) {
            --x;
        }
        res_r -= (image.image[x][j].R / i_mx);

        if (i + 1 < height) {
            x = i + 1;
        } else {
            x = i;
        }
        res_r -= (image.image[x][j].R / i_mx);

        if (y > 0) {
            --y;
        }
        res_r -= (image.image[i][y].R / i_mx);

        if (j + 1 < width) {
            y = j + 1;
        } else {
            y = j;
        }
        res_r -= (image.image[i][y].R / i_mx);

        res_r = std::min(1.0, std::max(0.0, res_r));

        const uint8_t mx = static_cast<uint8_t>(i_mx);

        return res_r > threshold_ ? bmp::Pixel(mx, mx, mx) : bmp::Pixel(0, 0, 0);
    };

    auto tmp_image = image.image;

    for (auto i = 0; i < height; ++i) {
        for (auto j = 0; j < width; ++j) {
            tmp_image[i][j] = apply_matrix(i, j);
        }
    }

    image.image = tmp_image;
}

void Edge::ParseArgs(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("Edge detecting needs only 1 argument.");
    }

    try {
        threshold_ = std::stod(args[0]);
    } catch (...) {
        throw std::runtime_error("Error while parsing Edge-detecting arguments.");
    }
}
