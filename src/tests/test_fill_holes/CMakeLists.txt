aux_source_directories(SOURCES "" .)
add_executable(test_fill_holes ${SOURCES})
target_link_libraries(test_fill_holes geogram)