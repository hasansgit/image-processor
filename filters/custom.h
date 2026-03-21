#include <vector>
#include "filter.h"

class Custom : public Filter {
public:
    void Apply(bmp::BMPImage &image) override;

    void ParseArgs(const std::vector<std::string> &args) override;

private:
    double Dist(bmp::Pixel lhs, bmp::Pixel rhs);
    double eps_;
};
