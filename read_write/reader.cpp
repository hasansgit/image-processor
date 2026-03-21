#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "reader.h"

void ReadBMP(const std::string &path, bmp::BMPImage &image) {
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("File not found: " + path);
    }

    file.read(reinterpret_cast<char *>(&image.fileHeader.bfType), sizeof(image.fileHeader.bfType));
    file.read(reinterpret_cast<char *>(&image.fileHeader.bfSize), sizeof(image.fileHeader.bfSize));
    file.read(reinterpret_cast<char *>(&image.fileHeader.bfReserved1), sizeof(image.fileHeader.bfReserved1));
    file.read(reinterpret_cast<char *>(&image.fileHeader.bfReserved2), sizeof(image.fileHeader.bfReserved2));
    file.read(reinterpret_cast<char *>(&image.fileHeader.bfOffset), sizeof(image.fileHeader.bfOffset));

    file.read(reinterpret_cast<char *>(&image.infoHeader.biSize), sizeof(image.infoHeader.biSize));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biWidth), sizeof(image.infoHeader.biWidth));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biHeight), sizeof(image.infoHeader.biHeight));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biPlanes), sizeof(image.infoHeader.biPlanes));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biBitPerPixel), sizeof(image.infoHeader.biBitPerPixel));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biCompression), sizeof(image.infoHeader.biCompression));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biImageSize), sizeof(image.infoHeader.biImageSize));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biHorRes), sizeof(image.infoHeader.biHorRes));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biVertRes), sizeof(image.infoHeader.biVertRes));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biColorCount), sizeof(image.infoHeader.biColorCount));
    file.read(reinterpret_cast<char *>(&image.infoHeader.biImportantColors),
              sizeof(image.infoHeader.biImportantColors));

    if (image.fileHeader.bfType != bmp::BM) {
        throw std::runtime_error("File signature is invalid");
    }

    if (image.infoHeader.biBitPerPixel != bmp::BITPERPIXEL || image.infoHeader.biCompression != 0) {
        throw std::runtime_error("File format is invalid");
    }

    if (image.infoHeader.biHeight < 0) {
        throw std::runtime_error("Why your image is starting from up?");
    }

    // file.seekg(image.fileHeader.bfOffset);

    const uint32_t width = image.infoHeader.biWidth;
    const uint32_t height = image.infoHeader.biHeight;

    const uint32_t holes = ((4 - (width * 3) % 4) % 4);
    const uint32_t str_len = width * 3 + holes;

    const uint32_t image_size = str_len * height;

    std::vector<uint8_t> pixel_data(image_size);
    file.read(reinterpret_cast<char *>(pixel_data.data()), image_size);

    if (!file) {
        throw std::runtime_error("Error while reading image");
    }

    image.image.resize(height, std::vector<bmp::Pixel>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            uint32_t ind = i * str_len + j * 3;
            image.image[i][j] = {pixel_data[ind], pixel_data[ind + 1], pixel_data[ind + 2]};
        }
    }
}

void WriteImage(const std::string &path, bmp::BMPImage &image) {
    try {
        std::ofstream file(path, std::ios::binary);

        file.write(reinterpret_cast<char *>(&image.fileHeader.bfType), sizeof(image.fileHeader.bfType));
        file.write(reinterpret_cast<char *>(&image.fileHeader.bfSize), sizeof(image.fileHeader.bfSize));
        file.write(reinterpret_cast<char *>(&image.fileHeader.bfReserved1), sizeof(image.fileHeader.bfReserved1));
        file.write(reinterpret_cast<char *>(&image.fileHeader.bfReserved2), sizeof(image.fileHeader.bfReserved2));
        file.write(reinterpret_cast<char *>(&image.fileHeader.bfOffset), sizeof(image.fileHeader.bfOffset));

        file.write(reinterpret_cast<char *>(&image.infoHeader.biSize), sizeof(image.infoHeader.biSize));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biWidth), sizeof(image.infoHeader.biWidth));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biHeight), sizeof(image.infoHeader.biHeight));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biPlanes), sizeof(image.infoHeader.biPlanes));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biBitPerPixel), sizeof(image.infoHeader.biBitPerPixel));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biCompression), sizeof(image.infoHeader.biCompression));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biImageSize), sizeof(image.infoHeader.biImageSize));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biHorRes), sizeof(image.infoHeader.biHorRes));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biVertRes), sizeof(image.infoHeader.biVertRes));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biColorCount), sizeof(image.infoHeader.biColorCount));
        file.write(reinterpret_cast<char *>(&image.infoHeader.biImportantColors),
                   sizeof(image.infoHeader.biImportantColors));

        const int holes = (4 - (image.infoHeader.biWidth * 3) % 4) % 4;
        for (const auto &row : image.image) {
            for (const auto &pixel : row) {
                file.put(static_cast<char>(pixel.B));
                file.put(static_cast<char>(pixel.G));
                file.put(static_cast<char>(pixel.R));
            }

            for (int i = 0; i < holes; ++i) {
                file.put(0);
            }
        }

    } catch (...) {
        throw std::runtime_error("Error while writing image.");
    }
}
