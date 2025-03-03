# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(PROJECT_VER "2.0.0.8")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(akil_01)
spiffs_create_partition_image(storage data FLASH_IN_PROJECT)

add_compile_options("-Wno-deprecated-declarations")
add_compile_options("-Wno-missing-field-initializers")
add_compile_options("-Wno-deprecated-enum-enum-conversion")

# 2.0.0.3 07.01.2025
   #lokal röleler eklendi
# 2.0.0.4 12.01.2025
   #Termostat düzenlendi. Min/Max Temp ve Açılışta V_STAT Eklendi
# 2.0.0.5 16.01.2025
   #TFTP ota ve disk serverlar eklendi   
# 2.0.0.6 15.02.2025
   #I2C library düzenlendi.
# 2.0.0.7 18.02.2025
   #Json kurtköye göre düzenlendi   
# 2.0.0.8 03.03.2025
   #Termostat Lokal/Remote olarak düzenlendi
       

      