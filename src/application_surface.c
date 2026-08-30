/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: src/application_surface.c
 *
 * PURPOSE:
 *   Compose TMS over the Framework product surface while keeping a small API
 *   for treasury frontends, consoles and automated checks.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/tms/application_surface.h"

#include <stdlib.h>

#include "umicom/tms/application_surface_controllers.h"

struct UmiTmsApplicationSurface {
    UmiApplicationPresentationProductSurface product;
};

UmiStatus umi_tms_application_surface_create(
    UmiTmsApplicationSurface **out_surface)
{
    return umi_tms_application_surface_create_for_audience(
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_STANDARD, out_surface);
}

UmiStatus umi_tms_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiTmsApplicationSurface **out_surface)
{
    UmiTmsApplicationSurface *surface;
    UmiStatus status;
    if (out_surface == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    *out_surface = NULL;
    surface = calloc(1U, sizeof(*surface));
    if (surface == NULL) return UMI_STATUS_OUT_OF_MEMORY;
    status = umi_application_presentation_product_surface_init_for_audience(
        UMI_TMS_APPLICATION_ID, audience,
        umi_tms_application_surface_controllers_register, surface,
        &surface->product);
    if (status != UMI_STATUS_OK) {
        umi_tms_application_surface_destroy(surface);
        return status;
    }
    *out_surface = surface;
    return UMI_STATUS_OK;
}

void umi_tms_application_surface_destroy(UmiTmsApplicationSurface *surface)
{
    if (surface == NULL) return;
    umi_application_presentation_product_surface_dispose(&surface->product);
    free(surface);
}

UmiStatus umi_tms_application_surface_activate(
    UmiTmsApplicationSurface *surface, const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_activate(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_tms_application_surface_deactivate(
    UmiTmsApplicationSurface *surface, const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_deactivate(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_tms_application_surface_focus(
    UmiTmsApplicationSurface *surface, const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_focus(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_tms_application_surface_refresh(
    UmiTmsApplicationSurface *surface)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_refresh(
              &surface->product)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_tms_application_surface_command(
    UmiTmsApplicationSurface *surface,
    const char *component_id,
    const char *command_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_command(
              &surface->product, component_id, command_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_tms_application_surface_context_changed(
    UmiTmsApplicationSurface *surface,
    const char *component_id,
    const char *context_value)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_context_changed(
              &surface->product, component_id, context_value)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_tms_application_surface_advance(
    UmiTmsApplicationSurface *surface, uint32_t elapsed_seconds)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_advance(
              &surface->product, elapsed_seconds)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_tms_application_surface_set_background(
    UmiTmsApplicationSurface *surface, int background)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_set_background(
              &surface->product, background)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiStatus umi_tms_application_surface_snapshot(
    const UmiTmsApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_snapshot(
              &surface->product, out_snapshot)
        : UMI_STATUS_INVALID_ARGUMENT;
}

UmiApplicationPresentationSurfaceRuntime *
umi_tms_application_surface_runtime(UmiTmsApplicationSurface *surface)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_runtime(
              &surface->product)
        : NULL;
}
