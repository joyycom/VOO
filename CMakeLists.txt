cmake_minimum_required(VERSION 3.14)

project(VOO LANGUAGES CXX)

option(IS_RELEASEBUILD "release package"  true)

set(EDITOR_VERSION_MAJOR 1 CACHE STRING "major version")
set(EDITOR_VERSION_MINOR 4 CACHE STRING "minor version")
set(EDITOR_VERSION_PATCH 1 CACHE STRING "patch version")


set(BUILD_NUM 1 CACHE STRING "jenkins build num")
set(EDITOR_VERSION ${EDITOR_VERSION_MAJOR}.${EDITOR_VERSION_MINOR}.${EDITOR_VERSION_PATCH} CACHE STRING "jenkins build num")
set(RemoteBuild false CACHE STRING "remote jenkins build")

set(DEPENDENCIES_PATH "${PROJECT_SOURCE_DIR}/thirds/dependencies")
message("DEPENDENCIES_PATH=${DEPENDENCIES_PATH}")

if(APPLE)
    set(CMAKE_DIR "${PROJECT_SOURCE_DIR}/cmake/mac")
elseif(WIN32)
    set(CMAKE_DIR "${PROJECT_SOURCE_DIR}/cmake/win")
endif()

if(IS_RELEASEBUILD)
    set(IS_RELEASE true CACHE BOOL "release app")
else()
    set(IS_RELEASE false CACHE BOOL "debug app")
endif()

message("Build Num EDITOR_VERSION: ${EDITOR_VERSION}  BUILD_NUM: ${BUILD_NUM} IS_RELEASE: ${IS_RELEASE}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_DIR}")

if(WIN32)
include(generate_product_version)
generate_product_version(
    VersionFilesOutputVariable
    NAME "VOO"
    ICON "${CMAKE_DIR}/skyeditor.ico"
    VERSION_MAJOR ${EDITOR_VERSION_MAJOR}
    VERSION_MINOR ${EDITOR_VERSION_MINOR}
    VERSION_PATCH ${EDITOR_VERSION_PATCH}
    VERSION_REVISION ${BUILD_NUM}
    COMPANY_NAME "Guangzhou Joyy Information Technology Co., Ltd."
    FILE_DESCRIPTION "VOO"
    )
endif()


configure_file(
    "cmake/BuildConfig_source.h"
    "${PROJECT_BINARY_DIR}/BuildConfig.h"
    )

message("SourceDir:${PROJECT_SOURCE_DIR} BINARY_DIR ${PROJECT_BINARY_DIR} ${CMAKE_BINARY_DIR}")
include("${PROJECT_BINARY_DIR}/BuildConfig.h")

if(APPLE)
    include_directories(${PROJECT_SOURCE_DIR}/src ${DEPENDENCIES_PATH}/include)
elseif(WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/src ${DEPENDENCIES_PATH}/include ${Qt5AxContainer_INCLUDE_DIRS})
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} /W4 -DSKYEDITOR_WIN32")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG /MD")
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/skyeditor.rc")
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif()

