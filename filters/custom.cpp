#include "custom.h"
#include "gauss.h"
#include <cmath>
#include <cstdint>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

double Custom::Dist(bmp::Pixel lhs, bmp::Pixel rhs) {
    const double red_coef = 0.299;
    const double blue_coeff = 0.114;
    const double green_coef = 0.587;

    return std::pow(rhs.B - lhs.B, 2) * blue_coeff + std::pow(rhs.R - lhs.R, 2) * red_coef +
           std::pow(rhs.G - lhs.G, 2) * green_coef;
}

void Custom::Apply(bmp::BMPImage& image) {
    auto gauss = Gauss();
    gauss.ParseArgs({"3"});
    gauss.Apply(image);

    // const double eps_ = 0.67;

    std::vector<std::vector<bool>> used;
    used.resize(image.infoHeader.biHeight, std::vector<bool>(image.infoHeader.biWidth, false));

    for (int32_t i = 0; i < image.infoHeader.biHeight; ++i) {
        for (int32_t j = 0; j < image.infoHeader.biWidth; ++j) {

            if (used[i][j]) {
                continue;
            }

            uint64_t r = 0;
            uint64_t g = 0;
            uint64_t b = 0;

            std::queue<std::pair<int32_t, int32_t>> q;
            q.emplace(i, j);

            std::queue<std::pair<int32_t, int32_t>> marked;

            while (!q.empty()) {
                int32_t row = q.front().first;
                int32_t col = q.front().second;

                q.pop();

                auto check = [&](int32_t t_row, int32_t t_col) {
                    return t_row >= 0 && t_row < image.infoHeader.biHeight && t_col >= 0 &&
                           t_col < image.infoHeader.biWidth && !used[t_row][t_col] &&
                           Dist(image.image[row][col], image.image[t_row][t_col]) < eps_;
                };

                if (used[row][col]) {
                    continue;
                }

                used[row][col] = true;
                r += image.image[row][col].R;
                g += image.image[row][col].G;
                b += image.image[row][col].B;

                marked.emplace(row, col);

                if (check(row + 1, col)) {
                    q.emplace(row + 1, col);
                }
                if (check(row, col + 1)) {
                    q.emplace(row, col + 1);
                }
                if (check(row - 1, col)) {
                    q.emplace(row - 1, col);
                }
                if (check(row, col - 1)) {
                    q.emplace(row, col - 1);
                }
            }

            if (marked.empty()) {
                continue;
            }
            r /= marked.size();
            g /= marked.size();
            b /= marked.size();
            while (!marked.empty()) {
                auto row = marked.front().first;
                auto col = marked.front().second;

                marked.pop();

                image.image[row][col] = {static_cast<uint8_t>(b), static_cast<uint8_t>(g), static_cast<uint8_t>(r)};
            }
        }
    }
}

void Custom::ParseArgs(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("Yamete blur needs only 1 argument.");
    }

    try {
        eps_ = std::stod(args[0]);
    } catch (...) {
        throw std::runtime_error("Error while parsing Yamete arguments.");
    }
}
