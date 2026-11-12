/*-----------------------------------------------------------------------------
 * Umicom TMS
 * File: tests/test_accounting_host.c
 *
 * PURPOSE:
 *   Verify TMS launches shared accounting capture, review and host retirement.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/tms/gtk_workstation.h"
#include "umicom/ui/gtk4/accounting_practice.h"
#include "umicom/ui/gtk4/automation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)
static GtkWindow *Child(GtkWindow *parent, GtkWindow *exclude) {
    GListModel *list = gtk_window_get_toplevels();
    for (guint i = 0U; i < g_list_model_get_n_items(list); ++i) {
        GtkWindow *window = g_list_model_get_item(list, i);
        if (window != exclude && gtk_window_get_transient_for(window) == parent &&
            strcmp(gtk_widget_get_name(GTK_WIDGET(window)), "treasury.accounting.window") == 0) return window;
        g_object_unref(window);
    }
    return NULL;
}
static GtkWidget *Find(GtkWindow *window, const char *name) {
    GtkWidget *value = umi_gtk4_automation_find_named_widget(GTK_WIDGET(window), name);
    CHECK(value != NULL); return value;
}
int main(int argc, char **argv) {
    CHECK(argc == 2);
    if (!gtk_init_check()) return 77;
    /* Record the loaded toolkit, not only compiler headers: native backend
     * lifetime defects can depend on the DLL selected by the process. These
     * diagnostics do not relax assertions or turn a crash into a passing skip. */
    fprintf(stderr, "TMS accounting host %s: GTK runtime %u.%u.%u; headers %u.%u.%u; backend %s\n",
        argv[1], gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version(),
        GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION,
        G_OBJECT_TYPE_NAME(gdk_display_get_default()));
    fflush(stderr);
    UmiTmsGtkWorkstation *workstation = NULL;
    CHECK(umi_tms_gtk_workstation_create(&workstation) == UMI_STATUS_OK);
    GtkWindow *parent = GTK_WINDOW(gtk_window_new()); g_object_ref(parent);
    gtk_window_set_child(parent, umi_tms_gtk_workstation_widget(workstation));
    GtkWidget *launch = Find(parent, "treasury.accounting.launch"); g_object_ref(launch);
    /* A retained product root can outlive its owner while the parent window
     * remains open. Semantic signals must honor both local and inherited state. */
    if (!strcmp(argv[1], "disabled-self") || !strcmp(argv[1], "disabled-ancestor") ||
        !strcmp(argv[1], "owner-retired")) {
        if (!strcmp(argv[1], "disabled-self")) gtk_widget_set_sensitive(launch, FALSE);
        else if (!strcmp(argv[1], "disabled-ancestor"))
            gtk_widget_set_sensitive(umi_tms_gtk_workstation_widget(workstation), FALSE);
        else {
            umi_tms_gtk_workstation_destroy(workstation);
            workstation = NULL;
        }
        guint count = g_list_model_get_n_items(gtk_window_get_toplevels());
        g_signal_emit_by_name(launch, "clicked");
        g_signal_emit_by_name(launch, "clicked");
        CHECK(g_list_model_get_n_items(gtk_window_get_toplevels()) == count);
        gtk_window_destroy(parent);
        umi_tms_gtk_workstation_destroy(workstation);
        g_object_unref(launch); g_object_unref(parent);
        return 0;
    }
    g_signal_emit_by_name(launch, "clicked");
    GtkWindow *first = Child(parent, NULL); CHECK(first != NULL);
    UmiOpenTmsAccountingBook *book = malloc(sizeof *book); CHECK(book != NULL);
    g_signal_emit_by_name(Find(first, "treasury.accounting.capture"), "clicked");
    gtk_editable_set_text(GTK_EDITABLE(Find(first, "treasury.accounting.account")), "practice-receivable");
    gtk_editable_set_text(GTK_EDITABLE(Find(first, "treasury.accounting.debit")), "0");
    gtk_editable_set_text(GTK_EDITABLE(Find(first, "treasury.accounting.credit")), "100");
    g_signal_emit_by_name(Find(first, "treasury.accounting.capture"), "clicked");
    CHECK(UmiGtk4AccountingPracticeRead(first, book) == UMI_STATUS_OK && book->count == 2U);
    if (!strcmp(argv[1], "capture-review")) {
        g_signal_emit_by_name(Find(first, "treasury.accounting.prepare"), "clicked");
        gtk_check_button_set_active(GTK_CHECK_BUTTON(Find(first, "treasury.accounting.ack")), TRUE);
        g_signal_emit_by_name(Find(first, "treasury.accounting.apply"), "clicked");
        CHECK(UmiGtk4AccountingPracticeRead(first, book) == UMI_STATUS_OK);
        CHECK(book->approved && !book->posted);
    } else if (!strcmp(argv[1], "independent")) {
        g_signal_emit_by_name(launch, "clicked");
        GtkWindow *second = Child(parent, first); CHECK(second != NULL);
        fprintf(stderr, "Independent child created; inspecting its separate state.\n"); fflush(stderr);
        CHECK(UmiGtk4AccountingPracticeRead(second, book) == UMI_STATUS_OK && book->count == 0U);
        fprintf(stderr, "Disposing independent child and its input widgets.\n"); fflush(stderr);
        gtk_window_destroy(second); g_object_unref(second);
        fprintf(stderr, "Independent child disposed; checking surviving host.\n"); fflush(stderr);
        CHECK(UmiGtk4AccountingPracticeRead(first, book) == UMI_STATUS_OK && book->count == 2U);
    } else {
        CHECK(!strcmp(argv[1], "retire"));
        gtk_window_destroy(parent);
        CHECK(UmiGtk4AccountingPracticeRead(first, book) == UMI_STATUS_INVALID_STATE);
        guint count = g_list_model_get_n_items(gtk_window_get_toplevels());
        g_signal_emit_by_name(launch, "clicked");
        CHECK(g_list_model_get_n_items(gtk_window_get_toplevels()) == count);
    }
    fprintf(stderr, "Disposing surviving host and parent.\n"); fflush(stderr);
    gtk_window_destroy(first); gtk_window_destroy(parent);
    umi_tms_gtk_workstation_destroy(workstation);
    g_object_unref(launch); g_object_unref(first); g_object_unref(parent); free(book);
    return 0;
}
