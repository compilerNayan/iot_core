/**
 * @file 01-IIoTCoreApp.h
 * @brief Interface for IoT core app lifecycle: Start, Stop, Loop, Restart.
 */

#ifndef IOT_CORE_IIOT_CORE_APP_H
#define IOT_CORE_IIOT_CORE_APP_H

#include <StandardDefines.h>

DefineStandardPointers(IIoTCoreApp)

class IIoTCoreApp {
    Public Virtual ~IIoTCoreApp() = default;

    /** Start the app (e.g. setup, start threads). */
    Public Virtual Void Start() = 0;

    /** Stop the app (e.g. stop threads, cleanup). */
    Public Virtual Void Stop() = 0;

    /** One iteration of the main loop (e.g. run health checkers, process). */
    Public Virtual Void Loop() = 0;

    /** Restart the app (Stop then Start). */
    Public Virtual Void Restart() = 0;
};

#endif // IOT_CORE_IIOT_CORE_APP_H
