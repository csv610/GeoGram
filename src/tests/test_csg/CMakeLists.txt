aux_source_directories(SOURCES "" .)
add_executable(test_csg ${SOURCES})
target_link_libraries(test_csg geogram)