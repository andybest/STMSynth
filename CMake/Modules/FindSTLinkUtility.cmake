# Find the STM32 ST-Link Utility

find_program(STM32_STLINK_CLI_EXECUTABLE
    "st-util"
    PATHS
    "/usr/local/bin"
    DOC "STM32 ST-Link Utility Command Line Interface"
    )

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(STLinkUtility DEFAULT_MSG STM32_STLINK_CLI_EXECUTABLE)