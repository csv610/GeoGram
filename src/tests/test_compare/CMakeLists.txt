aux_source_directories(SOURCES "" .)
add_executable(test_compare ${SOURCES})
target_link_libraries(test_compare geogram)