IF( WIN32 )
 SET( CPACK_GENRATOR          "NSIS"                        )
 SET( CPACK_NSIS_PACKAGE_NAME install_${CMAKE_PROJECT_NAME} )
 SET( CONFIG_INSTALL_DIR      "cmake_config"                )
ENDIF() 

IF( UNIX )
  SET( CPACK_GENERATOR    "RPM"          )
  SET( CONFIG_INSTALL_DIR "cmake_config" )
ENDIF() 

SET( CPACK_PACKAGE_VERSION        ${PROJECT_VERSION}            )
SET( CPACK_PACKAGE_NAME           install_${CMAKE_PROJECT_NAME} )
SET( CPACK_INSTALL_PREFIX         ${CMAKE_INSTALL_PREFIX}       )

INCLUDE( CPack )