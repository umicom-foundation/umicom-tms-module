/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: src/application_surface_controllers.c
 *
 * PURPOSE:
 *   Describe honest treasury panel states and stage commands for approval
 *   without claiming that a trade, payment or settlement was executed.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/tms/application_surface_controllers.h"

#include <stdio.h>
#include <string.h>

/* Provide the tms guidance operation used by this module and its client applications. */
static const char *tms_guidance(const char *component_id)
{
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.treasury.trade-blotter") == 0)
        return "No treasury trades are loaded.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.treasury.trade-capture") == 0)
        return "Create a draft only after selecting an approved legal entity.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.treasury.market-data") == 0)
        return "Connect an approved market-data source to load quotes.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.treasury.pricing") == 0)
        return "Choose a product and market context before pricing.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.treasury.risk") == 0)
        return "Risk measures require positions and approved market data.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.treasury.workflow") == 0)
        return "No treasury workflow tasks are waiting.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.treasury.settlement") == 0)
        return "No settlement instructions are pending.";
    return "Accounting events will appear after approved processing.";
}

/* Provide the tms controller operation used by this module and its client applications. */
static UmiStatus tms_controller(
    void *context,
    const UmiApplicationPresentationPanelPlacement *placement,
    UmiApplicationPresentationSurfaceEvent event,
    const char *payload,
    UmiApplicationPresentationSurfaceUpdate *out_update)
{
    (void)context;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (placement == NULL || placement->panel == NULL || out_update == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    (void)memset(out_update, 0, sizeof(*out_update));
    /* Apply this branch only when its contract condition is satisfied. */
    if (event == UMI_APPLICATION_PRESENTATION_EVENT_UNMOUNT ||
        event == UMI_APPLICATION_PRESENTATION_EVENT_DEACTIVATE) {
        out_update->state = UMI_APPLICATION_PRESENTATION_STATE_DORMANT;
        return UMI_STATUS_OK;
    }
    /* Apply this branch only when its contract condition is satisfied. */
    if (event == UMI_APPLICATION_PRESENTATION_EVENT_COMMAND) {
        out_update->state =
            UMI_APPLICATION_PRESENTATION_STATE_PERMISSION_REQUIRED;
        (void)snprintf(out_update->message, sizeof(out_update->message),
                       "Treasury command '%s' is staged and requires approval.",
                       payload != NULL ? payload : "");
        (void)snprintf(out_update->badge, sizeof(out_update->badge), "SAFE");
        return UMI_STATUS_OK;
    }
    out_update->state = UMI_APPLICATION_PRESENTATION_STATE_EMPTY;
    (void)snprintf(out_update->message, sizeof(out_update->message), "%s",
                   tms_guidance(placement->panel->component_id));
    return UMI_STATUS_OK;
}

/*
 * Add tms application surface controllers only after its inputs and available capacity
 * have been checked.
 */
UmiStatus umi_tms_application_surface_controllers_register(
    UmiApplicationPresentationSurfaceRuntime *runtime, void *context)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (runtime == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_presentation_surface_runtime_register_controller_for_all(
        runtime, tms_controller, context);
}
