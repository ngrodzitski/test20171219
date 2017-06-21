# Copyright 2013-2017, Corvusoft Ltd, All Rights Reserved.
cmake_minimum_required( VERSION 2.8.10 )

project( restbed CXX )
set( restbed_VERSION_MAJOR 4 )
set( restbed_VERSION_MINOR 5.0 )
set( restbed_VERSION ${restbed_VERSION_MAJOR}.${restbed_VERSION_MINOR} )
set( PACKAGE_VERSION ${restbed_VERSION} )
set( VERSION "${restbed_VERSION}" )

#
# Build Options
#
option( BUILD_SHARED   "Build shared library."              ON )

#
# Configuration
#
include( "${PROJECT_SOURCE_DIR}/cmake/configuration.cmake" )

message( "   ${Blue}Copyright 2013-2017, Corvusoft Ltd, All Rights Reserved.${Reset}" )

#
# Dependencies
#
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules" )

find_package( asio REQUIRED )
include_directories( SYSTEM ${asio_INCLUDE} )

find_package( kashmir REQUIRED )
include_directories( SYSTEM ${kashmir_INCLUDE} )

#
# Build
#
include( "${PROJECT_SOURCE_DIR}/cmake/manifest.cmake" )

include_directories( ${INCLUDE_DIR} )

add_library( ${PROJECT_NAME} ${MANIFEST} )

if ( BUILD_SHARED )
    set_target_properties( ${PROJECT_NAME} PROPERTIES SOVERSION ${restbed_VERSION_MAJOR} VERSION ${restbed_VERSION} )
endif ( )


#
# Install
#
include( "${PROJECT_SOURCE_DIR}/cmake/artifacts.cmake" )

if ( NOT DEFINED CMAKE_INSTALL_LIBDIR )
    set( CMAKE_INSTALL_LIBDIR library )
endif ( )

add_subdirectory( "${PROJECT_SOURCE_DIR}/benches" )
install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib )
