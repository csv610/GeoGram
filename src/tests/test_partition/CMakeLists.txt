aux_source_directories(SOURCES "" .)
add_executable(test_partition ${SOURCES})
target_link_libraries(test_partition geogram)