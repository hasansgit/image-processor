project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_EXE test.cpp)
set(SOURCE_EXE_PROJ image_processor.cpp)

set(SOURCE_LIBS
    arg_parser/parser.cpp
    read_write/reader.cpp
    filters/crop.cpp
    filters/grayscale.cpp
    filters/negative.cpp
    filters/sharpening.cpp
    filters/edge.cpp
    filters/gauss.cpp
    filters/custom.cpp
    pipeline/pipeline.cpp
)

add_library(libs STATIC ${SOURCE_LIBS})

target_include_directories(libs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/arg_parser
    ${CMAKE_CURRENT_SOURCE_DIR}/read_write
    ${CMAKE_CURRENT_SOURCE_DIR}/bmp
    ${CMAKE_CURRENT_SOURCE_DIR}/filter
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline
)

add_executable(tester ${SOURCE_EXE})
add_executable(image_processor ${SOURCE_EXE_PROJ})

target_link_libraries(tester PRIVATE libs)
target_link_libraries(image_processor PRIVATE libs)

