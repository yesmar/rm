# Copyright © 2018, Ramsey Dow. All rights reserved.
# SPDX short identifier: MIT
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(rm LANGUAGES C)
set(CMAKE_C_FLAGS "-g -Wall -Wextra -Werror")
add_executable(driver code/rm.c code/driver.c)
set_property(TARGET driver PROPERTY C_STANDARD 11)
set_property(TARGET driver PROPERTY C_EXTENSIONS OFF)
