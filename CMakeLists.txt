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

# Commands without a provider must not become fictional approval requests.
if(BUILD_TESTING)
    add_executable(tms-command-feedback-test tests/test_command_feedback.c)
    target_link_libraries(tms-command-feedback-test PRIVATE Umicom::tms_module)
    umicom_apply_warnings(tms-command-feedback-test)
    umicom_apply_sanitizers(tms-command-feedback-test)
    foreach(case IN ITEMS command dirty refresh)
        add_test(NAME tms.command_feedback.${case} COMMAND tms-command-feedback-test ${case})
        set_tests_properties(tms.command_feedback.${case} PROPERTIES TIMEOUT 30)
    endforeach()
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

# The product entry point delegates to the same Framework review used by GTK.
if(BUILD_TESTING)
    add_executable(tms-workflow-tools-test tests/test_workflow_tools.c)
    target_link_libraries(tms-workflow-tools-test PRIVATE Umicom::tms_module)
    if(COMMAND umicom_apply_warnings)
        umicom_apply_warnings(tms-workflow-tools-test)
        umicom_apply_sanitizers(tms-workflow-tools-test)
    endif()
    foreach(case IN ITEMS valid currency overflow no-side-effects)
        add_test(NAME tms.workflow_tools.${case} COMMAND tms-workflow-tools-test ${case})
        set_tests_properties(tms.workflow_tools.${case} PROPERTIES TIMEOUT 30 LABELS "tms;workflow;regression")
    endforeach()
endif()

# Install the public workflow lesson beside this product's other guides.
install(FILES docs/WORKFLOW_TOOLS.html
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/tms/docs)

# Exercise the product identity boundary against the shared transaction owner.
if(BUILD_TESTING)
    add_executable(umicom_tms_module-context-review-test tests/test_context_review.c)
    target_link_libraries(umicom_tms_module-context-review-test PRIVATE umicom_tms_module)
    umicom_apply_warnings(umicom_tms_module-context-review-test)
    umicom_apply_sanitizers(umicom_tms_module-context-review-test)
    foreach(context_case publication stale foreign clear divergent_ui)
        add_test(NAME tms.context_review.${context_case}
            COMMAND umicom_tms_module-context-review-test ${context_case})
        set_tests_properties(tms.context_review.${context_case} PROPERTIES
            LABELS "application;context-review;tms")
    endforeach()
    if(COMMAND umicom_register_validation_target)
        umicom_register_validation_target(umicom_tms_module-context-review-test)
    endif()
endif()


# Install the shared workflow explanation beside this product's existing guides.
install(FILES docs/STATEMENT_RECONCILIATION.html
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/tms/docs)

# Use the shared synthetic financial-message workflow; older SDKs retain the
# original console and report an unavailable optional command explicitly.
if(TARGET umicom-tms-console AND TARGET Umicom::finance)
    target_link_libraries(umicom-tms-console PRIVATE Umicom::finance)
    get_target_property(_tms_ufmp_definitions Umicom::finance INTERFACE_COMPILE_DEFINITIONS)
    if(BUILD_TESTING AND "UMICOM_HAS_UFMP_FOUNDATION=1" IN_LIST _tms_ufmp_definitions)
        add_test(NAME tms.ufmp.simulation COMMAND umicom-tms-console --ufmp-simulate)
        set_tests_properties(tms.ufmp.simulation PROPERTIES TIMEOUT 30
            PASS_REGULAR_EXPRESSION "Changed payload refused: yes"
            LABELS "application;tms;ufmp;synthetic;regression")
        if(COMMAND umicom_test_impact_owners)
            umicom_test_impact_owners(tms.ufmp.simulation TARGETS umicom-tms-console)
        endif()
    endif()
    unset(_tms_ufmp_definitions)
endif()

# The preview is calculated by Framework; a product host never nets money itself.
if(TARGET umicom-tms-console AND TARGET Umicom::finance)
    get_target_property(_tms_clearing_definitions Umicom::finance INTERFACE_COMPILE_DEFINITIONS)
    if(BUILD_TESTING AND "UMICOM_HAS_UFMP_CLEARING=1" IN_LIST _tms_clearing_definitions)
        add_test(NAME tms.ufmp.clearing-preview COMMAND umicom-tms-console --clearing-preview)
        set_tests_properties(tms.ufmp.clearing-preview PROPERTIES TIMEOUT 30
            PASS_REGULAR_EXPRESSION "Net receive: -8500" LABELS "application;tms;ufmp;synthetic;regression")
        if(COMMAND umicom_test_impact_owners)
            umicom_test_impact_owners(tms.ufmp.clearing-preview TARGETS umicom-tms-console)
        endif()
    endif()
    unset(_tms_clearing_definitions)
