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
#include "../01-interface/01-IIoTCoreApp.h"


/* @Component */
class IoTCoreApp final : public IIoTCoreApp {

    /* @Autowired */
    Private IThreadPoolPtr threadPool;

    WiFiHealthCheckerThread wifiHealthCheckerThread;
    InternetHealthCheckerThread internetHealthCheckerThread;
    LocalServerThread localServerThread;
    ResponseHandlerThread responseHandlerThread;
    
    Public IoTCoreApp() = default;

    Public ~IoTCoreApp() override = default;

    Public Void Start() override {
        threadPool->Execute<ResponseHandlerThread>();
        threadPool->Execute<CloudServerThread>();
        threadPool->Execute<LogPublisherThread>();
        threadPool->Execute<DeviceTimeSyncThread>();
    }

    Public Void Stop() override {}

    Public Void Loop() override {
        wifiHealthCheckerThread.Run();
        internetHealthCheckerThread.Run();
        localServerThread.Run();    
    }

    Public Void Restart() override {
        Stop();
        Start();
    }

};

#endif // IOT_CORE_IOT_CORE_APP_H
