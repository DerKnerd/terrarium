# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(SRCS "ePaper/epd.cpp" "ePaper/epdif.cpp" "ePaper/epdpaint.cpp" "ePaper/font12.cpp" "output/Display.cpp" "input/Temperature.cpp" "esp32-ds18b20/ds18b20.c" "esp32-owb/owb.c" "esp32-owb/owb_gpio.c" "esp32-owb/owb_rmt.c")
idf_component_register(SRCS "main.cpp" ${app_sources})
