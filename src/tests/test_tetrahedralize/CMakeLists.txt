aux_source_directories(SOURCES "" .)
add_executable(test_tetrahedralize ${SOURCES})
target_link_libraries(test_tetrahedralize geogram)