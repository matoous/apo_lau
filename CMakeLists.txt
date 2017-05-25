cmake_minimum_required(VERSION 3.6)
project(apo_lau)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        conf_files/unit1.txt
        conf_files/unit2.txt
        conf_files/unit3.txt
        conf_files/unit4.txt
        conf_files/unit5.txt
        app.c
        console_info.c
        console_info.h
        devices_list.c
        devices_list.h
        display.c
        display.h
        font_rom8x16.c
        font_types.h
        light_admin_unit.h
        Makefile
        mzapo_parlcd.c
        mzapo_parlcd.h
        mzapo_phys.c
        mzapo_phys.h
        mzapo_regs.h
        passer.h
        pixel.c
        pixel.h
        socket_rocket.c
        socket_rocket.h)

add_executable(apo_lau ${SOURCE_FILES})