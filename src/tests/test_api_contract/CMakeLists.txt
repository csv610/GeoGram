aux_source_directories(SOURCES "" .)
add_executable(test_api_contract ${SOURCES})
target_link_libraries(test_api_contract geogram)