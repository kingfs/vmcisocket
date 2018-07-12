cmake_minimum_required(VERSION 3.5)

project(vmci VERSION 2.0)

include_directories(vmci PRIVATE src)

add_library(vmci SHARED src/vmcilib.c)
