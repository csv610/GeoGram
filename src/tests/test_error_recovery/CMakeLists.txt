aux_source_directories(SOURCES "" .)
add_executable(test_error_recovery ${SOURCES})
target_link_libraries(test_error_recovery geogram)