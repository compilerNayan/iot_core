# Startup threads: core and stack size

**Design principle:** Keep the **application core (1)** free for critical local control (e.g. switch on/off via CommandProcessorThread). Run heavy or blocking work (HTTP dispatch, Firebase, cloud) on the **system core (0)** so that even when Firebase or cloud calls block, local control stays responsive.

| Thread | Core | Stack | Reason |
|--------|------|-------|--------|
| **ResponseHandlerThread** | Application (1) | 8 KB | HTTP dispatch, controllers, JSON; heavy/blocking — keep off application core. |
| **CloudServerThread** | System (0) | 8 KB | Firebase/SSL; can block — run on system so application core stays free. |
| **LogPublisherThread** | System (0) | 8 KB | Firebase; same as above. |
| **DeviceTimeSyncThread** | System (0) | 4 KB | NTP/sync; network I/O, small stack. |
| **WiFiHealthCheckerThread** | System (0) | 4 KB | WiFi reconnect; keep near stack. |
| **InternetHealthCheckerThread** | System (0) | 4 KB | Light network check. |
| **LocalServerThread** | System (0) | 4 KB | Local TCP; I/O-bound. |
| **CommandProcessorThread** | Application (1) | 4 KB | Critical local control (e.g. switch on/off); must stay responsive even when cloud blocks. |

**Summary**

- **System core (0):** ResponseHandler, CloudServer, LogPublisher (8 KB), plus WiFiHealthChecker, InternetHealthChecker, DeviceTimeSync, LocalServer (4 KB). All heavy or network-facing work runs here.
- **Application core (1):** CommandProcessorThread (4 KB). Reserved for time-critical local control so switches work even when Firebase/HTTP is blocking.

*Note: The thread pool currently uses a single stack size for all workers. Per-thread stack size would require pool or platform support.*
