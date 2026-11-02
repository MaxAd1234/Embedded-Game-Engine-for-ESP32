# Install script for directory: /Users/maximilianadolenko/.espressif/v6.0/esp-idf

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Users/maximilianadolenko/.espressif/tools/riscv32-esp-elf/esp-15.2.0_20251204/riscv32-esp-elf/bin/riscv32-esp-elf-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/riscv/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_stdio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_gpspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_clock/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_mspi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_blockdev/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_security/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/bootloader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esptool_py/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/partition_table/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_app_format/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_bootloader_format/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/app_update/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_partition/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/efuse/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_timg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_security/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_driver_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_uart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_pm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_mm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_driver_dma/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_wdt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_ana_conv/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_rtc_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/bootloader_support/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/spi_flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_usb_cdc_rom_console/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_system/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_common/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_rom/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/hal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/log/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/heap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_usb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_pmu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_touch_sens/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_i2s/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_cam/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hw_support/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/freertos/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_libc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/pthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/cxx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/Input/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_ringbuf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_driver_i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_psram/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_driver_spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_parlio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_driver_bitscrambler/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_driver_parlio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_lcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_driver_i2s/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_lcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/Vectors/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/Colliders/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/Textures/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/Physics/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/GameObject/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/Tiles/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/ObjectRegister/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/TileMap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/audio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/assets/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/GameProperties/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/World/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_ledc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_driver_ledc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/esp_hal_twai/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/driver/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/espressif__cmake_utilities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/espressif__esp_lcd_ili9341/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/main/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/maximilianadolenko/Code/bachelor_thesis/Embedded-Game-Engine-for-ESP32/engine/build/esp-idf/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
