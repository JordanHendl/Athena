# 
# Copyright (C) 2020 Jordan Hendl
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

# 
# File:   CMakeLists.h
# Author: Jordan Hendl
# 
# Created on December 31, 2020, 11:30 PM
# 

CMAKE_MINIMUM_REQUIRED( VERSION 3.8.0 )

LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

PROJECT( KT CXX )

# Set build config.
SET( ARCHITECTURE "64bit" CACHE STRING "The system architecture."              )
SET( CXX_STANDARD "17"    CACHE STRING "The C++ standard to use for building." )
SET( MAJOR        "0"     CACHE STRING "The major version of this build."      )
SET( MINOR        "1"     CACHE STRING "The minor version of this build."      )
SET( BRANCH       "0"     CACHE STRING "The branch version of this build."     )
SET( REVISION     "0"     CACHE STRING "The revision of this build."           )

SET( PROJECT_VERSION "${MAJOR}.${MINOR}.${BRANCH}.${REVISION}" )

# Print build configurations.
MESSAGE( " " ) 
MESSAGE( "Build Configuration:"                 )
MESSAGE( "├─ARCHITECTURE:   ${ARCHITECTURE}"    )
MESSAGE( "├─CXX_STANDARD:   ${CXX_STANDARD}"    )
MESSAGE( "└─VERSION:        ${PROJECT_VERSION}" )
MESSAGE( "  ├─MAJOR:        ${MAJOR}"           )
MESSAGE( "  ├─MINOR:        ${MINOR}"           )
MESSAGE( "  ├─BRANCH:       ${BRANCH}"          )
MESSAGE( "  └─REVISION:     ${REVISION}"        )

STRING( TIMESTAMP DATE "%Y%m%d" )
STRING( TIMESTAMP Time "%H%M%S" )

# Handle compiler information
STRING( TOLOWER ${CMAKE_CXX_COMPILER_ID} CXX_COMPILER_NAME )
SET( CXX_COMPILER  "${CXX_COMPILER_NAME}${CMAKE_CXX_COMPILER_VERSION}" )

SET( CLANG FALSE )
SET( INTEL FALSE )
SET( GNU   FALSE )
SET( MSVC  FALSE )
IF( CXX_COMPILER_NAME MATCHES "clang" )
  SET( CLANG TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "gnu" )
  SET( GNU TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "intel" )
  SET( INTEL TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "msvc" )
  SET( MSVC TRUE )
ENDIF()

MESSAGE( " " ) 
MESSAGE(  "System Configuration:"                     )
MESSAGE(  "├─CXX_COMPILER_NAME: ${CXX_COMPILER_NAME}" )
MESSAGE(  "├─CXX_COMPILER:      ${CXX_COMPILER}"      )
MESSAGE(  "├─INTEL:             ${INTEL}"             )
MESSAGE(  "├─GNU:               ${GNU}"               )
MESSAGE(  "└─MSVC:              ${MSVC}"              )
MESSAGE( " " ) 

SET( BIN_DIR     "bin"     )
SET( INCLUDE_DIR "include" )
SET( LIB_DIR     "lib64"   )
SET( DOC_DIR     "doc"     )
SET( CONFIG_DIR  "KT"      )

INCLUDE( KPackage ) 

# Set output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIB_DIR} )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIB_DIR} )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BIN_DIR} )
set(DOCUMENTATION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${DOC_DIR} )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/src ) 

ADD_SUBDIRECTORY( src )

#EXPORT( TARGETS karma_test FILE ${LIB_DIR}/KTConfig.cmake )

INSTALL( EXPORT kt FILE KTConfig.cmake NAMESPACE Karma:: DESTINATION ${CONFIG_INSTALL_DIR} )