cmake_minimum_required(VERSION 3.16)

## @brief Set the name of the project.
## @details This name will be used as the target name for the final binary.
set(PROJECT_NAME angelsoftworks-freertos-ecs)

## @brief Include the ESP-IDF build system.
## @details Loads ESP-IDF’s project configuration and build logic.
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

## @brief Define the project using the given name.
## @note Must be called after including project.cmake.
project(${PROJECT_NAME})