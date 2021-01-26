/*
 * Copyright (C) 2020 Jordan Hendl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Manager.h"
#include <assert.h>
#include <thread>

class Object
{
  public:
    Object() = default ;
    ~Object() = default ;
    
    athena::Result method1() ;
    
    athena::Result method2() ;
    
    bool method3() ;
};

athena::Result Object::method1()
{
  std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) ) ;
  return athena::Result::Pass ;
}

athena::Result Object::method2()
{
  std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) ) ;
  return athena::Result::Skip ;
}

bool Object::method3()
{
  std::this_thread::sleep_for( std::chrono::milliseconds( 15 ) ) ;
  return false ;
}

bool success()
{
  std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) ) ;
  return true ;
}

bool failure()
{
  std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) ) ;
  return false ;
}

int main()
{
  athena::Manager manager ;
  Object          object  ;
  
  manager.initialize( "Athena Unit Tester" ) ;
  manager.add( "object_test1", &object, &Object::method1 ) ;
  manager.add( "object_test2", &object, &Object::method2 ) ;
  manager.add( "object_test3", &object, &Object::method3 ) ;
  manager.add( "success"     , &success                  ) ;
  manager.add( "failure"     , &failure                  ) ;
  
  assert( manager.test( athena::Output::Verbose ) == 2 ) ;
  
  return 0 ;
}