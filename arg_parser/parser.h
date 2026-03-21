#pragma once

#include <string>
#include <vector>

struct FilterArgs {
    std::string filter_name;
    std::vector<std::string> filter_args;
};

struct ImageProcessorArgs {
    std::string input_file;
    std::string output_file;

    std::vector<FilterArgs> filters;
};

ImageProcessorArgs ExtractImageProcessorArgs(int argc, const char* argv[]);
