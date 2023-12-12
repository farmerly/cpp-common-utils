project(xmlutils)

add_compile_options(-fPIC)

aux_source_directory(. SRC_FILES)
add_library(${PROJECT_NAME} ${SRC_FILES})
