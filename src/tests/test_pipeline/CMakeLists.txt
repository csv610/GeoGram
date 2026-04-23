aux_source_directories(SOURCES "" .)
add_executable(test_pipeline ${SOURCES})
target_link_libraries(test_pipeline geogram)