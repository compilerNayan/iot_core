/**
 * @file 02-IoTCoreApp.h
 * @brief Default IIoTCoreApp: Start runs thread pool workers, Loop runs health/server runnables.
 */

#ifndef IOT_CORE_IOT_CORE_APP_H
#define IOT_CORE_IOT_CORE_APP_H

#include <Arduino.h>
#include <IThreadPool.h>
#include <WiFiHealthCheckerThread.h>
#include <InternetHealthCheckerThread.h>
#include <local_server/LocalServerThread.h>
#include <response_handler/ResponseHandlerThread.h>
#include <cloud_server/CloudServerThread.h>
#include <cloud_server/LogPublisherThread.h>
#include <device_identity/DeviceTimeSyncThread.h>
#include <device_identity/IDeviceDiagnostics.h>
#include "../01-interface/01-IIoTCoreApp.h"


/* @Component */
class IoTCoreApp final : public IIoTCoreApp {

    /* @Autowired */
    Private IDeviceDiagnosticsPtr deviceDiagnostics;

    /* @Autowired */
    Private IThreadPoolPtr threadPool;

    /* @Autowired */
    Private ILoggerPtr logger;

    WiFiHealthCheckerThread wifiHealthCheckerThread;
    InternetHealthCheckerThread internetHealthCheckerThread;
    LocalServerThread localServerThread;

    Private StdVector<IRunnablePtr> startupThreads;
    Private StdVector<ThreadPoolCore> startupThreadCores;

    Public IoTCoreApp() {
        AddStartupThread<ResponseHandlerThread>(ThreadPoolCore::Application);
        AddStartupThread<CloudServerThread>(ThreadPoolCore::System);
        AddStartupThread<LogPublisherThread>(ThreadPoolCore::System);
        AddStartupThread<DeviceTimeSyncThread>(ThreadPoolCore::System);
        AddStartupThread<WiFiHealthCheckerThread>(ThreadPoolCore::System);
        AddStartupThread<InternetHealthCheckerThread>(ThreadPoolCore::System);
        AddStartupThread<LocalServerThread>(ThreadPoolCore::System);
    }

    Public ~IoTCoreApp() override = default;

    Public Void Start() override {
        logger->Info(Tag::Untagged, StdString("[ArduinoSpringBootApp] Starting app..."));
        if (deviceDiagnostics->HadPreviousCrash()) {
            logger->Info(Tag::Untagged, StdString("[ArduinoSpringBootApp] Previous run: crashed (core dump/panic)."));
        } else {
            logger->Info(Tag::Untagged, StdString("[ArduinoSpringBootApp] Previous run: normal."));
        }
        for (Size i = 0; i < startupThreads.size(); ++i) {
            if (startupThreads[i]) {
                threadPool->Execute(startupThreads[i], startupThreadCores[i]);
            }
        }
    }

    Protected Void AddStartupThreadImpl(IRunnablePtr thread, ThreadPoolCore core) override {
        if (thread) {
            startupThreads.push_back(thread);
            startupThreadCores.push_back(core);
        }
    }

    Public Void Stop() override {}

    Public Void Loop() override {
    }

    Public Void Restart() override {
        Stop();
        Start();
    }

};

#endif // IOT_CORE_IOT_CORE_APP_H
