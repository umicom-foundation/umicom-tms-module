/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: src/console/main.c
 *
 * PURPOSE:
 *   Provide a tiny native verification frontend for the Framework-composed TMS
 *   workbench.  No treasury business processing lives in this executable.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#ifdef UMICOM_HAS_UFMP_FOUNDATION
#include "umicom/financial_messaging/simulation.h"
#endif
#ifdef UMICOM_HAS_UFMP_CLEARING
#include "umicom/financial_messaging/clearing_preview.h"
#endif

#include "umicom/tms/workbench_profile.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
/* Preserve the original entry signature for review. RunDefault keeps the
 * complete existing console workflow; main now dispatches an optional shared
 * Framework simulation before invoking that unchanged workflow. */
#if 0
int main(void)
#endif
static int RunDefault(void)
{
    UmiTmsWorkbenchProfile *profile = NULL;
    UmiTmsWorkbenchProfileSnapshot snapshot;
    UmiStatus status;

    status = umi_tms_workbench_profile_create(&profile);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to create TMS workbench profile: %d\n",
                      (int)status);
        return 1;
    }

    status = umi_tms_workbench_profile_snapshot(profile, &snapshot);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(stderr,
                      "Unable to read TMS workbench profile: %d\n",
                      (int)status);
        umi_tms_workbench_profile_destroy(profile);
        return 1;
    }

    (void)printf("Umicom TMS Module\n");
    (void)printf("Profile: %s\n", snapshot.profile_id);
    (void)printf("Groups: %zu\n", snapshot.group_count);
    (void)printf("Endpoints: %zu\n", snapshot.endpoint_count);
    (void)printf("Trading group: %s\n", snapshot.trading_group_id);
    (void)printf("Operations group: %s\n", snapshot.operations_group_id);

    umi_tms_workbench_profile_destroy(profile);
    return 0;
}

/* Financial rules remain in Framework; this host owns only command routing
 * and writing the synthetic report to its console. */
int main(int argc, char **argv)
{
    if (argc == 1) return RunDefault();
    if (argc == 2 && strcmp(argv[1], "--ufmp-simulate") == 0) {
#ifdef UMICOM_HAS_UFMP_FOUNDATION
        char report[1024];
        size_t required = 0U;
        UmiStatus status = UmiUfmpSimulationFormat(report, sizeof report, &required);
        if (status != UMI_STATUS_OK) {
            fprintf(stderr, "UFMP simulation failed: %d\n", (int)status);
            return 1;
        }
        return fputs(report, stdout) == EOF ? 1 : 0;
#else
        fputs("This Framework SDK does not include UFMP simulation support.\n", stderr);
        return 2;
#endif
    }
    if (argc == 2 && strcmp(argv[1], "--clearing-preview") == 0) {
#ifdef UMICOM_HAS_UFMP_CLEARING
        char report[1024];
        size_t required = 0U;
        UmiStatus status = UmiUfmpClearingExampleFormat(report, sizeof report, &required);
        if (status != UMI_STATUS_OK) {
            fprintf(stderr, "Clearing preview failed: %d\n", (int)status);
            return 1;
        }
        return fputs(report, stdout) == EOF ? 1 : 0;
#else
        fputs("This Framework SDK does not include clearing previews.\n", stderr);
        return 2;
#endif
    }
    /* Retain the earlier help text for review; the expanded line documents both optional commands. */
#if 0
    fputs("Usage: umicom-tms-console [--ufmp-simulate]\n", stderr);
#endif
    fputs("Usage: umicom-tms-console [--ufmp-simulate | --clearing-preview]\n", stderr);
    return 2;
}
