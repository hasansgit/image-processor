#include "parser.h"
#include <stdexcept>
#include <string>

ImageProcessorArgs ExtractImageProcessorArgs(int argc, const char* argv[]) {
    ImageProcessorArgs args;

    if (argc < 3) {
        throw std::runtime_error("To few arguments.");
    }

    args.input_file = argv[1];
    args.output_file = argv[2];

    if (!args.input_file.ends_with(".bmp")) {
        throw std::runtime_error("Input file extension is invalid.");
    }
    if (!args.output_file.ends_with(".bmp")) {
        throw std::runtime_error("Output file extension is invalid.");
    }

    for (int i = 3; i < argc;) {
        FilterArgs filter;
        filter.filter_name = argv[i];

        if (argv[i][0] != '-') {
            throw std::runtime_error("Invalid arguments.");
        }
        ++i;

        for (; i < argc && argv[i][0] != '-'; ++i) {
            filter.filter_args.push_back(argv[i]);
        }

        args.filters.push_back(filter);
    }
    return args;
}
