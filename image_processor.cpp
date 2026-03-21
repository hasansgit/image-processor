#include "arg_parser/parser.h"
#include "help.h"
#include "bmp/bmp.h"
#include "read_write/reader.h"
#include "pipeline/pipeline.h"

int main(int argc, const char* argv[]) {

    if (argc == 1) {
        Help();
        return 0;
    }

    ImageProcessorArgs args = ExtractImageProcessorArgs(argc, argv);

    bmp::BMPImage image = bmp::BMPImage();

    ReadBMP(args.input_file, image);

    auto pipeline = Pipeline(args.filters);

    pipeline.Run(image);

    WriteImage(args.output_file, image);

    return 0;
}
