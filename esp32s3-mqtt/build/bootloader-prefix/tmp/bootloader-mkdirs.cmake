# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/ESP-IDF/v5.3/esp-idf/components/bootloader/subproject"
  "D:/Git ESP/phys-esp32-firmware/esp32s3-mqtt/build/bootloader"
  "D:/Git ESP/phys-esp32-firmware/esp32s3-mqtt/build/bootloader-prefix"
  "D:/Git ESP/phys-esp32-firmware/esp32s3-mqtt/build/bootloader-prefix/tmp"
  "D:/Git ESP/phys-esp32-firmware/esp32s3-mqtt/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Git ESP/phys-esp32-firmware/esp32s3-mqtt/build/bootloader-prefix/src"
  "D:/Git ESP/phys-esp32-firmware/esp32s3-mqtt/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Git ESP/phys-esp32-firmware/esp32s3-mqtt/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Git ESP/phys-esp32-firmware/esp32s3-mqtt/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
