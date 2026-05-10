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
    Private StdVector<ThreadPoolStackSize> startupThreadStackSize;

    Public IoTCoreApp() {
        AddStartupThread<WiFiHealthCheckerThread>(ThreadPoolCore::System, ThreadPoolStackSize::KB_4);
        AddStartupThread<LocalServerThread>(ThreadPoolCore::System, ThreadPoolStackSize::KB_4);
        AddStartupThread<InternetHealthCheckerThread>(ThreadPoolCore::Application, ThreadPoolStackSize::KB_4);
        AddStartupThread<ResponseHandlerThread>(ThreadPoolCore::System, ThreadPoolStackSize::KB_12);
        //AddStartupThread<DeviceTimeSyncThread>(ThreadPoolCore::Application, ThreadPoolStackSize::KB_8);
        AddStartupThread<CloudServerThread>(ThreadPoolCore::Application, ThreadPoolStackSize::KB_12);
        AddStartupThread<LogPublisherThread>(ThreadPoolCore::System, ThreadPoolStackSize::KB_16);
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
                threadPool->Execute(startupThreads[i], startupThreadCores[i], startupThreadStackSize[i]);
            }
        }
    }

    Protected Void AddStartupThreadImpl(IRunnablePtr thread, ThreadPoolCore core, ThreadPoolStackSize stackSize) override {
        if (thread) {
            startupThreads.push_back(thread);
            startupThreadCores.push_back(core);
            startupThreadStackSize.push_back(stackSize);
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
