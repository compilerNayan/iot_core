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
#include <IWiFiConnectionManager.h>
#include "../01-interface/01-IIoTCoreApp.h"


/* @Component */
class IoTCoreApp final : public IIoTCoreApp {

    /* @Autowired */
    Private IDeviceDiagnosticsPtr deviceDiagnostics;

    /* @Autowired */
    Private IThreadPoolPtr threadPool;

    /* @Autowired */
    Private ILoggerPtr logger;

    /* @Autowired */
    Private IWiFiConnectionManagerPtr wiFiConnectionManager;

    WiFiHealthCheckerThread wifiHealthCheckerThread;
    InternetHealthCheckerThread internetHealthCheckerThread;
    LocalServerThread localServerThread;

    Private StdVector<IRunnablePtr> startupThreads;
    Private StdVector<ThreadPoolCore> startupThreadCores;
    Private StdVector<Bool> startupThreadHeavyDuty;

    Public IoTCoreApp() {
        AddStartupThread<WiFiHealthCheckerThread>(ThreadPoolCore::System, false);
        //AddStartupThread<LocalServerThread>(ThreadPoolCore::System, false);
        AddStartupThread<InternetHealthCheckerThread>(ThreadPoolCore::Application, false);
        //AddStartupThread<DeviceTimeSyncThread>(ThreadPoolCore::System, false);
        AddStartupThread<ResponseHandlerThread>(ThreadPoolCore::Application, true);
        //AddStartupThread<CloudServerThread>(ThreadPoolCore::System, true);
        //AddStartupThread<LogPublisherThread>(ThreadPoolCore::System, true);
    }

    Public ~IoTCoreApp() override = default;

    Public Void Start() override {
        logger->Info(Tag::Untagged, StdString("[ArduinoSpringBootApp] Starting app..."));
        if (deviceDiagnostics->HadPreviousCrash()) {
            logger->Info(Tag::Untagged, StdString("[ArduinoSpringBootApp] Previous run: crashed (core dump/panic)."));
        } else {
            logger->Info(Tag::Untagged, StdString("[ArduinoSpringBootApp] Previous run: normal."));
        }
        // Initialize WiFi/LwIP on main thread before spawning tasks (avoids tcpip "Invalid mbox" crash)
        if (wiFiConnectionManager) {
            wiFiConnectionManager->ConnectNetwork();
        }
        for (Size i = 0; i < startupThreads.size(); ++i) {
            if (startupThreads[i]) {
                threadPool->Execute(startupThreads[i], startupThreadCores[i], startupThreadHeavyDuty[i]);
            }
        }
    }

    Protected Void AddStartupThreadImpl(IRunnablePtr thread, ThreadPoolCore core, Bool heavyDuty) override {
        if (thread) {
            startupThreads.push_back(thread);
            startupThreadCores.push_back(core);
            startupThreadHeavyDuty.push_back(heavyDuty);
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
