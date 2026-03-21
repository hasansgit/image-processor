#pragma once

#include <cstdint>
#include <vector>

// varType varName = value; -- must be value
// varType varName{}; -- just default

namespace bmp {

constexpr uint16_t BM = 0x4d42;
constexpr uint8_t BITPERPIXEL = 24;

struct Pixel {
    uint8_t B = 0;
    uint8_t G = 0;
    uint8_t R = 0;
};

struct BITMAPFILEHEADER {
    uint16_t bfType = BM;
    uint32_t bfSize{};
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffset{};
};

struct BITMAPINFOHEADER {
    uint32_t biSize{};
    int32_t biWidth{};
    int32_t biHeight{};
    uint16_t biPlanes = 1;
    uint16_t biBitPerPixel = BITPERPIXEL;
    uint32_t biCompression = 0;
    uint32_t biImageSize{};
    int32_t biHorRes{};
    int32_t biVertRes{};
    uint32_t biColorCount = 0;
    uint32_t biImportantColors = 0;
};

using PixelMap = std::vector<std::vector<Pixel>>;

struct BMPImage {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    PixelMap image;
};

};  // namespace bmp
