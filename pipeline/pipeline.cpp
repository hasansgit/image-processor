#include "pipeline.h"
#include "../filters/crop.h"
#include "../filters/grayscale.h"
#include "../filters/negative.h"
#include "../filters/sharpening.h"
#include "../filters/edge.h"
#include "../filters/gauss.h"
#include "../filters/custom.h"
#include <memory>
#include <stdexcept>
#include <vector>

std::unique_ptr<Filter> Pipeline::MakeFilter(const std::string& filter_name) {
    if (filter_name == "-crop") {
        auto filter = std::make_unique<Crop>();
        return filter;
    } else if (filter_name == "-gs") {
        auto filter = std::make_unique<GrayScale>();
        return filter;
    } else if (filter_name == "-neg") {
        auto filter = std::make_unique<Negative>();
        return filter;
    } else if (filter_name == "-sharp") {
        auto filter = std::make_unique<Sharpening>();
        return filter;
    } else if (filter_name == "-edge") {
        auto filter = std::make_unique<Edge>();
        return filter;
    } else if (filter_name == "-blur") {
        auto filter = std::make_unique<Gauss>();
        return filter;
    } else if (filter_name == "-yamete") {
        auto filter = std::make_unique<Custom>();
        return filter;
    }

    throw std::runtime_error("Unknown filter: " + filter_name);
}

Pipeline::Pipeline(const std::vector<FilterArgs>& args) {
    filters_.clear();

    for (const auto& f_args : args) {
        filters_.push_back(MakeFilter(f_args.filter_name));
        filters_.back()->ParseArgs(f_args.filter_args);
    }
}

void Pipeline::Run(bmp::BMPImage& image) {
    for (auto& filter : filters_) {
        filter->Apply(image);
    }
};
