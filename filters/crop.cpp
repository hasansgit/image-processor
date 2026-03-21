#include "crop.h"
#include <algorithm>
#include <cctype>
// #include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

void Crop::Apply(bmp::BMPImage &image) {
    if (height_ > image.infoHeader.biHeight || width_ > image.infoHeader.biWidth) {
        // throw std::runtime_error("Can't crop to larger size!");
        height_ = std::min(height_, image.infoHeader.biHeight);
        width_ = std::min(width_, image.infoHeader.biWidth);
    }

    image.infoHeader.biHeight = height_;
    image.infoHeader.biWidth = width_;
    //
    // const uint32_t metainfo_size = sizeof(image.fileHeader) + sizeof(image.infoHeader);
    // image.fileHeader.bfOffset = metainfo_size;
    // image.fileHeader.bfSize = (width_ * 3 + (4 - (width_ * 3) % 4) % 4) * height_ + metainfo_size;

    std::reverse(image.image.begin(), image.image.end());
    image.image.resize(height_);
    std::reverse(image.image.begin(), image.image.end());

    for (auto &row : image.image) {
        row.resize(width_);
    }
}

void Crop::ParseArgs(const std::vector<std::string> &args) {
    if (args.size() != 2) {
        throw std::runtime_error("Crop gets only 2 arguments.");
    }

    if (args[0].size() > 4 || args[1].size() > 4) {
        throw std::runtime_error("Invalid arguments for filter Crop.");
    }

    auto is_num = [&](const std::string &str) {
        if (str[0] == '0' || str[0] == '-') {
            return false;
        }

        for (const auto &c : str) {
            if (!std::isdigit(c)) {
                return false;
            }
        }
        return true;
    };

    if (!is_num(args[0]) || !is_num(args[1])) {
        throw std::runtime_error("Invalid arguments for filter Crop.");
    }

    try {
        width_ = std::stoi(args[0]);
        height_ = std::stoi(args[1]);
    } catch (...) {
        throw std::runtime_error("Error while parsing args for Crop");
    }
}
