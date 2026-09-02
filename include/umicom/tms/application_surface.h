/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: include/umicom/tms/application_surface.h
 *
 * PURPOSE:
 *   Expose TMS standard, focus and learning workspaces through the shared
 *   Framework product surface and treasury-aware safe controllers.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TMS_APPLICATION_SURFACE_H
#define UMICOM_TMS_APPLICATION_SURFACE_H

#include "umicom/application/presentation/presentation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UMI_TMS_APPLICATION_ID "org.umicom.tms"
#define UMI_TMS_STANDARD_RECIPE_ID "org.umicom.workspace.tms.standard"

/**
 * Represent the tms application surface data shared with callers of this public contract.
 */
typedef struct UmiTmsApplicationSurface UmiTmsApplicationSurface;

/**
 * Initialise tms application surface from caller-provided values so later operations
 * receive a known state.
 */
UmiStatus umi_tms_application_surface_create(
    UmiTmsApplicationSurface **out_surface);
/**
 * Provide the tms application surface create for audience operation used by this module
 * and its client applications.
 */
UmiStatus umi_tms_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiTmsApplicationSurface **out_surface);
/**
 * Release or reset state held by tms application surface so the same storage can be reused
 * safely.
 */
void umi_tms_application_surface_destroy(UmiTmsApplicationSurface *surface);
/**
 * Provide the tms application surface activate operation used by this module and its
 * client applications.
 */
UmiStatus umi_tms_application_surface_activate(
    UmiTmsApplicationSurface *surface, const char *component_id);
/**
 * Provide the tms application surface deactivate operation used by this module and its
 * client applications.
 */
UmiStatus umi_tms_application_surface_deactivate(
    UmiTmsApplicationSurface *surface, const char *component_id);
/**
 * Provide the tms application surface focus operation used by this module and its client
 * applications.
 */
UmiStatus umi_tms_application_surface_focus(
    UmiTmsApplicationSurface *surface, const char *component_id);
/**
 * Provide the tms application surface refresh operation used by this module and its client
 * applications.
 */
UmiStatus umi_tms_application_surface_refresh(
    UmiTmsApplicationSurface *surface);
/**
 * Provide the tms application surface command operation used by this module and its client
 * applications.
 */
UmiStatus umi_tms_application_surface_command(
    UmiTmsApplicationSurface *surface,
    const char *component_id,
    const char *command_id);
/**
 * Provide the tms application surface context changed operation used by this module and
 * its client applications.
 */
UmiStatus umi_tms_application_surface_context_changed(
    UmiTmsApplicationSurface *surface,
    const char *component_id,
    const char *context_value);
/**
 * Provide the tms application surface advance operation used by this module and its client
 * applications.
 */
UmiStatus umi_tms_application_surface_advance(
    UmiTmsApplicationSurface *surface, uint32_t elapsed_seconds);
/**
 * Provide the tms application surface set background operation used by this module and its
 * client applications.
 */
UmiStatus umi_tms_application_surface_set_background(
    UmiTmsApplicationSurface *surface, int background);
/**
 * Provide the tms application surface snapshot operation used by this module and its
 * client applications.
 */
UmiStatus umi_tms_application_surface_snapshot(
    const UmiTmsApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot);
/**
 * Provide the tms application surface runtime operation used by this module and its client
 * applications.
 */
UmiApplicationPresentationSurfaceRuntime *
umi_tms_application_surface_runtime(UmiTmsApplicationSurface *surface);

#ifdef __cplusplus
}
#endif

#endif
