/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: tests/test_runtime.c
 *
 * PURPOSE:
 *   Verify the thin product starts Framework-owned application and Open TMS runtimes.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <assert.h>

#include "umicom/tms/runtime.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    UmiApplicationWorkspaceRuntime runtime;
    UmiOpenTmsService open_tms;
    UmiOpenTmsServiceSnapshot snapshot;

    assert(umi_tms_runtime_init(&runtime) == UMI_STATUS_OK);
    assert(runtime.session.experience != NULL);
    assert(runtime.session.active_panel_count > 0U);
    assert(runtime.commands.command_count > 0U);

    umi_tms_runtime_open_tms_init(&open_tms);
    assert(umi_open_tms_service_count() == 80U);
    assert(umi_tms_runtime_open_tms_snapshot(
               &open_tms, 1000U, &snapshot) == UMI_STATUS_OK);
    assert(snapshot.policyCount == 80U);
    assert(!snapshot.dashboard.operationsReady);
    return 0;
}