if(APPLE)
    MESSAGE(STATUS "current platform: macOS")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -g -Wall -DDSKYEDITOR_MAC") # -g 在可执行程序中包含标准调试信息 -Wall 打印出gcc提供的警告信息  -fdeclspec 开启__declspec
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG" )
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.joyy.app.skyeditor")

    message("CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

    #    set(MACOSX_BUNDLE_ICON_FILE skyeditor.icns)

    # And this part tells CMake where to find and install the file itself
    set(APP_ICON_MACOSX ${CMAKE_DIR}/skyeditor.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

    set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--deep")
endif()


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH $ENV{QT_DIR})
set(CMAKE_INSTALL_PREFIX ${ROOT_DIR})

MESSAGE(STATUS "QT_DIR = $ENV{QT_DIR}")

find_package(QT NAMES Qt5 COMPONENTS Core Quick Multimedia Widgets QuickWidgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Multimedia Widgets QuickWidgets LinguistTools SerialPort WebSockets Xml REQUIRED)

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(DEPLOYQT_EXECUTABLE NAMES windeployqt macdeployqt HINTS "${_qt_bin_dir}")

set(QUAZIP_LIB_LIBRARIES Qt5::Core)
set(QUAZIP_LIB_VERSION 1.0.0)
set(QUAZIP_LIB_SOVERSION 1.0.0)
set(QUAZIP_QT_MAJOR_VERSION 5 CACHE STRING "Qt version to use (4 or 5), defaults to 5")
set(QUAZIP_LIB_FILE_NAME quazip${QuaZip_VERSION_MAJOR}-qt${QUAZIP_QT_MAJOR_VERSION})
set(QUAZIP_LIB_TARGET_NAME QuaZip)

set(QUAZIP_TEST_QT_LIBRARIES Qt5::Core Qt5::Network)
set(QUAZIP_PKGCONFIG_REQUIRES Qt5Core)
set(QUAZIP_ENABLE_TESTS OFF)


set(QUAZIP_QT_ZLIB_USED ON)
find_package(ZLIB OPTIONAL_COMPONENTS)
if (ZLIB_FOUND)
    set(QUAZIP_LIB_LIBRARIES ${QUAZIP_LIB_LIBRARIES} ZLIB::ZLIB)
endif()

if(WIN32)
    find_package(Qt5AxContainer REQUIRED)
endif()

add_subdirectory("thirds/quazip")
add_subdirectory("thirds/mmkv")

get_filename_component(ROOT_DIR ${CMAKE_SOURCE_DIR} ABSOLUTE)

set(MY_SRC_DIR ${ROOT_DIR}/src)
set(TS_FILE_DIR ${ROOT_DIR}/src/i18n)

file(GLOB_RECURSE MY_CPP_LIST LIST_DIRECTORIES false
    ${MY_SRC_DIR}/*.cpp
    ${MY_SRC_DIR}/*.cc
    ${MY_SRC_DIR}/*.c
    ${MY_SRC_DIR}/*.mm
    )

file(GLOB_RECURSE MY_RESOURCES_LIST LIST_DIRECTORIES false
    ${MY_SRC_DIR}/*.qrc
    )

file(GLOB_RECURSE TS_FILES LIST_DIRECTORIES false
    ${TS_FILE_DIR}/*.ts
    )

file(GLOB_RECURSE QM_FILES LIST_DIRECTORIES false
    ${TS_FILE_DIR}/translations/*.qm
    )

set(PROJECT_SOURCES
    ${MY_CPP_LIST}
    ${MY_RESOURCES_LIST}
    ${TS_FILES}
    ${QM_FILES}
    )

if(ANDROID)
    add_library(VOO SHARED
        ${PROJECT_SOURCES}
        )
elseif(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9.0")
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_SOURCES} ${APP_ICON_MACOSX})
elseif(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LARGEADDRESSAWARE")

    add_executable(VOO
        ${PROJECT_SOURCES}
        ${VersionFilesOutputVariable}
        )
endif()

if(APPLE)
    configure_file(${CMAKE_DIR}/Info.plist.in ${CMAKE_BINARY_DIR}/Info.plist)
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/Info.plist)
    find_library(SKYMEDIA NAMES SkyMediaOSX PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    if (NOT SKYMEDIA)
        message(FATAL_ERROR "SkyMediaOSX.framework is not found")
    endif()
    find_library(OF_CORE NAMES of_core_macOS PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(OF_EFFECT NAMES of_effect_macOS PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)

    find_library(VENUS NAMES venus2_macOS PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_FACE NAMES vn_face_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_AUDIO_DENOISE NAMES vn_audiodenoise_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VN_CLASSIFYING NAMES vn_classifying_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_CORE NAMES vn_core_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_DOCRECT NAMES vn_docrect_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_FACE_REENACT NAMES vn_face_reenactment_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_FACE_PARSER NAMES vn_faceparser_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VN_GENERAL NAMES vn_general_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_GESTURE NAMES vn_gesture_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_HIIDO NAMES vn_hiido_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_IMAGENET NAMES vn_imagenet12_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_KIT NAMES vn_kit_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_POSE NAMES vn_pose_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_OBJCOUNT NAMES vn_objcount_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_RPC NAMES vn_rpc_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_SEGMENT NAMES vn_segment_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)
    find_library(VENUS_VAD NAMES vn_vad_osx PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)

    find_library(METAL NAMES Metal REQUIRED)
    find_library(METAL_KIT NAMES MetalKit REQUIRED)
    find_library(METAL_PERFORMANCE_SHADERS NAMES MetalPerformanceShaders REQUIRED)
    find_library(QUARTZCORE NAMES QuartzCore REQUIRED)
    find_library(CORE_MEDIA NAMES CoreMedia REQUIRED)
    find_library(AVFOUNDATION NAMES AVFoundation REQUIRED)
    find_library(AVKit NAMES AVKit REQUIRED)
    find_library(Accelerate NAMES Accelerate REQUIRED)
    find_library(COCOA NAMES Cocoa REQUIRED)


    find_library(SENTRY NAMES sentry PATHS "${DEPENDENCIES_PATH}/platform_libs/osx/" REQUIRED)

    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "@executable_path/../Frameworks")
    #    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "@executable_path/../Frameworks")
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${DEPLOYQT_EXECUTABLE} "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>" -qmldir=${CMAKE_SOURCE_DIR}
        )
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "${DEPENDENCIES_PATH}/platform_libs/osx/"
        $<TARGET_FILE:${PROJECT_NAME}>)

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DEPENDENCIES_PATH}/utils/osx"
        $<TARGET_BUNDLE_DIR:${PROJECT_NAME}>)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/LICENSE.TXT" "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/MacOS/LICENSE.TXT"
    )

    message("install ffmpeg done!")

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/BundleResources"
        "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/MacOS")

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DEPENDENCIES_PATH}/venus_models"
        "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/MacOS/venus_models")

elseif(WIN32)
    find_library(SKYMEDIA NAMES skymedia PATHS "${DEPENDENCIES_PATH}/platform_libs/win/lib" REQUIRED)
    find_library(OF_CORE NAMES of_core PATHS "${DEPENDENCIES_PATH}/platform_libs/win/lib" REQUIRED)
    find_library(OF_EFFECT NAMES of_effect PATHS "${DEPENDENCIES_PATH}/platform_libs/win/lib" REQUIRED)

    find_library(GLEW NAMES glew32 PATHS "${DEPENDENCIES_PATH}/platform_libs/win/lib" REQUIRED)
    find_library(SENTRY NAMES sentry PATHS "${DEPENDENCIES_PATH}/platform_libs/win/lib/" REQUIRED)
    find_library(VENUS NAMES venus PATHS "${DEPENDENCIES_PATH}/platform_libs/win/lib/" REQUIRED)

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DEPENDENCIES_PATH}/platform_libs/win/bin"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DEPENDENCIES_PATH}/platform_libs/win/bin"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DEPENDENCIES_PATH}/utils/win"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/BundleResources"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/LICENSE.TXT" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/LICENSE.TXT"
    )

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DEPENDENCIES_PATH}/venus_models"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/venus_models")
endif()

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/src/i18n/translations"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/translations")

target_compile_definitions(VOO
    PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

message(STATUS "SKYMEDIA = ${SKYMEDIA}")
message(STATUS "SENTRY = ${SENTRY}")

target_link_libraries(VOO
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::QuickWidgets
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Xml
    ${SKYMEDIA}
    ${OF_CORE}
    ${OF_EFFECT}
    ${SENTRY}
    ${GLEW}
    ${VENUS}
    ${VENUS_AUDIO_DENOISE}
    ${VN_CLASSIFYING}
    ${VENUS_CORE}
    ${VENUS_DOCRECT}
    ${VENUS_FACE}
    ${VENUS_FACE_REENACT}
    ${VENUS_FACE_PARSER}
    ${VENUS_GESTURE}
    ${VN_GENERAL}
    ${VENUS_HIIDO}
    ${VENUS_IMAGENET}
    ${VENUS_KIT}
    ${VENUS_OBJCOUNT}
    ${VENUS_POSE}
    ${VENUS_RPC}
    ${VENUS_SEGMENT}
    ${VENUS_VAD}
    QuaZip::QuaZip
    MMKV
    ${METAL}
    ${METAL_KIT}
    ${METAL_PERFORMANCE_SHADERS}
    ${QUARTZCORE}
    ${CORE_MEDIA}
    ${AVFOUNDATION}
    ${AVKit}
    ${Accelerate}
    ${COCOA}
    )

if(APPLE)
    message(STATUS "METAL PATH=${METAL}")
    message(STATUS "METAL_KIT PATH=${METAL_KIT}")
    message(STATUS "METAL_PERFORMANCE_SHADERS PATH=${METAL_PERFORMANCE_SHADERS}")
    message(STATUS "QUARTZCORE PATH=${QUARTZCORE}")
elseif(WIN32)
    target_link_libraries(VOO PRIVATE ${Qt5AxContainer_LIBRARIES})
endif()
