/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: src/runtime.c
 *
 * PURPOSE:
 *   Initialise the product through canonical Framework application runtime contracts.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/tms/runtime.h"

#include "umicom/application/experience_catalogue.h"

const UmiApplicationExperienceDefinition *umi_tms_runtime_experience(void)
{
    return umi_application_experience_catalogue_find("org.umicom.tms");
}

UmiStatus umi_tms_runtime_init(UmiApplicationWorkspaceRuntime *out_runtime)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_tms_runtime_experience();
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_workspace_runtime_init(experience, out_runtime);
}

UmiStatus umi_tms_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_tms_runtime_experience();
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_runtime_health_evaluate(
        experience, probe, user_data, out_health);
}

/* Compose TMS cash-position presentation from the shared Framework finance projection. */
UmiStatus umi_tms_runtime_create_cash_position_view(
    const UmiTreasuryCashPosition *position,
    UmiUiViewModel **out_view)
{
    /* Reuse the Framework treasury projection; cash calculations remain in the
     * existing treasury core rather than the TMS application. */
    return umi_finance_ui_cash_position_view_create(
        "cash-position", position, out_view);
}

/* Compose TMS forecast presentation from the same shared Framework projection layer. */
UmiStatus umi_tms_runtime_create_cash_forecast_view(
    const UmiTreasuryCashForecast *forecast,
    UmiUiViewModel **out_view)
{
    /* Reuse the same projection for GTK, web and any future frontend. */
    return umi_finance_ui_cash_forecast_view_create(
        "cash-forecast", forecast, out_view);
}
