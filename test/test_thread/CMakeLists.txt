cmake_minimum_required(VERSION 3.10)

project(test_threadpool)

aux_source_directory(. DIR_SOURCES)
add_executable(${PROJECT_NAME} ${DIR_SOURCES})