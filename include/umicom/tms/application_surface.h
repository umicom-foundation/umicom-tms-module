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

typedef struct UmiTmsApplicationSurface UmiTmsApplicationSurface;

UmiStatus umi_tms_application_surface_create(
    UmiTmsApplicationSurface **out_surface);
UmiStatus umi_tms_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiTmsApplicationSurface **out_surface);
void umi_tms_application_surface_destroy(UmiTmsApplicationSurface *surface);
UmiStatus umi_tms_application_surface_activate(
    UmiTmsApplicationSurface *surface, const char *component_id);
UmiStatus umi_tms_application_surface_deactivate(
    UmiTmsApplicationSurface *surface, const char *component_id);
UmiStatus umi_tms_application_surface_focus(
    UmiTmsApplicationSurface *surface, const char *component_id);
UmiStatus umi_tms_application_surface_refresh(
    UmiTmsApplicationSurface *surface);
UmiStatus umi_tms_application_surface_command(
    UmiTmsApplicationSurface *surface,
    const char *component_id,
    const char *command_id);
UmiStatus umi_tms_application_surface_context_changed(
    UmiTmsApplicationSurface *surface,
    const char *component_id,
    const char *context_value);
UmiStatus umi_tms_application_surface_advance(
    UmiTmsApplicationSurface *surface, uint32_t elapsed_seconds);
UmiStatus umi_tms_application_surface_set_background(
    UmiTmsApplicationSurface *surface, int background);
UmiStatus umi_tms_application_surface_snapshot(
    const UmiTmsApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot);
UmiApplicationPresentationSurfaceRuntime *
umi_tms_application_surface_runtime(UmiTmsApplicationSurface *surface);

#ifdef __cplusplus
}
#endif

#endif
