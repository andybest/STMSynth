

set(CMAKE_TOOLCHAIN_FILE "arm-none-eabi-gcc.cmake")

set(STM32_FAMILY "STM32F4xx")

find_package(${STM32_FAMILY} REQUIRED)
find_package(STLinkUtility)