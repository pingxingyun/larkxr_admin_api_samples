cmake_minimum_required(VERSION 3.0.0)
project(larkxr_samples VERSION 0.1.0)

##
# options
##
## for source tree
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_WIN "build on windows" OFF)
option(BUILD_APPLE "build on apple system" OFF)
option(BUILD_UNIX "build on unix system" OFF)

if (WIN32)
    message(STATUS "open build win32 apps. ${CMAKE_SYSTEM_NAME}")
    set(BUILD_WIN ON)
elseif (APPLE)
    message(STATUS "open build apple apps.${CMAKE_SYSTEM_NAME}")
    set(BUILD_APPLE ON)
elseif (UNIX)
    message(STATUS "open build unix app.${CMAKE_SYSTEM_NAME}")
    set(BUILD_UNIX ON)
endif()

# flags
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fexceptions -std=c++11")
if (BUILD_WIN)
    # add_definitions(-DMT -DMTD)
endif()
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# project setup
# setup unicode
add_definitions(-DUNICODE -D_UNICODE)
# use dll import
add_definitions(-DLARK_XR_DLL)

# win exe
if (BUILD_WIN)
    message(STATUS "build win32 apps. ${CMAKE_SYSTEM_NAME}")
    # add_subdirectory(src/app/desktop)

    # test
    # project root dir from test.
    add_definitions(
        -DLK_PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
        -DWIN32_LEAN_AND_MEAN="1"
    )

    add_subdirectory(src/lib_larkxradminapi)
    add_subdirectory(src/sample_upload_file)
endif (BUILD_WIN)

# mac exe
# TODO
#if (BUILD_APPLE)
#    message(STATUS "build apple apps.${CMAKE_SYSTEM_NAME}")
#    add_subdirectory(src/app/mac)
#endif(BUILD_APPLE)
# linux freebsd
# TODO
#if (BUILD_UNIX)
#    message(STATUS "build unix app.${CMAKE_SYSTEM_NAME}")
#endif(BUILD_UNIX)