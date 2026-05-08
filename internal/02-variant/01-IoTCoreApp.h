/**
 * @file 02-IoTCoreApp.h
 * @brief Default IIoTCoreApp: Start runs thread pool workers, Loop runs health/server runnables.
 */

#ifndef IOT_CORE_IOT_CORE_APP_H
#define IOT_CORE_IOT_CORE_APP_H

#include <Arduino.h>
#include <IThreadPool.h>
#include <WiFiHealthCheckerThread.h>


/* @Component */
class IoTCoreApp final : public IIoTCoreApp {


    Public IoTCoreApp() {
    }

    Public ~IoTCoreApp() override = default;

    Public Void Start() override {
    }

    Public Void Stop() override {}

    Public Void Loop() override {
    }

    Public Void Restart() override {
    }

};

#endif // IOT_CORE_IOT_CORE_APP_H
