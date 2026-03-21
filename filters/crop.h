#include <cstdint>
#include <vector>
#include "filter.h"

class Crop : public Filter {
public:
    void Apply(bmp::BMPImage &image) override;

    void ParseArgs(const std::vector<std::string> &args) override;

private:
    int32_t width_;
    int32_t height_;
};
