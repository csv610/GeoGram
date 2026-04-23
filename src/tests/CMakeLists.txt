if(GEOGRAM_WITH_TETGEN)
  add_subdirectory(test_csg)
  add_subdirectory(test_sampling)
  add_subdirectory(test_tetrahedralize)
endif()

add_subdirectory(test_compare)
add_subdirectory(test_partition)
add_subdirectory(test_fill_holes)
add_subdirectory(test_framefield)
add_subdirectory(bench_perf)

# New stress/edge case tests
add_subdirectory(test_memory)
add_subdirectory(test_stress)
add_subdirectory(test_edge_cases)
add_subdirectory(test_thread_safety)

# Error recovery and pipeline tests
add_subdirectory(test_error_recovery)
add_subdirectory(test_api_contract)
add_subdirectory(test_pipeline)