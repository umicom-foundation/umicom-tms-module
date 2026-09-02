/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: tests/test_workbench_profile.c
 *
 * PURPOSE:
 *   Verify TMS consumes the Framework-owned linked-workbench profile without
 *   duplicating panel or colour-group definitions in the application module.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <assert.h>
#include <string.h>

#include "umicom/tms/workbench_profile.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    UmiTmsWorkbenchProfile *profile = NULL;
    UmiTmsWorkbenchProfileSnapshot snapshot;
    const UmiWorkbenchContextHostProfile *host_profile;
    const UmiWorkbenchContextSourceTradingProfile *sources;

    assert(umi_tms_workbench_profile_create(&profile) == UMI_STATUS_OK);
    assert(profile != NULL);
    assert(umi_tms_workbench_profile_snapshot(
               profile, &snapshot) == UMI_STATUS_OK);

    assert(strcmp(snapshot.application_id, "org.umicom.tms") == 0);
    assert(strcmp(snapshot.profile_id, "tms.linked-workbench") == 0);
    assert(strcmp(snapshot.trading_group_id, "trading.red") == 0);
    assert(strcmp(snapshot.operations_group_id, "operations.green") == 0);
    assert(snapshot.group_count == 2U);
    assert(snapshot.endpoint_count == 4U);

    host_profile = umi_tms_workbench_profile_context_host(profile);
    sources = umi_tms_workbench_profile_context_sources(profile);
    assert(host_profile != NULL);
    assert(sources != NULL);
    assert(umi_workbench_context_host_profile_validate(
               host_profile) == UMI_STATUS_OK);
    assert(strcmp(sources->tms_application_id, "org.umicom.tms") == 0);

    umi_tms_workbench_profile_destroy(profile);
    return 0;
}
