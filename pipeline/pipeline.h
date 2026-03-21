#include <memory>
#include <vector>

#include "../filters/filter.h"
#include "../arg_parser/parser.h"
#include "../bmp/bmp.h"

std::unique_ptr<Filter> MakeFilter(const std::string& filter_name);

class Pipeline {
public:
    explicit Pipeline(const std::vector<FilterArgs>& args);

    void Run(bmp::BMPImage& image);

private:
    std::unique_ptr<Filter> MakeFilter(const std::string& filter_name);
    std::vector<std::unique_ptr<Filter>> filters_;
};
