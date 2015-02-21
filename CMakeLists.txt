project(STMSynth C CXX ASM)
cmake_minimum_required(VERSION 2.8)

set(OUTPUT_NAME STMSynth)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/CMake/arm-none-eabi-gcc.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include(CMake/STM32.cmake)

# SYNTHIA
set(SYNTHIA_INCLUDES ${CMAKE_SOURCE_DIR}/src/synthia)
file(GLOB SYNTHIA_SOURCES ${CMAKE_SOURCE_DIR}/src/synthia/*.cc)

# USB CORE
set(USB_CORE_INCLUDES ${CMAKE_SOURCE_DIR}/Middleware/STM32_USB_Device_Library/Core/Inc)
file(GLOB USB_CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/Middleware/STM32_USB_Device_Library/Core/Src/*.c
)

# FREERTOS
set(FREERTOS_BASE_DIR ${CMAKE_SOURCE_DIR}/Middleware/FreeRTOS/Source)

set(FREERTOS_BASE_INCLUDES ${FREERTOS_BASE_DIR}/include)
file(GLOB FREERTOS_BASE_SOURCES ${FREERTOS_BASE_DIR}/*.c)

set(FREERTOS_PORT_INCLUDES ${FREERTOS_BASE_DIR}/portable/GCC/ARM_CM4F)
file(GLOB FREERTOS_PORT_SOURCES ${FREERTOS_BASE_DIR}/portable/GCC/ARM_CM4F/*.c)

set(FREERTOS_HEAP_SOURCES ${FREERTOS_BASE_DIR}/portable/MemMang/heap_2.c)

set(FREERTOS_CMSIS_INCLUDES ${FREERTOS_BASE_DIR}/CMSIS_RTOS)
set(FREERTOS_CMSIS_SOURCES ${FREERTOS_BASE_DIR}/CMSIS_RTOS/cmsis_os.c)

set(FREERTOS_INCLUDES
    ${FREERTOS_BASE_INCLUDES}
    ${FREERTOS_PORT_INCLUDES}
    ${FREERTOS_CMSIS_INCLUDES}
)

set(FREERTOS_SOURCES
    ${FREERTOS_BASE_SOURCES}
    ${FREERTOS_PORT_SOURCES}
    ${FREERTOS_HEAP_SOURCES}
    ${FREERTOS_CMSIS_SOURCES}
)


include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/usb
    ${CMAKE_SOURCE_DIR}/src/config
    ${SYNTHIA_INCLUDES}
    ${USB_CORE_INCLUDES}
    ${STM32_INCLUDE_DIRS}
    ${FREERTOS_INCLUDES}
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_SOURCE_DIR}/CMake/STM32F407VG_FLASH.ld")

add_executable(${OUTPUT_NAME}.elf
    src/main.cc
    src/Audio.cc
    src/syscalls.c
    src/system_stm32f4xx.c
    src/stm32f4xx_it.c
    src/stm32f4xx_hal_msp.c
    src/usb/usbd_conf.c
    src/usb/usbd_desc.c
    src/usb/usbd_midi.c
    src/usb/usbd_hid.c
    
    ${USB_CORE_SOURCES}
    ${SYNTHIA_SOURCES}
    ${STM32_SOURCES}
    ${FREERTOS_SOURCES}
    ${STM32_STARTUP_SOURCE}
)

target_link_libraries(${OUTPUT_NAME}.elf ${STM32_LIBRARIES})

add_custom_target(${OUTPUT_NAME}.hex DEPENDS ${OUTPUT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${OUTPUT_NAME}.elf ${OUTPUT_NAME}.hex)
add_custom_target(${OUTPUT_NAME}.bin DEPENDS ${OUTPUT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${OUTPUT_NAME}.elf ${OUTPUT_NAME}.bin)


if(DEFINED STM32_STLINK_CLI_EXECUTABLE)
    set(STLINK_CMD ${STM32_STLINK_CLI_EXECUTABLE} -c SWD UR -P ${CMAKE_BINARY_DIR}/${OUTPUT_NAME}.hex)
    add_custom_target(program-flash DEPENDS ${OUTPUT_NAME}.hex COMMAND ${STLINK_CMD})
endif()
