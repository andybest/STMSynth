# Find CMSIS and Hal Libraries.
# Add any extra middleware here.

set(STM32_DRIVERS_ROOT_DIR ${CMAKE_SOURCE_DIR}/Drivers)

find_path(STM32_DRIVERS_ROOT_DIR
    CMSIS/Include/arm_common_tables.h
    STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
    DOC "STM32 Drivers Directory"
)

# Device specific startup code
set(STM32_STARTUP_SOURCE ${STM32_DRIVERS_ROOT_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s)

set(STM32_INCLUDE_DIRS
    ${STM32_DRIVERS_ROOT_DIR}/CMSIS/Include
    ${STM32_DRIVERS_ROOT_DIR}/CMSIS/Device/ST/STM32F4xx/Include
    ${STM32_DRIVERS_ROOT_DIR}/STM32F4xx_HAL_Driver/Inc
)

file(GLOB STM32_SOURCES 
    ${STM32_DRIVERS_ROOT_DIR}/STM32F4xx_HAL_Driver/src/*.c
    ${STM32_DRIVERS_ROOT_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/*.c
)