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
#include <map>
#include <string>
#include <stdio.h>
#include <string>
#include <chrono>
#include <iostream>
#include <tuple>
#include <iomanip>

namespace athena
{
  /** Alias for a chrono time point since that name is super long.
   */
  typedef std::chrono::steady_clock::time_point TimePoint ;

  /** Function to get a name from a result.
   * @param result The result to convert to a name
   * @return The name of the result.
   */
  const char* nameFromResult( Result result )
  {
    switch( result.value() )
    {
      case Result::Pass :
        return "Passed" ;
      case Result::Fail :
        return "Failed" ;
      case Result::Skip :
        return "Skipped" ;
      default:
        return "Unknown" ;
    };
  }
  
  /** Function to obtain a symbol from a result.
   * @param result The result to pull a symbol from.
   * @return The symbol representation of the result.
   */
  const char* symbolFromResult( Result result )
  {
    switch( result.value() )
    {
      case Result::Pass :
        return " ✔ " ;
      case Result::Fail :
        return " ✗ " ;
      case Result::Skip :
        return " ■ " ;
      default:
        return "???" ;
    };
  }
  
  /** Function to extract a name from an output.
   * @param output The output to recieve a name from.
   * @return The name of the output.
   */
  const char* nameFromOutput( Output output )
  {
    switch( output.value() )
    {
      case Output::Normal :
        return "Normal" ;
      case Output::Quiet :
        return "Quiet" ;
      case Output::Verbose :
        return "Verbose" ;
      default:
        return "Unknown" ;
    };
  }

  struct ManagerData
  {
    typedef std::tuple<Result, double>                TestResult  ;
    typedef std::map<std::string, TestResult>         ResultMap   ;
    typedef std::map<std::string, Manager::Callback*> CallbackMap ;
    
    CallbackMap callbacks ;
    ResultMap   results   ;
    
    /** Method to process test results.
     */
    unsigned processResults( Output output ) ;

    /** Method to print the test information.
     * @param result The rest result.
     * @param output The output type.
     */
    void printTest( std::string name, double time, Result result, Output output ) ;
    
    /** Method to print the test summary.
     * @param passed The amount of passed tests.
     * @param failed The amount of failed tests.
     * @param skipped The amount of skipped tests.
     * @param total_time The total time to test all tests.
     * @param output The output type.
     */
    void printSummary( unsigned passed, unsigned failed, unsigned skipped, double total_time, Output output ) ;
  };
  
  unsigned ManagerData::processResults( Output output )
  {
    unsigned    num_passed  ;
    unsigned    num_skipped ;
    unsigned    num_failed  ;
    unsigned    total_tests ;
    double      time        ;
    double      total_time  ;
    Result      result      ;
    std::string name        ;
    
    total_time  = 0.0 ;
    num_passed  = 0   ;
    num_skipped = 0   ;
    num_failed  = 0   ;
    total_tests = 0   ;
    
    for( const auto& res : this->results )
    {
      name   = res.first ;
      result = std::get<0>( res.second ) ;
      time   = std::get<1>( res.second ) ;
      
      total_time += time ;
      
      switch( result.value() )
      {
        case Result::Fail :
          num_failed++  ;
          total_tests++ ;
          break ;
        case Result::Pass :
          num_passed++  ;
          total_tests++ ;
          break ;
        case Result::Skip :
          num_skipped++ ;
          break ;
        default :
          break ;
      };
      
      this->printTest( name, time, result, output ) ;
    }
    
    this->printSummary( num_passed, num_failed, num_skipped, total_time, output ) ;

    return num_failed ;
  }
  
  void ManagerData::printTest( std::string name, double time, Result result, Output output )
  {
    if( output.value() != Output::Quiet )
    {
      std::cout << "\n"                                                                ;
      std::cout << "| " << result.symbol() << " | " << name << " -> " << result.name() ;

      if( output.value() == Output::Verbose )
      {
        std::cout << "\n" ;
        if( time < 1000.0 )
        {
          std::cout << "    └─Time: " << std::setprecision( 5 ) << time << "μs" ;
        }
        else
        {
          std::cout << "    └─Time: " << std::setprecision( 5 ) << time / 1000.0 << "ms" ;
        }
      }
    }
  }
  
