cmake_minimum_required(VERSION 3.14)
project(iot_core VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(IOT_CORE_DEPS
    springbootplusplus
    03_device_connectivity
    04_local_server
    05_cloud_server
    06_response_handler
    07_device_identity
)

foreach(DEP IN LISTS IOT_CORE_DEPS)
    FetchContent_Declare(${DEP}
        GIT_REPOSITORY https://github.com/compilerNayan/${DEP}.git
        GIT_TAG        main
    )
endforeach()
FetchContent_MakeAvailable(${IOT_CORE_DEPS})

add_library(iot_core INTERFACE)

set_target_properties(iot_core PROPERTIES
    VERSION ${PROJECT_VERSION}
)

target_include_directories(iot_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

foreach(DEP IN LISTS IOT_CORE_DEPS)
    if(TARGET ${DEP}::${DEP})
        target_link_libraries(iot_core INTERFACE ${DEP}::${DEP})
    elseif(TARGET ${DEP})
        target_link_libraries(iot_core INTERFACE ${DEP})
    else()
        FetchContent_GetProperties(${DEP})
        if(${DEP}_SOURCE_DIR)
            target_include_directories(iot_core INTERFACE
                $<BUILD_INTERFACE:${${DEP}_SOURCE_DIR}/include>
                $<BUILD_INTERFACE:${${DEP}_SOURCE_DIR}>
            )
        endif()
    endif()
endforeach()

include(GNUInstallDirs)

install(TARGETS iot_core
    EXPORT iot_coreTargets
)

install(EXPORT iot_coreTargets
    FILE iot_coreTargets.cmake
    NAMESPACE iot_core::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/iot_core
)
