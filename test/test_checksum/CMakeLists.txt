cmake_minimum_required(VERSION 3.10)

project(test_checksum)

aux_source_directory(. DIR_SOURCES)
add_executable(${PROJECT_NAME} ${DIR_SOURCES})

target_link_libraries(${PROJECT_NAME} toolutils)
target_link_libraries(${PROJECT_NAME} checksum)