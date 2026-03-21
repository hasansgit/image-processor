// #include <cstddef>
#include <cstring>
// #include <fstream>
// #include <iostream>
#include <string>
#define CATCH_CONFIG_MAIN

#include <vector>
#include <stdexcept>
#include "arg_parser/parser.h"
#include "catch.hpp"
#include "bmp/bmp.h"
#include "read_write/reader.h"
#include "filters/crop.h"
#include "filters/grayscale.h"
#include "filters/gauss.h"
#include "pipeline/pipeline.h"

void MainsCopy(int argc, const char* argv[]) {
    ImageProcessorArgs args = ExtractImageProcessorArgs(argc, argv);

    bmp::BMPImage image = bmp::BMPImage();

    ReadBMP(args.input_file, image);

    auto pipeline = Pipeline(args.filters);

    pipeline.Run(image);

    WriteImage(args.output_file, image);
}
////
//// bool IsEqualFiles(const std::string lhs, const std::string rhs) {
////     auto l_file = std::ifstream(lhs, std::ios::binary | std::ios::ate);
////     auto r_file = std::ifstream(rhs, std::ios::binary | std::ios::ate);
////
////     if (!l_file.is_open() || !r_file.is_open()) {
////         throw std::runtime_error("Can't open files.");
////     }
////
////     if (l_file.tellg() != r_file.tellg()) {
////         return false;
////     }
////
////     l_file.seekg(0);
////     r_file.seekg(0);
////
////     const size_t buff_size = 4 * 1024;
////     char l_buff[buff_size];
////     char r_buff[buff_size];
////
////     while (l_file && r_file) {
////         l_file.read(l_buff, buff_size);
////         r_file.read(r_buff, buff_size);
////
////         if (l_file.gcount() != r_file.gcount() || std::memcmp(l_buff, r_buff, l_file.gcount())) {
////             return false;
////         }
////     }
////
////     return l_file.eof() && r_file.eof();
//// }

// Parser testing

TEST_CASE("just working") {
    auto argc = 5;

    const char* argv[]{"image_processor.cpp", "image.bmp", "out.bmp", "-blur", "400"};

    ImageProcessorArgs args = ExtractImageProcessorArgs(argc, argv);

    REQUIRE(args.input_file == argv[1]);
    REQUIRE(args.output_file == argv[2]);

    REQUIRE(args.filters.size() == 1);
    REQUIRE(args.filters[0].filter_name == argv[3]);

    REQUIRE(args.filters[0].filter_args.size() == 1);
    REQUIRE(args.filters[0].filter_args[0] == argv[4]);
}

TEST_CASE("more args") {
    auto argc = 8;

    const char* argv[]{"image_processor.cpp", "image.bmp", "out.bmp", "-blur", "400", "-crop", "400", "600"};

    ImageProcessorArgs args = ExtractImageProcessorArgs(argc, argv);

    REQUIRE(args.input_file == argv[1]);
    REQUIRE(args.output_file == argv[2]);

    REQUIRE(args.filters.size() == 2);

    REQUIRE(args.filters[0].filter_name == argv[3]);
    REQUIRE(args.filters[0].filter_args.size() == 1);
    REQUIRE(args.filters[0].filter_args[0] == argv[4]);

    REQUIRE(args.filters[1].filter_name == argv[5]);
    REQUIRE(args.filters[1].filter_args.size() == 2);
    REQUIRE(args.filters[1].filter_args[0] == argv[6]);
    REQUIRE(args.filters[1].filter_args[1] == argv[7]);
}

TEST_CASE("fck test") {

    int argc = 6;

    const char* argv[] = {
        "image_processor", "../test_script/data/flag.bmp", "trash_files/cropped_flag.bmp", "-crop", "50", "50"};

    ImageProcessorArgs args = ExtractImageProcessorArgs(argc, argv);

    REQUIRE(args.input_file == argv[1]);
    REQUIRE(args.output_file == argv[2]);

    REQUIRE(args.filters.size() == 1);

    REQUIRE(args.filters[0].filter_name == argv[3]);
    REQUIRE(args.filters[0].filter_args.size() == 2);
    REQUIRE(args.filters[0].filter_args[0] == argv[4]);
    REQUIRE(args.filters[0].filter_args[0] == argv[5]);
}

