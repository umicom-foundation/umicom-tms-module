/*-----------------------------------------------------------------------------
 * Umicom TMS Module
 * File: include/umicom/tms/application_surface_controllers.h
 *
 * PURPOSE:
 *   Register treasury guidance and safe command staging for every panel in a
 *   selected Framework-owned TMS workspace recipe.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_TMS_APPLICATION_SURFACE_CONTROLLERS_H
#define UMICOM_TMS_APPLICATION_SURFACE_CONTROLLERS_H

#include "umicom/application/presentation/surface_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

UmiStatus umi_tms_application_surface_controllers_register(
    UmiApplicationPresentationSurfaceRuntime *runtime, void *context);

#ifdef __cplusplus
}
#endif

#endif
