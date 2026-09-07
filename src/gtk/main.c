/*-----------------------------------------------------------------------------
 * Umicom TMS
 * File: src/gtk/main.c
 *
 * PURPOSE:
 *   Launch the branded native treasury workstation while shared UI behavior
 *   remains owned by Umicom Framework.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <gtk/gtk.h>
#include <stdio.h>

#include "umicom/tms/gtk_workstation.h"
#include "umicom/ui/gtk4/workstation/shell_header.h"
#include "umicom/ui/gtk4/workstation/window_fit.h"

/* Tie workstation cleanup to the native window's lifetime. */
static void workstation_destroy_notify(gpointer data)
{
    umi_tms_gtk_workstation_destroy((UmiTmsGtkWorkstation *)data);
}

/* Create a resizable workstation which fits common laptop displays. */
static void on_activate(GtkApplication *application, gpointer user_data)
{
    UmiTmsGtkWorkstation *workstation = NULL;
    GtkWidget *content;
    GtkWindow *window;
    UmiStatus status;
    (void)user_data;
    status = umi_tms_gtk_workstation_create(&workstation);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr, "Unable to create Umicom TMS: %s\n",
                      umi_status_text(status));
        g_application_quit(G_APPLICATION(application));
        return;
    }
    /* Only the real native launcher opts into disk checkpoints. */
    {
        UmiStatus storage_status = umi_tms_gtk_workstation_enable_checkpoint_storage(workstation, 1);
        if (storage_status != UMI_STATUS_OK)
            (void)fprintf(stderr, "Layout storage unavailable: %s\n", umi_status_text(storage_status));
    }

    content = umi_tms_gtk_workstation_widget(workstation);
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (content == NULL) {
        umi_tms_gtk_workstation_destroy(workstation);
        g_application_quit(G_APPLICATION(application));
        return;
    }
    window = GTK_WINDOW(gtk_application_window_new(application));
    gtk_window_set_title(window, "Umicom TMS");
    (void)umi_gtk4_ws_apply_window_identity(window);
    /* Let Framework adapt the preferred workstation size to the display. */
    (void)umi_gtk4_ws_window_fit(window, 1240, 800, 960, 600);
    /* Bind the existing Framework identity before the first realization. */
    status = umi_tms_gtk_workstation_bind_window(workstation, window);
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr, "Umicom TMS titlebar failed: %s\n", umi_status_text(status));
        umi_tms_gtk_workstation_destroy(workstation);
        gtk_window_destroy(window);
        g_application_quit(G_APPLICATION(application));
        return;
    }
    gtk_window_set_child(window, content);
    g_object_set_data_full(G_OBJECT(window), "umicom-tms-workstation",
                           workstation, workstation_destroy_notify);
    gtk_window_present(window);
}

/* Start one portable application loop under the canonical TMS identity. */
int main(int argc, char **argv)
{
    GtkApplication *application;
    int result;
    g_set_prgname("umicom-tms");
    application = gtk_application_new(
        "org.umicom.tms", G_APPLICATION_DEFAULT_FLAGS);
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (application == NULL) return 1;
    g_signal_connect(application, "activate", G_CALLBACK(on_activate), NULL);
    result = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);
    return result;
}

#ifdef _WIN32
#include <windows.h>

/* Windows uses the same startup and argument handling as other platforms. */
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance,
                   LPSTR command_line, int show_command)
{
    (void)instance;
    (void)previous_instance;
    (void)command_line;
    (void)show_command;
    return main(__argc, __argv);
}
#endif