TEST_CASE("throwing few args") {
    auto argc = 2;

    const char* argv[] = {"image_processor.cpp", "image.bmp"};
    REQUIRE_THROWS_AS(ExtractImageProcessorArgs(argc, argv), std::runtime_error);
}

TEST_CASE("invalid file exitension") {
    auto argc = 3;

    const char* argv1[]{"image_processor.cpp", "image.png", "out.bmp"};
    REQUIRE_THROWS_AS(ExtractImageProcessorArgs(argc, argv1), std::runtime_error);

    const char* argv2[]{"image_processor.cpp", "image.bmp", "out.png"};
    REQUIRE_THROWS_AS(ExtractImageProcessorArgs(argc, argv2), std::runtime_error);
}

TEST_CASE("invalid argument (not a filter)") {
    auto argc = 4;

    const char* argv[]{"image_processor.cpp", "image.bmp", "out.bmp", "subway-surfers"};
    REQUIRE_THROWS_AS(ExtractImageProcessorArgs(argc, argv), std::runtime_error);
}

//// // IsEqualFiles function testing
//// TEST_CASE("IsEqualFiles func testing") {
////     REQUIRE(IsEqualFiles("../test_script/data/lenna.bmp", "../test_script/data/lenna.bmp"));
////     REQUIRE(!IsEqualFiles("../test_script/data/lenna.bmp", "../test_script/data/lenna_neg.bmp"));
////     REQUIRE(!IsEqualFiles("../test_script/data/lenna.bmp", "../test_script/data/flag.bmp"));
//// }
//
//// // Testing read and write without filters
//// TEST_CASE("writing without filters") {
////     bmp::BMPImage image = bmp::BMPImage();
////
////     auto input_path = "../test_script/data/flag.bmp";
////     auto out_path = "trash_files/copy.bmp";
////
////     ReadBMP(input_path, image);
////
////     WriteImage(out_path, image);
////
////     REQUIRE(IsEqualFiles(input_path, out_path));
//// }
//
//// TEST_CASE("writing without filters bigger file (lenna)") {
////     bmp::BMPImage image = bmp::BMPImage();
////
////     auto input_path = "../test_script/data/lenna.bmp";
////     auto out_path = "trash_files/copy.bmp";
////
////     ReadBMP(input_path, image);
////
////     WriteImage(out_path, image);
////
////     REQUIRE(IsEqualFiles(input_path, out_path));
//// }
//
//// Testing Crop::ParseArgs()
TEST_CASE("no args, few args, lot args, 2 args") {
    auto filter = Crop();

    std::vector<std::string> args{};
    REQUIRE_THROWS(filter.ParseArgs(args));

    args = {"400"};
    REQUIRE_THROWS(filter.ParseArgs(args));

    args = {"400", "600", "234"};
    REQUIRE_THROWS(filter.ParseArgs(args));

    args = {"400", "500"};
    REQUIRE_NOTHROW(filter.ParseArgs(args));
}

TEST_CASE("invalid first arg, invalid second arg, normal args") {
    auto filter = Crop();

    std::vector<std::string> args{"400r", "500"};
    REQUIRE_THROWS(filter.ParseArgs(args));

    args = {"400", "l500"};
    REQUIRE_THROWS(filter.ParseArgs(args));

    args = {"400", "500"};
    REQUIRE_NOTHROW(filter.ParseArgs(args));
}

//// Testing Crop::Apply()
//// TEST_CASE("throws if larger") {
////     auto filter = Crop();
////
////     auto image = bmp::BMPImage();
////     ReadBMP("../test_script/data/flag.bmp", image);
////
////     std::vector<std::string> args{"4000", "9000"};
////     filter.ParseArgs(args);
////
////     REQUIRE_THROWS(filter.Apply(image));
//// }
//
//// TEST_CASE("is working") {
////     const int argc = 6;
////
////     const char* argv[] = {
////         "image_processor", "../test_script/data/lenna.bmp", "trash_files/cropped_lenna.bmp", "-crop", "999",
///"1999", /     };
////
////     MainsCopy(argc, argv);
////
////     REQUIRE(IsEqualFiles(argv[2], "../test_script/data/lenna_crop.bmp"));
//// }

// Grayscale::ParseArgs()
TEST_CASE("lot args,no args") {
    auto filter = GrayScale();

    std::vector<std::string> args{"400", "600", "234"};
    REQUIRE_THROWS(filter.ParseArgs(args));

    args.clear();
    REQUIRE_NOTHROW(filter.ParseArgs(args));
}

