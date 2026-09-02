/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: include/umicom/tms/workbench_profile.h
 *
 * PURPOSE:
 *   Expose TMS's thin application composition over the Framework-owned
 *   linked-workbench trading/operations profile.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_TMS_WORKBENCH_PROFILE_H
#define UMICOM_TMS_WORKBENCH_PROFILE_H

#include <stddef.h>
#include <stdint.h>

#include "umicom/workbench_selection_provider/trading_workbench.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UMI_TMS_WORKBENCH_PROFILE_API_VERSION 1U

/**
 * Represent the tms workbench profile data shared with callers of this public contract.
 */
typedef struct UmiTmsWorkbenchProfile UmiTmsWorkbenchProfile;

/**
 * Represent the tms workbench profile snapshot data shared with callers of this public
 * contract.
 */
typedef struct UmiTmsWorkbenchProfileSnapshot {
    uint32_t struct_size;
    uint32_t api_version;
    char application_id[UMI_WORKBENCH_CONTEXT_HOST_ID_CAPACITY];
    char profile_id[UMI_WORKBENCH_CONTEXT_HOST_ID_CAPACITY];
    char title[UMI_WORKBENCH_CONTEXT_HOST_TITLE_CAPACITY];
    char trading_group_id[UMI_WORKBENCH_CONTEXT_HOST_ID_CAPACITY];
    char operations_group_id[UMI_WORKBENCH_CONTEXT_HOST_ID_CAPACITY];
    size_t group_count;
    size_t endpoint_count;
    uint64_t revision;
} UmiTmsWorkbenchProfileSnapshot;

/**
 * Initialise tms workbench profile from caller-provided values so later operations receive
 * a known state.
 */
UmiStatus umi_tms_workbench_profile_create(
    UmiTmsWorkbenchProfile **out_profile);
/**
 * Release or reset state held by tms workbench profile so the same storage can be reused
 * safely.
 */
void umi_tms_workbench_profile_destroy(
    UmiTmsWorkbenchProfile *profile);
/**
 * Provide the tms workbench profile snapshot operation used by this module and its client
 * applications.
 */
UmiStatus umi_tms_workbench_profile_snapshot(
    const UmiTmsWorkbenchProfile *profile,
    UmiTmsWorkbenchProfileSnapshot *out_snapshot);

/**
 * Provide the tms workbench profile context host operation used by this module and its
 * client applications.
 */
const UmiWorkbenchContextHostProfile *
umi_tms_workbench_profile_context_host(
    const UmiTmsWorkbenchProfile *profile);
/**
 * Provide the tms workbench profile context sources operation used by this module and its
 * client applications.
 */
const UmiWorkbenchContextSourceTradingProfile *
umi_tms_workbench_profile_context_sources(
    const UmiTmsWorkbenchProfile *profile);

#ifdef __cplusplus
}
#endif
#endif
