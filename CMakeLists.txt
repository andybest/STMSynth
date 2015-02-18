project(STMSynth C CXX ASM)
cmake_minimum_required(VERSION 2.8)

set(OUTPUT_NAME STMSynth)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

include(CMake/STM32.cmake)

set(SYNTHIA_INCLUDES ${CMAKE_SOURCE_DIR}/src/synthia)
file(GLOB SYNTHIA_SOURCES ${CMAKE_SOURCE_DIR}/src/synthia/*.cc)

#set(USB_CORE_INCLUDES ${CMAKE_SOURCE_DIR}/Middleware/STM32_USB_Device_Library/Core/Inc)
#file(GLOB USB_CORE_SOURCES ${CMAKE_SOURCE_DIR}/Middleware/STM32_USB_Device_Library/Core/Src/*.c)

include_directories(
    ${CMAKE_SOURCE_DIR}/Src
    ${SYNTHIA_INCLUDES}
    ${STM32_INCLUDE_DIRS}
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_SOURCE_DIR}/CMake/STM32F407VG_FLASH.ld")

add_executable(${OUTPUT_NAME}.elf
    src/main.cc
    src/syscalls.c
    src/system_stm32f4xx.c
    src/stm32f4xx_it.c
    ${SYNTHIA_SOURCES}
    ${STM32_SOURCES}
    ${STM32_STARTUP_SOURCE}
)

target_link_libraries(${OUTPUT_NAME}.elf ${STM32_LIBRARIES})

add_custom_target(${OUTPUT_NAME}.hex DEPENDS ${OUTPUT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${OUTPUT_NAME}.elf ${OUTPUT_NAME}.hex)
add_custom_target(${OUTPUT_NAME}.bin DEPENDS ${OUTPUT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${OUTPUT_NAME}.elf ${OUTPUT_NAME}.bin)


if(DEFINED STM32_STLINK_CLI_EXECUTABLE)
    set(STLINK_CMD ${STM32_STLINK_CLI_EXECUTABLE} -c SWD UR -P ${CMAKE_BINARY_DIR}/${OUTPUT_NAME}.hex)
    add_custom_target(program-flash DEPENDS ${OUTPUT_NAME}.hex COMMAND ${STLINK_CMD})
endif()
