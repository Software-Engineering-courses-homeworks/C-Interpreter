cmake_minimum_required(VERSION 3.29)
project(C_Interpeter C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(C_Interpeter
    chunk.c
    chunk.h
    common.h
    debug.c
    debug.h
    main.c
    memory.c
    memory.h
        value.h
        value.c
        vm.h
        vm.c
        compiler.c
        scanner.h
        scanner.c)
