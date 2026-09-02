/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: src/workbench_profile.c
 *
 * PURPOSE:
 *   Compose the TMS workbench entirely from Framework-owned groups, endpoints
 *   and context-source definitions.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/tms/workbench_profile.h"

#include <stdlib.h>
#include <string.h>

struct UmiTmsWorkbenchProfile {
    UmiWorkbenchSelectionProviderTradingWorkbench shared;
    uint64_t revision;
};

/* Provide the copy text operation used by this module and its client applications. */
static UmiStatus copy_text(char *destination,
                           size_t capacity,
                           const char *source)
{
    return umi_workbench_selection_provider_copy_text(
        destination,
        capacity,
        source != NULL ? source : "");
}

/*
 * Initialise tms workbench profile from caller-provided values so later operations receive
 * a known state.
 */
UmiStatus umi_tms_workbench_profile_create(
    UmiTmsWorkbenchProfile **out_profile)
{
    UmiTmsWorkbenchProfile *profile;
    UmiStatus status;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (out_profile == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }

    *out_profile = NULL;
    profile = (UmiTmsWorkbenchProfile *)calloc(1U, sizeof(*profile));
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (profile == NULL) {
        return UMI_STATUS_OUT_OF_MEMORY;
    }

    /*
     * TMS does not redefine trading/operations channels.  It selects the TMS
     * profile from the same Framework object consumed by Trader.
     */
    status = umi_workbench_selection_provider_trading_workbench_build(
        &profile->shared);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status == UMI_STATUS_OK) {
        status = umi_workbench_selection_provider_trading_workbench_validate(
            &profile->shared);
    }
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        free(profile);
        return status;
    }

    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(profile->shared.tms.application_id, "org.umicom.tms") != 0) {
        free(profile);
        return UMI_STATUS_INVALID_STATE;
    }

    profile->revision = 1U;
    *out_profile = profile;
    return UMI_STATUS_OK;
}

/*
 * Release or reset state held by tms workbench profile so the same storage can be reused
 * safely.
 */
void umi_tms_workbench_profile_destroy(
    UmiTmsWorkbenchProfile *profile)
{
    free(profile);
}

/*
 * Provide the tms workbench profile snapshot operation used by this module and its client
 * applications.
 */
UmiStatus umi_tms_workbench_profile_snapshot(
    const UmiTmsWorkbenchProfile *profile,
    UmiTmsWorkbenchProfileSnapshot *out_snapshot)
{
    UmiStatus status;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (profile == NULL || out_snapshot == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }

    memset(out_snapshot, 0, sizeof(*out_snapshot));
    out_snapshot->struct_size = (uint32_t)sizeof(*out_snapshot);
    out_snapshot->api_version = UMI_TMS_WORKBENCH_PROFILE_API_VERSION;

    status = copy_text(
        out_snapshot->application_id,
        sizeof(out_snapshot->application_id),
        profile->shared.tms.application_id);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->profile_id,
        sizeof(out_snapshot->profile_id),
        profile->shared.tms.profile_id);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->title,
        sizeof(out_snapshot->title),
        profile->shared.tms.title);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->trading_group_id,
        sizeof(out_snapshot->trading_group_id),
        profile->shared.trading_group_id);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    status = copy_text(
        out_snapshot->operations_group_id,
        sizeof(out_snapshot->operations_group_id),
        profile->shared.operations_group_id);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) return status;

    out_snapshot->group_count = profile->shared.tms.group_count;
    out_snapshot->endpoint_count = profile->shared.tms.endpoint_count;
    out_snapshot->revision =
        profile->revision + profile->shared.tms.revision;

    return UMI_STATUS_OK;
}

/*
 * Provide the tms workbench profile context host operation used by this module and its
 * client applications.
 */
const UmiWorkbenchContextHostProfile *
umi_tms_workbench_profile_context_host(
    const UmiTmsWorkbenchProfile *profile)
{
    return profile != NULL ? &profile->shared.tms : NULL;
}

/*
 * Provide the tms workbench profile context sources operation used by this module and its
 * client applications.
 */
const UmiWorkbenchContextSourceTradingProfile *
umi_tms_workbench_profile_context_sources(
    const UmiTmsWorkbenchProfile *profile)
{
    return profile != NULL ? &profile->shared.sources : NULL;
}
