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
#include "umicom/ui/gtk4/finance_review.h"
#include "umicom/ui/gtk4/cash_plan.h"

#include "umicom/tms/application_surface.h"
#include "umicom/tms/application_surface_controllers.h"

struct UmiTmsGtkWorkstation {
    UmiApplicationProductGtk4Workstation *framework_workstation;
    GtkWidget *root;
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
    /* Compose the reusable manual review above the existing workstation.
     * Calculations remain in Framework; this product owns only the assembly. */
    GtkWidget *review = NULL;
    status = UmiGtk4FinanceReviewCreate(UMI_GTK4_FINANCE_REVIEW_CASH, &review);
    if (status != UMI_STATUS_OK) {
        umi_tms_gtk_workstation_destroy(workstation);
        return status;
    }
    workstation->root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    g_object_ref_sink(workstation->root);
    /* Dated planning shares the Framework owner and never changes account balances. */
    gtk_box_append(GTK_BOX(workstation->root), UmiGtk4CashPlanLauncherCreate());
    gtk_box_append(GTK_BOX(workstation->root), review);
    GtkWidget *inner = umi_application_product_gtk4_workstation_widget(workstation->framework_workstation);
    gtk_widget_set_vexpand(inner, TRUE);
    gtk_box_append(GTK_BOX(workstation->root), inner);
    *out_workstation = workstation;
    return UMI_STATUS_OK;
}

/* Forward explicit native titlebar adoption to the existing Framework owner. */
UmiStatus umi_tms_gtk_workstation_bind_window(
    UmiTmsGtkWorkstation *workstation, GtkWindow *window)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_bind_window(workstation->framework_workstation, window)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/* Delegate explicit persistence to the existing shared layout owner. */
UmiStatus umi_tms_gtk_workstation_enable_checkpoint_storage(
    UmiTmsGtkWorkstation *workstation, int restore_saved)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_enable_checkpoint_storage(
            workstation->framework_workstation, restore_saved)
        : UMI_STATUS_INVALID_ARGUMENT;
}


/* Destroy owned services in reverse creation order and accept NULL safely. */
/* Release this composition after its layout-owned storage and widgets. */
void umi_tms_gtk_workstation_destroy(UmiTmsGtkWorkstation *workstation)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return;
    /* A native window may retain the composition root. Remove the service-
     * backed body and disable the wrapper before releasing its owner reference. */
    if (workstation->root != NULL) {
        gtk_widget_set_sensitive(workstation->root, FALSE);
        GtkWidget *inner = umi_application_product_gtk4_workstation_widget(workstation->framework_workstation);
        if (inner != NULL && gtk_widget_get_parent(inner) == workstation->root)
            gtk_box_remove(GTK_BOX(workstation->root), inner);
    }
    umi_application_product_gtk4_workstation_destroy(
        workstation->framework_workstation);
    workstation->framework_workstation = NULL;
    g_clear_object(&workstation->root);
    free(workstation);
}

/* Migration: this composition now owns the wrapper; its children are
 * the manual-review component and the unchanged Framework product body. */
/* Borrow the shared root; callers must not destroy it independently. */
GtkWidget *umi_tms_gtk_workstation_widget(UmiTmsGtkWorkstation *workstation)
{
    /* The old body-only return is preserved here. The wrapper now contains
     * both the Framework review component and the unchanged product body. */
    // return workstation != NULL
    // ? umi_application_product_gtk4_workstation_widget(
    // workstation->framework_workstation)
    // : NULL;
    return workstation != NULL ? workstation->root : NULL;
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
