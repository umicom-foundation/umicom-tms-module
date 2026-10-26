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
    UmiTmsGtkWorkstation *workstation = NULL;
    CHECK(umi_tms_gtk_workstation_create(&workstation) == UMI_STATUS_OK);
    GtkWindow *parent = GTK_WINDOW(gtk_window_new()); g_object_ref(parent);
    gtk_window_set_child(parent, umi_tms_gtk_workstation_widget(workstation));
    GtkWidget *launch = Find(parent, "treasury.accounting.launch"); g_object_ref(launch);
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
        CHECK(UmiGtk4AccountingPracticeRead(second, book) == UMI_STATUS_OK && book->count == 0U);
        gtk_window_destroy(second); g_object_unref(second);
        CHECK(UmiGtk4AccountingPracticeRead(first, book) == UMI_STATUS_OK && book->count == 2U);
    } else {
        CHECK(!strcmp(argv[1], "retire"));
        gtk_window_destroy(parent);
        CHECK(UmiGtk4AccountingPracticeRead(first, book) == UMI_STATUS_INVALID_STATE);
        guint count = g_list_model_get_n_items(gtk_window_get_toplevels());
        g_signal_emit_by_name(launch, "clicked");
        CHECK(g_list_model_get_n_items(gtk_window_get_toplevels()) == count);
    }
    gtk_window_destroy(first); gtk_window_destroy(parent);
    umi_tms_gtk_workstation_destroy(workstation);
    g_object_unref(launch); g_object_unref(first); g_object_unref(parent); free(book);
    return 0;
}
