aux_source_directories(SOURCES "" .)
add_executable(test_framefield ${SOURCES})
target_link_libraries(test_framefield geogram)