/*-----------------------------------------------------------------------------
 * Umicom TMS
 * File: src/gtk/workstation.c
 *
 * PURPOSE:
 *   Supply treasury identity and approval-aware controllers to the shared
 *   Framework native product workstation.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/tms/gtk_workstation.h"

#include <stdlib.h>

#include "umicom/tms/application_surface.h"
#include "umicom/tms/application_surface_controllers.h"

struct UmiTmsGtkWorkstation {
    UmiApplicationProductGtk4Workstation *framework_workstation;
};

/* TMS contributes product identity and guarded controllers while Framework
 * supplies the shell, rendering, docking, layout and appearance behavior. */
UmiStatus umi_tms_gtk_workstation_create(
    UmiTmsGtkWorkstation **out_workstation)
{
    UmiTmsGtkWorkstation *workstation;
    UmiApplicationProductGtk4WorkstationConfig config;
    UmiStatus status;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (out_workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    *out_workstation = NULL;
    workstation = (UmiTmsGtkWorkstation *)calloc(1U, sizeof(*workstation));
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_OUT_OF_MEMORY;
    config = umi_application_product_gtk4_workstation_config_default(
        UMI_TMS_APPLICATION_ID,
        "Umicom TMS",
        umi_tms_application_surface_controllers_register,
        NULL);
    config.mode_badge = "Controlled";
    status = umi_application_product_gtk4_workstation_create(
        &config, &workstation->framework_workstation);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        umi_tms_gtk_workstation_destroy(workstation);
        return status;
    }
    *out_workstation = workstation;
    return UMI_STATUS_OK;
}

/* Destroy owned services in reverse creation order and accept NULL safely. */
void umi_tms_gtk_workstation_destroy(UmiTmsGtkWorkstation *workstation)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return;
    umi_application_product_gtk4_workstation_destroy(
        workstation->framework_workstation);
    workstation->framework_workstation = NULL;
    free(workstation);
}

/* Borrow the shared root; callers must not destroy it independently. */
GtkWidget *umi_tms_gtk_workstation_widget(UmiTmsGtkWorkstation *workstation)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_widget(
              workstation->framework_workstation)
        : NULL;
}

/* Role-specific layout selection stays inside the canonical Framework model. */
UmiStatus umi_tms_gtk_workstation_select_layout(
    UmiTmsGtkWorkstation *workstation,
    const char *layout_id)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_select_layout(
              workstation->framework_workstation, layout_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/* Return copied evidence so diagnostics cannot mutate a live treasury shell. */
UmiStatus umi_tms_gtk_workstation_snapshot(
    const UmiTmsGtkWorkstation *workstation,
    UmiApplicationProductGtk4WorkstationSnapshot *out_snapshot)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_snapshot(
              workstation->framework_workstation, out_snapshot)
        : UMI_STATUS_INVALID_ARGUMENT;
}