endif()

# Delivery recovery is a shared service; this host supplies only presentation.
if(TARGET umicom-tms-console AND TARGET Umicom::financial_inbox)
    target_link_libraries(umicom-tms-console PRIVATE Umicom::financial_inbox)
    get_target_property(_tms_outbox_definitions Umicom::financial_inbox INTERFACE_COMPILE_DEFINITIONS)
    if(BUILD_TESTING AND "UMICOM_HAS_UFMP_OUTBOX=1" IN_LIST _tms_outbox_definitions)
        add_test(NAME tms.ufmp.outbox-recovery COMMAND umicom-tms-console --outbox-recovery)
        set_tests_properties(tms.ufmp.outbox-recovery PROPERTIES TIMEOUT 30
            PASS_REGULAR_EXPRESSION "Automatic retry after ambiguity: no"
            LABELS "application;tms;ufmp;synthetic;regression")
        if(COMMAND umicom_test_impact_owners)
            umicom_test_impact_owners(tms.ufmp.outbox-recovery TARGETS umicom-tms-console)
        endif()
    endif()
    unset(_tms_outbox_definitions)
endif()

# Qualify the actual TMS composition as well as Framework's standalone surface.
if(BUILD_TESTING AND TARGET umicom_tms_gtk)
    get_target_property(_tms_forward_features Umicom::ui_gtk4 INTERFACE_COMPILE_DEFINITIONS)
    if("UMICOM_HAS_FORWARD_PRACTICE=1" IN_LIST _tms_forward_features)
        add_executable(umi-tms-forward-host-test tests/test_forward_host.c)
        target_link_libraries(umi-tms-forward-host-test PRIVATE Umicom::tms_gtk)
        umicom_apply_warnings(umi-tms-forward-host-test)
        umicom_apply_sanitizers(umi-tms-forward-host-test)
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(umi-tms-forward-host-test)
        endif()
        foreach(scenario capture-review independent retire)
            set(test tms.forward_host.${scenario})
            add_test(NAME ${test} COMMAND umi-tms-forward-host-test ${scenario})
            set_tests_properties(${test} PROPERTIES TIMEOUT 75 SKIP_RETURN_CODE 77
                ENVIRONMENT "G_DEBUG=fatal-warnings" LABELS "tms;treasury;gtk4;workflow;regression")
            if(COMMAND umicom_test_impact_owners)
                umicom_test_impact_owners(${test} TARGETS umi-tms-forward-host-test)
            endif()
        endforeach()
        unset(test)
    endif()
    unset(_tms_forward_features)
endif()

# Exercise the actual product launcher, independent batches and parent retirement.
if(BUILD_TESTING AND TARGET umicom_tms_gtk)
    get_target_property(_tms_accounting_features Umicom::ui_gtk4 INTERFACE_COMPILE_DEFINITIONS)
    if("UMICOM_HAS_ACCOUNTING_PRACTICE=1" IN_LIST _tms_accounting_features)
        add_executable(umi-tms-accounting-host-test tests/test_accounting_host.c)
        target_link_libraries(umi-tms-accounting-host-test PRIVATE Umicom::tms_gtk)
        umicom_apply_warnings(umi-tms-accounting-host-test)
        umicom_apply_sanitizers(umi-tms-accounting-host-test)
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(umi-tms-accounting-host-test)
        endif()
        foreach(scenario capture-review independent retire)
            set(test tms.accounting_host.${scenario})
            add_test(NAME ${test} COMMAND umi-tms-accounting-host-test ${scenario})
            set_tests_properties(${test} PROPERTIES TIMEOUT 75 SKIP_RETURN_CODE 77
                ENVIRONMENT "G_DEBUG=fatal-warnings" LABELS "tms;treasury;accounting;gtk4;workflow;regression")
            if(COMMAND umicom_test_impact_owners)
                umicom_test_impact_owners(${test} TARGETS umi-tms-accounting-host-test)
            endif()
        endforeach()
        unset(test)
    endif()
    unset(_tms_accounting_features)
endif()
