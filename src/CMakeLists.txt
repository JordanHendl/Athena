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

SET( KARMA_TEST_SOURCES 
  Manager.cpp
)

SET( KARMA_TEST_HEADERS
  Manager.h
)

ADD_LIBRARY               ( karma_test SHARED  ${KARMA_TEST_SOURCES} ${KARMA_TEST_HEADERS} )
TARGET_LINK_LIBRARIES     ( karma_test PUBLIC  ${KARMA_TEST_LIBRARIES}                     )
TARGET_INCLUDE_DIRECTORIES( karma_test PRIVATE ${KARMA_TEST_INCLUDE_DIRS}                  )

ADD_EXECUTABLE       ( karma_test_test Test.cpp   )
TARGET_LINK_LIBRARIES( karma_test_test karma_test )

INSTALL( FILES  ${KARMA_TEST_HEADERS} DESTINATION ${INCLUDE_DIR} )
INSTALL( TARGETS karma_test EXPORT kt DESTINATION ${LIB_DIR}     )