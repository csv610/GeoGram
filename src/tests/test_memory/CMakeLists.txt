aux_source_directories(SOURCES "" .)
add_executable(test_memory ${SOURCES})
target_link_libraries(test_memory geogram)