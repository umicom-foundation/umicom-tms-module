/*-----------------------------------------------------------------------------
 * Umicom TMS
 * File: include/umicom/tms/gtk_workstation.h
 *
 * PURPOSE:
 *   Expose the thin native treasury composition over the Framework-owned
 *   product workstation and customisable suite layouts.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TMS_GTK_WORKSTATION_H
#define UMICOM_TMS_GTK_WORKSTATION_H

#include <gtk/gtk.h>

#include "umicom/application/suite_layout/gtk4_product_workstation.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represent the tms gtk workstation data shared with callers of this public contract.
 */
typedef struct UmiTmsGtkWorkstation UmiTmsGtkWorkstation;

/** Create TMS as a thin client of the Framework native product host. */
UmiStatus umi_tms_gtk_workstation_create(
    UmiTmsGtkWorkstation **out_workstation);
/** Release the TMS boundary and all Framework services which it owns. */
void umi_tms_gtk_workstation_destroy(UmiTmsGtkWorkstation *workstation);
/** Borrow the root widget for a native application window. */
GtkWidget *umi_tms_gtk_workstation_widget(UmiTmsGtkWorkstation *workstation);
/** Select a front-office, middle-office or back-office layout. */
UmiStatus umi_tms_gtk_workstation_select_layout(
    UmiTmsGtkWorkstation *workstation,
    const char *layout_id);
/** Copy current layout, treasury surface and panel-coverage evidence. */
UmiStatus umi_tms_gtk_workstation_snapshot(
    const UmiTmsGtkWorkstation *workstation,
    UmiApplicationProductGtk4WorkstationSnapshot *out_snapshot);

#ifdef __cplusplus
}
#endif

#endif
