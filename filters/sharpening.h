#include <vector>
#include "filter.h"

class Sharpening : public Filter {
public:
    void Apply(bmp::BMPImage &image) override;

    void ParseArgs(const std::vector<std::string> &args) override;
};
