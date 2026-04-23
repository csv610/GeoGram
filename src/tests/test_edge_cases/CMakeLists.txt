aux_source_directories(SOURCES "" .)
add_executable(test_edge_cases ${SOURCES})
target_link_libraries(test_edge_cases geogram)