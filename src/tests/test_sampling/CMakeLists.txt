aux_source_directories(SOURCES "" .)
add_executable(test_sampling ${SOURCES})
target_link_libraries(test_sampling geogram)