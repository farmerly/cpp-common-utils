project(logutils)

add_compile_options(-fPIC)

aux_source_directory(. SRC_FILES)
add_library(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} toolutils)