  void ManagerData::printSummary( unsigned passed, unsigned failed, unsigned skipped, double total_time, Output output )
  {
    unsigned total = passed + failed ;
    std::cout << std::endl << std::endl ;

    std::cout << "    Passed : " << passed  << " / " << total << "\n" ;
    std::cout << "    Failed : " << failed  << " / " << total << "\n" ;
    std::cout << "    Skipped: " << skipped <<                   "\n" ;
    
    if( output.value() == output.Verbose )
    {
      if( total_time >= 1000.0 )
      std::cout << "\n    Total Elapsed Time : " << std::setprecision( 5 ) << total_time / 1000 << "ms" << std::endl ;
      else
      std::cout << "\n    Total Elapsed Time : " << std::setprecision( 5 ) << total_time << "μs" << std::endl ;
    }
  }

  Result::Result()
  {
    this->enumeration = Result::Skip ;
  }

  Result::Result( const Result& result )
  {
    this->enumeration = result.enumeration ;
  }

  Result::Result( unsigned result )
  {
    this->enumeration = result ;
  }

  Result& Result::operator=( const Result& result )
  {
    this->enumeration = result.enumeration ;
    
    return *this ;
  }

  Result& Result::operator=( unsigned result )
  {
    this->enumeration = result ;
    
    return *this ;
  }

  Result& Result::operator=( bool result )
  {
    this->enumeration = result ;
    
    return *this ;
  }

  const char* Result::name() const
  {
    return nameFromResult( *this ) ;
  }

  const char* Result::symbol() const
  {
    return symbolFromResult( *this ) ;
  }

  void Result::set( unsigned result )
  {
    this->enumeration = result ;
  }

  Result::operator unsigned() const
  {
    return this->enumeration ;
  }

  Result::operator bool() const
  {
    return static_cast<bool>( this->enumeration == 1 ? true : false ) ;
  }

  unsigned Result::value() const
  {
    return this->enumeration ;
  }
  
  Output::Output( unsigned output_type )
  {
    this->enumeration = output_type ;
  }

  Output::operator unsigned() const
  {
    return this->enumeration ;
  }

  Output& Output::operator=( unsigned output_type )
  {
    this->enumeration = output_type ;
    
    return *this ;
  }

  const char* Output::name() const
  {
    return nameFromOutput( *this ) ;
  }

  void Output::set( unsigned output_type )
  {
    this->enumeration = output_type ;
  }

  unsigned Output::value() const
  {
    return this->enumeration ;
  }
  
  Manager::Manager()
  {
    this->manager_data = new ManagerData() ;
  }

  Manager::~Manager()
  {
    delete this->manager_data ;
  }

  unsigned Manager::size() const
  {
    return data().callbacks.size() ;
  }

  unsigned Manager::test( const Output& output )
  {
    TimePoint begin       ;
    TimePoint end         ;
    double    time        ;
    Result    result      ;
    
    for( auto& cb : data().callbacks )
    {
      begin  = std::chrono::steady_clock::now() ;
      result = cb.second->test() ;
      end    = std::chrono::steady_clock::now() ;
      
      if( result.value() == Result::Skip )
      {
        time = 0.0 ;
      }
      else
      {
        time = std::chrono::duration<double, std::micro>( end - begin ).count() ;
      }
      
      data().results.insert( { cb.first, std::make_tuple( result, time ) } ) ;
    }
    
    return data().processResults( output ) ;
  }

  void Manager::add( const char* name, Callback* callback )
  {
    data().callbacks.insert( { std::string( name ), callback } ) ;
  }

  ManagerData& Manager::data()
  {
    return *this->manager_data ;
  }

  const ManagerData& Manager::data() const
  {
    return *this->manager_data ;
  }
}
