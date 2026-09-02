/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: tests/test_application_surface.c
 *
 * PURPOSE:
 *   Verify TMS starts standard, focus and learning layouts through the shared
 *   Framework product surface without connecting to live treasury services.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/tms/application_surface.h"

#include <assert.h>

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    const UmiApplicationComponentRecipeAudience audiences[] = {
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_LEARNING,
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_STANDARD,
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_FOCUS};
    size_t index;
    /* Visit each bounded item once so every record receives the same rule. */
    for (index = 0U; index < sizeof(audiences) / sizeof(audiences[0]); ++index) {
        UmiTmsApplicationSurface *surface = NULL;
        UmiApplicationPresentationSurfaceSnapshot snapshot;
        assert(umi_tms_application_surface_create_for_audience(
                   audiences[index], &surface) == UMI_STATUS_OK);
        assert(umi_tms_application_surface_snapshot(surface, &snapshot) ==
               UMI_STATUS_OK);
        assert(snapshot.panel_count > 0U);
        assert(snapshot.visible_count > 0U);
        umi_tms_application_surface_destroy(surface);
    }
    return 0;
}
