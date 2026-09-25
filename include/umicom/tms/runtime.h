/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: include/umicom/tms/runtime.h
 *
 * PURPOSE:
 *   Bind the thin product to Framework-owned application, finance and Open TMS
 *   runtime contracts without duplicating treasury services.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_TMS_RUNTIME_H
#define UMICOM_TMS_RUNTIME_H

#include "umicom/application/runtime/runtime.h"
#include "umicom/finance_ui/funds_review.h"
#include "umicom/finance_ui/finance_ui.h"
#include "umicom/open_tms/service.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialise tms runtime from caller-provided values so later operations receive a known
 * state.
 */
UmiStatus umi_tms_runtime_init(
    UmiApplicationWorkspaceRuntime *out_runtime);
/**
 * Provide the tms runtime health operation used by this module and its client
 * applications.
 */
UmiStatus umi_tms_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health);
/**
 * Provide the tms runtime experience operation used by this module and its client
 * applications.
 */
const UmiApplicationExperienceDefinition *umi_tms_runtime_experience(void);
/* Compose an existing Framework cash position into the shared finance view model. */
UmiStatus umi_tms_runtime_create_cash_position_view(
    const UmiTreasuryCashPosition *position,
    UmiUiViewModel **out_view);
/* Compose an existing Framework cash forecast into the shared finance view model. */
UmiStatus umi_tms_runtime_create_cash_forecast_view(
    const UmiTreasuryCashForecast *forecast,
    UmiUiViewModel **out_view);


/** Review caller-supplied cash flows without creating a treasury trade.
 * Delegates to Framework funds_review.h. Currency and scale must match.
 * Negative projected cash/headroom remains visible as a forecast shortfall. */
UmiStatus UmiTmsRuntimeReviewCash(const UmiMoney *opening,
    const UmiMoney *inflows, const UmiMoney *outflows, const UmiMoney *buffer,
    UmiFinanceCashReview *out);

/**
 * Initialise the Framework-owned Open TMS service used by the product.
 */
void umi_tms_runtime_open_tms_init(UmiOpenTmsService *service);

/**
 * Copy the Framework-owned Open TMS operational snapshot.
 *
 * TMS remains a presentation/composition repository. FX lifecycle, pricing,
 * positions, risk, settlement, accounting and reconciliation remain Framework-owned.
 */
UmiStatus umi_tms_runtime_open_tms_snapshot(
    const UmiOpenTmsService *service,
    uint64_t now_milliseconds,
    UmiOpenTmsServiceSnapshot *out_snapshot);

#ifdef __cplusplus
}
#endif

#endif