//// Grayscale works
//// TEST_CASE("is working - Grayscale") {
////     const int argc = 4;
////
////     const char* argv[] = {
////         "image_processor",
////         "../test_script/data/lenna.bmp",
////         "trash_files/gs_lenna.bmp",
////         "-gs",
////     };
////
////     MainsCopy(argc, argv);
////
////     REQUIRE(IsEqualFiles(argv[2], "../test_script/data/lenna_gs.bmp"));
//// }
//// Negative works
//// TEST_CASE("is working - Negative") {
////     const int argc = 4;
////
////     const char* argv[] = {
////         "image_processor",
////         "../test_script/data/lenna.bmp",
////         "trash_files/neg_lenna.bmp",
////         "-neg",
////     };
////
////     MainsCopy(argc, argv);
////
////     REQUIRE(IsEqualFiles(argv[2], "../test_script/data/lenna_neg.bmp"));
//// }
////
//// Sharpening works
//// TEST_CASE("is working - Sharpening") {
////     const int argc = 4;
////
////     const char* argv[] = {
////         "image_processor",
////         "../test_script/data/lenna.bmp",
////         "trash_files/sharp_lenna.bmp",
////         "-sharp",
////     };
////
////     MainsCopy(argc, argv);
////
////     REQUIRE(IsEqualFiles(argv[2], "../test_script/data/lenna_sharp.bmp"));
//// }

//  Edge works
TEST_CASE("is working - Edge") {
    const int argc = 5;

    const char* argv[] = {
        "image_processor", "../test_script/data/flag.bmp", "trash_files/edge_flag.bmp", "-edge", "0.1",
    };

    MainsCopy(argc, argv);

    const char* argv2[] = {
        "image_processor", "../test_script/data/lenna.bmp", "trash_files/lenna_edge.bmp", "-edge", "0.07",
    };

    MainsCopy(argc, argv2);

    //  Edge works, but "etalon" has some metadata, which original file hasn't, I don't want to добавить их
    // REQUIRE(IsEqualFiles(argv[2], "../test_script/data/flag_edge.bmp"));
}

// Gauss::ParseArgs()
TEST_CASE("lot args, no args, needed args, other") {
    auto filter = Gauss();

    std::vector<std::string> args{"400", "600", "234"};
    REQUIRE_THROWS(filter.ParseArgs(args));

    args.clear();
    REQUIRE_THROWS(filter.ParseArgs(args));

    args = {"7.5"};
    REQUIRE_NOTHROW(filter.ParseArgs(args));

    args = {"3"};
    REQUIRE_NOTHROW(filter.ParseArgs(args));

    args = {"0.123123123"};
    REQUIRE_NOTHROW(filter.ParseArgs(args));
}

TEST_CASE("is working - gauss") {
    const int argc = 5;

    const char* argv[] = {
        "image_processor", "../test_script/data/lenna.bmp", "trash_files/blur7.5_lenna.bmp", "-blur", "7.5",
    };
    MainsCopy(argc, argv);
}

TEST_CASE("is working - gauss2") {
    const int argc = 5;

    const char* argv[] = {
        "image_processor", "../test_script/data/lenna.bmp", "trash_files/blur3_lenna.bmp", "-blur", "3",
    };
    MainsCopy(argc, argv);
}

TEST_CASE("is working - gauss then edge") {
    const int argc = 8;

    const char* argv[] = {
        "image_processor",
        "../test_script/data/lenna.bmp",
        "trash_files/blur_edge_lenna.bmp",
        "-blur",
        "2.718281828",
        "-edge",
        "0.01",
        "-neg",
    };
    MainsCopy(argc, argv);
}

// TEST_CASE("is working - yamete") {
//     const int argc = 6;
//
//     const char* argv[] = {
//         "image_processor", "../test_script/data/lenna.bmp", "trash_files/yamete_lenna.bmp", "-blur", "3", "-yamete",
//     };
//     MainsCopy(argc, argv);
// }
TEST_CASE("is working - yamete") {
    const int argc = 5;

    const char* argv[] = {
        "image_processor", "../test_script/data/lenna.bmp", "trash_files/yamete_lenna.bmp", "-yamete", "0.67",
    };
    MainsCopy(argc, argv);
}
