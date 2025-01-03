# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(PROJECT_VER "2.0.0.1")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(akil_01)
spiffs_create_partition_image(storage data FLASH_IN_PROJECT)

add_compile_options("-Wno-deprecated-declarations")
add_compile_options("-Wno-missing-field-initializers")
add_compile_options("-Wno-deprecated-enum-enum-conversion")