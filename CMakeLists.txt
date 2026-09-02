#-----------------------------------------------------------------------------
# Umicom TMS Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build the thin Umicom TMS application composition over Framework-owned
#   linked-workbench, selection-provider and context contracts.
#
# AUTHOR AND ORGANISATION:
#   Sammy Hegab
#   Umicom Foundation
#
# LICENCE:
#   MIT
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
option(UMICOM_TMS_BUILD_GTK4
       "Build the Framework-rendered native Umicom TMS workstation"
       ON)

include(CTest)
include(GNUInstallDirs)

# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()

# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::workbench_selection_provider)
    message(FATAL_ERROR
        "Umicom TMS requires Framework target "
        "Umicom::workbench_selection_provider")
endif()
# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom TMS requires Framework target Umicom::application")
endif()
# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::finance_ui)
    message(FATAL_ERROR
        "Umicom TMS requires Framework target Umicom::finance_ui")
endif()

# Define the local fallback only when the parent build did not provide the shared helper.
if(NOT COMMAND umicom_apply_warnings)
    # Define the apply warnings build helper so parent and application projects apply one
    # consistent rule.
    function(umicom_apply_warnings target)
        # Select the warning options understood by the active compiler.
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        # Use this fallback path when the earlier condition does not apply.
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

# Define the local fallback only when the parent build did not provide the shared helper.
if(NOT COMMAND umicom_apply_sanitizers)
    # Define the apply sanitizers build helper so parent and application projects apply one
    # consistent rule.
    function(umicom_apply_sanitizers target)
    endfunction()
endif()

add_library(umicom_tms_module STATIC
    src/application_surface.c
    src/application_surface_controllers.c
    src/workbench_profile.c
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

# Create this optional product surface only when its build option is enabled.
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

# The TMS repository stays thin: it supplies guarded treasury controllers and
# delegates the complete native workstation to Framework.
if(UMICOM_TMS_BUILD_GTK4 AND TARGET Umicom::ui_gtk4)
    add_library(umicom_tms_gtk STATIC src/gtk/workstation.c)
    add_library(Umicom::tms_gtk ALIAS umicom_tms_gtk)
    target_include_directories(umicom_tms_gtk PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    target_link_libraries(umicom_tms_gtk PUBLIC
        Umicom::tms_module
        Umicom::ui_gtk4)
    umicom_apply_warnings(umicom_tms_gtk)
    umicom_apply_sanitizers(umicom_tms_gtk)

    add_executable(umicom-tms src/gtk/main.c)
    target_link_libraries(umicom-tms PRIVATE Umicom::tms_gtk)
    umicom_apply_warnings(umicom-tms)
    umicom_apply_sanitizers(umicom-tms)
    # Define the local fallback only when the parent build did not provide the shared helper.
    if(NOT COMMAND umicom_apply_application_branding)
        include(
            "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationBranding.cmake")
    endif()
    umicom_apply_application_branding(
        TARGET umicom-tms
        PRODUCT_NAME "Umicom TMS"
        INTERNAL_NAME "umicom-tms"
        APPLICATION_ID "org.umicom.tms"
        VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
        VERSION_MINOR ${PROJECT_VERSION_MINOR}
        VERSION_PATCH ${PROJECT_VERSION_PATCH}
        RESOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/resources"
        DESKTOP_ENTRY
        WINDOWS_GUI)
# Apply this branch only when its contract condition is satisfied.
elseif(UMICOM_TMS_BUILD_GTK4)
    message(STATUS
        "Umicom TMS native workstation disabled because Umicom::ui_gtk4 is unavailable")
endif()

# Register verification targets only when the developer has enabled testing.
if(BUILD_TESTING)
    add_executable(tms-application-surface-test
        tests/test_application_surface.c)
    target_link_libraries(tms-application-surface-test PRIVATE
        Umicom::tms_module)
    umicom_apply_warnings(tms-application-surface-test)
    umicom_apply_sanitizers(tms-application-surface-test)
    add_test(NAME tms.module.application_surface
             COMMAND tms-application-surface-test)

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
# Configure the optional target only when its feature has created it.
if(TARGET umicom-tms-console)
    install(TARGETS umicom-tms-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
# Configure the optional target only when its feature has created it.
if(TARGET umicom-tms)
    install(TARGETS umicom-tms
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
