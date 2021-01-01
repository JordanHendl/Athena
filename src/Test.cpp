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

class Object
{
  public:
    Object() = default ;
    ~Object() = default ;
    
    karma::test::Result method1() ;
    
    karma::test::Result method2() ;
    
    bool method3() ;
};

karma::test::Result Object::method1()
{
  return karma::test::Result::Pass ;
}

karma::test::Result Object::method2()
{
  return karma::test::Result::Skip ;
}

bool Object::method3()
{
  return false ;
}

bool success()
{
  return true ;
}

bool failure()
{
  return false ;
}

int main()
{
  karma::test::Manager manager ;
  Object               object  ;
  
  manager.add( "object_test1", &object, &Object::method1 ) ;
  manager.add( "object_test2", &object, &Object::method2 ) ;
  manager.add( "object_test3", &object, &Object::method3 ) ;
  manager.add( "success"     , &success                  ) ;
  manager.add( "failure"     , &failure                  ) ;
  
  assert( manager.test( karma::test::Output::Verbose ) == 2 ) ;
  
  return 0 ;
}