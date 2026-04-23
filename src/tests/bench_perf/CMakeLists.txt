aux_source_directories(SOURCES "" .)
add_executable(bench_perf ${SOURCES})
target_link_libraries(bench_perf geogram)