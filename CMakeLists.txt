#-----------------------------------------------------------------------------
# Umicom TMS Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build the thin Umicom TMS application composition over Framework-owned
#   linked-workbench, selection-provider and context contracts.
#
# Created by: Sammy Hegab
# Organisation: Umicom Foundation
# Licence: MIT
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.24)

project(
    UmicomTmsModule
    VERSION 0.2.0
    DESCRIPTION "Thin Umicom TMS workbench composition"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(UMICOM_TMS_BUILD_CONSOLE
       "Build the native Umicom TMS verification console"
       ON)

include(CTest)
include(GNUInstallDirs)

if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()

if(NOT TARGET Umicom::workbench_selection_provider)
    message(FATAL_ERROR
        "Umicom TMS requires Framework target "
        "Umicom::workbench_selection_provider")
endif()
if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom TMS requires Framework target Umicom::application")
endif()
if(NOT TARGET Umicom::finance_ui)
    message(FATAL_ERROR
        "Umicom TMS requires Framework target Umicom::finance_ui")
endif()

if(NOT COMMAND umicom_apply_warnings)
    function(umicom_apply_warnings target)
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        else()
            target_compile_options(${target} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wshadow
            )
        endif()
    endfunction()
endif()

if(NOT COMMAND umicom_apply_sanitizers)
    function(umicom_apply_sanitizers target)
    endfunction()
endif()

add_library(umicom_tms_module STATIC
    src/workbench_profile.c
    src/experience.c
    src/runtime.c
    src/readiness.c
    src/workspace_commands.c
)
add_library(Umicom::tms_module ALIAS umicom_tms_module)

target_include_directories(umicom_tms_module PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(umicom_tms_module PUBLIC
    Umicom::Framework
    Umicom::workbench_selection_provider
    Umicom::application
    Umicom::finance_ui
)
umicom_apply_warnings(umicom_tms_module)
umicom_apply_sanitizers(umicom_tms_module)

if(UMICOM_TMS_BUILD_CONSOLE)
    add_executable(umicom-tms-console
        src/console/main.c
    )
    target_link_libraries(umicom-tms-console PRIVATE
        Umicom::tms_module
    )
    umicom_apply_warnings(umicom-tms-console)
    umicom_apply_sanitizers(umicom-tms-console)
endif()

if(BUILD_TESTING)
    add_executable(tms-workbench-profile-test
        tests/test_workbench_profile.c
    )
    target_link_libraries(tms-workbench-profile-test PRIVATE
        Umicom::tms_module
    )
    umicom_apply_warnings(tms-workbench-profile-test)
    umicom_apply_sanitizers(tms-workbench-profile-test)
    add_test(
        NAME tms.module.workbench_profile
        COMMAND tms-workbench-profile-test
    )

    add_executable(tms-experience-test
        tests/test_experience.c
    )
    target_link_libraries(tms-experience-test PRIVATE
        Umicom::tms_module
    )
    umicom_apply_warnings(tms-experience-test)
    umicom_apply_sanitizers(tms-experience-test)
    add_test(
        NAME tms.module.experience
        COMMAND tms-experience-test
    )

    add_executable(tms-runtime-test
        tests/test_runtime.c
    )
    target_link_libraries(tms-runtime-test PRIVATE
        umicom_tms_module
    )
    umicom_apply_warnings(tms-runtime-test)
    umicom_apply_sanitizers(tms-runtime-test)
    add_test(
        NAME tms.module.runtime
        COMMAND tms-runtime-test
    )

    add_executable(tms-readiness-runtime-test
        tests/test_readiness_runtime.c
    )
    target_link_libraries(tms-readiness-runtime-test PRIVATE
        umicom_tms_module
    )
    umicom_apply_warnings(tms-readiness-runtime-test)
    umicom_apply_sanitizers(tms-readiness-runtime-test)
    add_test(
        NAME tms.module.readiness_runtime
        COMMAND tms-readiness-runtime-test
    )

    add_executable(tms-framework-views-test
        tests/test_framework_views.c
    )
    target_link_libraries(tms-framework-views-test PRIVATE
        Umicom::tms_module
    )
    umicom_apply_warnings(tms-framework-views-test)
    umicom_apply_sanitizers(tms-framework-views-test)
    add_test(
        NAME tms.module.framework_views
        COMMAND tms-framework-views-test
    )

endif()

install(TARGETS umicom_tms_module
    EXPORT UmicomTmsModuleTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(TARGET umicom-tms-console)
    install(TARGETS umicom-tms-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES application.umicom.yaml README.md LICENSE
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/tms)

message(STATUS "Umicom TMS Module version: ${PROJECT_VERSION}")
message(STATUS "Umicom TMS Framework workbench and experience profiles enabled")

include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationProductisationContribution.cmake")
umicom_add_application_productisation_contribution(
    NAME "tms"
    TARGET "umicom_tms_productisation"
    SOURCE "src/productisation_contribution.c"
    TEST_SOURCE "tests/test_productisation_contribution.c")
