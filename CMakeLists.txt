# Copyright © 2018, Ramsey Dow. All rights reserved.
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(rm LANGUAGES C)
set(CMAKE_C_FLAGS "-g -Wall -Wextra -Werror")
add_executable(rm code/rm.c code/driver.c)
set_property(TARGET rm PROPERTY C_STANDARD 11)
set_property(TARGET rm PROPERTY C_EXTENSIONS OFF)
