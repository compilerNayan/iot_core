/**
 * @file 01-IIoTCoreApp.h
 * @brief Interface for IoT core app lifecycle: Start, Stop, Loop, Restart.
 */

#ifndef IOT_CORE_IIOT_CORE_APP_H
#define IOT_CORE_IIOT_CORE_APP_H

#include <StandardDefines.h>
#include <IRunnable.h>
#include <memory>
#include <type_traits>

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

    /** Register a thread (by type T) to be started when the app starts (Start()). Creates T via default constructor. */
    template<typename T>
    Public Void AddStartupThread() {
        static_assert(std::is_base_of_v<IRunnable, T>, "T must derive from IRunnable");
        AddStartupThreadImpl(std::make_shared<T>());
    }

    /** Implement in variant: register the given thread for startup. */
    Protected Virtual Void AddStartupThreadImpl(IRunnablePtr thread) = 0;
};

#endif // IOT_CORE_IIOT_CORE_APP_H
