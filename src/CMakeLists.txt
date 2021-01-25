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

SET( ATHENA_SOURCES 
  Manager.cpp
)

SET( ATHENA_HEADERS
  Manager.h
)

ADD_LIBRARY               ( athena SHARED  ${ATHENA_SOURCES} ${ATHENA_HEADERS} )
TARGET_LINK_LIBRARIES     ( athena PUBLIC  ${ATHENA_LIBRARIES}                 )
TARGET_INCLUDE_DIRECTORIES( athena PRIVATE ${ATHENA_INCLUDE_DIRS}              )

ADD_EXECUTABLE       ( athena_test Test.cpp   )
TARGET_LINK_LIBRARIES( athena_test athena     )

INSTALL( FILES ${ATHENA_HEADERS} DESTINATION ${HEADER_INSTALL_DIR} )

INSTALL( TARGETS athena    EXPORT Athena
         RUNTIME  DESTINATION  ${LIB_DIR}
         ARCHIVE  DESTINATION  ${LIB_DIR}
         LIBRARY  DESTINATION  ${LIB_DIR}
         INCLUDES DESTINATION  ${INCLUDE_DIR}
        )
