if(NOT GEOGRAM_LIB_ONLY)
    set(APP_SOURCES
        app_boolean.cpp
        app_csg.cpp
        app_compare.cpp
        app_delaunay.cpp
        app_fillholes.cpp
        app_framefield.cpp
        app_harmonics.cpp
        app_hull.cpp
        app_parameterize.cpp
        app_partition.cpp
        app_ray.cpp
        app_reconstruct.cpp
        app_remesh.cpp
        app_repair.cpp
        app_rvd.cpp
        app_sample.cpp
        app_segment.cpp
        app_smooth.cpp
        app_tetra.cpp
    )

    foreach(src ${APP_SOURCES})
        get_filename_component(APP_NAME ${src} NAME_WE)
        set(TARGET_NAME "geogram_${APP_NAME}")
        add_executable(${TARGET_NAME} ${src})
        target_link_libraries(${TARGET_NAME} geogram)
        install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "GEOGRAM/Apps")
    endforeach()

    add_executable(geogram_app main.cpp)
    target_link_libraries(geogram_app geogram)
    install(TARGETS geogram_app RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    set_target_properties(geogram_app PROPERTIES FOLDER "GEOGRAM/Apps")
endif()