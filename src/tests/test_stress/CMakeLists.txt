aux_source_directories(SOURCES "" .)
add_executable(test_stress ${SOURCES})
target_link_libraries(test_stress geogram)