project(sockutils)

find_package(Libevent REQUIRED)

aux_source_directory(. DIR_LIB_SRCS)
add_library (${PROJECT_NAME} ${DIR_LIB_SRCS})

target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} logutils)
target_link_libraries(${PROJECT_NAME} libevent::core)
target_link_libraries(${PROJECT_NAME} libevent::pthreads)