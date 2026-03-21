#include <string>
#include "../bmp/bmp.h"

void ReadBMP(const std::string& path, bmp::BMPImage& image);

void WriteImage(const std::string& path, bmp::BMPImage& image);
