aux_source_directories(SOURCES "" .)
add_executable(test_thread_safety ${SOURCES})
target_link_libraries(test_thread_safety geogram)