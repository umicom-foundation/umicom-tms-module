/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: tests/test_framework_views.c
 *
 * PURPOSE:
 *   Verify TMS renders existing Framework treasury records through shared finance UI views.
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
    UmiTreasuryCashPosition position;
    UmiUiViewModel *view = NULL;
    assert(umi_treasury_cash_position_init(&position, "gbp", "GBP", 10000, 12000) == UMI_STATUS_OK);
    assert(umi_tms_runtime_create_cash_position_view(&position, &view) == UMI_STATUS_OK);
    assert(view != NULL);
    umi_ui_view_model_destroy(view);
    return 0;
}
