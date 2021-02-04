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
#if defined ( __unix__ ) || defined( _WIN32 )
  constexpr const char* END_COLOR   = "\x1B[m"     ;
  constexpr const char* COLOR_GREEN = "\u001b[32m" ;
  constexpr const char* COLOR_GREY  = "\x1B[1;30m" ;
  constexpr const char* COLOR_RED   = "\u001b[31m" ;
  constexpr const char* COLOR_WHITE = "\u001b[1m"  ;
  constexpr const char* UNDERLINE   = "\u001b[4m"  ;
#else
  constexpr const char* END_COLOR   = "" ;
  constexpr const char* COLOR_GREEN = "" ;
  constexpr const char* COLOR_GREY  = "" ;
  constexpr const char* COLOR_RED   = "" ;
  constexpr const char* COLOR_WHITE = "" ;
#endif
  
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
        return "Pass" ;
      case Result::Fail :
        return "Fail" ;
      case Result::Skip :
        return "Skip" ;
      default:
        return "Unknown" ;
    };
  }
  
  /** Function to obtain a color from a test result.
   * @param result The result to retrieve a color for.
   * @return A color representing the result.
   */
  const char* colorFromResult( Result result )
  {
    switch( result.value() )
    {
      case Result::Pass :
        return COLOR_GREEN ;
      case Result::Fail :
        return COLOR_RED ;
      case Result::Skip :
        return COLOR_GREY ;
      default:
        return COLOR_RED ;
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
    
    CallbackMap callbacks    ;
    ResultMap   results      ;
    std::string program_name ;

    /** Method to print the Athena header.
     */
    void printHeader() const ;

    /** Method to process test results.
     */
    unsigned processResults( Output output ) ;

    /** Method to print the test information.
     * @param result The rest result.
     * @param output The output type.
     * @param last Whether or not this is the last test.
     */
    void printTest( std::string name, double time, Result result, Output output, bool last ) ;
    
    /** Method to print the test summary.
     * @param passed The amount of passed tests.
     * @param failed The amount of failed tests.
     * @param skipped The amount of skipped tests.
     * @param total_time The total time to test all tests.
     * @param output The output type.
     */
    void printSummary( unsigned passed, unsigned failed, unsigned skipped, double total_time, Output output ) ;
  };
  
  void ManagerData::printHeader() const
  {
    std::cout << "\n" ;
    std::cout << COLOR_WHITE << std::string( 60, '-' ) << "\n" ;
    std::cout << COLOR_WHITE << " ATHENA: " << this->program_name ;
    std::cout << COLOR_WHITE << std::string( 50 - this->program_name.size(), ' ' ) << "|\n" ;
    std::cout << COLOR_WHITE << std::string( 60, '-' ) << "\n" ;
    std::cout << END_COLOR ;
  }
  
  unsigned ManagerData::processResults( Output output )
  {
    unsigned    num_passed  ;
    unsigned    num_skipped ;
    unsigned    num_failed  ;
    unsigned    total_tests ;
    unsigned    count       ;
    double      time        ;
    double      total_time  ;
    Result      result      ;
    std::string name        ;
    
    total_time  = 0.0 ;
    num_passed  = 0   ;
    num_skipped = 0   ;
    num_failed  = 0   ;
    total_tests = 0   ;
    count       = 0   ;
    
    this->printHeader() ;
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
      
      count++ ;
      this->printTest( name, time, result, output, count == this->results.size() ) ;
    }
    
    this->printSummary( num_passed, num_failed, num_skipped, total_time, output ) ;

    return num_failed ;
  }
  
  void ManagerData::printTest( std::string name, double time, Result result, Output output, bool last )
  {
    const std::string separator = last ? "└─" : "├─" ;
    if( output.value() != Output::Quiet )
    {
      std::cout << colorFromResult( result ) << std::left << "--|" << result.symbol() << result.name() << " " ;

      if( output.value() == Output::Verbose && result.value() != Result::Skip )
      {
        if( time < 1000.0 )
        {
          std::cout << std::right << std::setw( 6 ) << std::setfill('0') << std::setprecision( 2 ) << std::fixed << time << "μs" ;
        }
        else
        {
          std::cout << std::right << std::setw( 6 ) << std::setfill('0') << std::setprecision( 2 ) << std::fixed << time / 1000.0 << "ms" ;
        }
      }
      else if( result.value() == Result::Skip )
      {
        std::cout << "NONE" << std::setw( 4 ) << std::setfill(' ') << " " ;
      }
      
      std::cout << std::left << " " << separator << " " << name << " " ;
      std::cout << END_COLOR                                    ;
      if( !last ) std::cout << "\n"                             ;
    }
  }
  
  void ManagerData::printSummary( unsigned passed, unsigned failed, unsigned skipped, double total_time, Output output )
  {
    unsigned total = passed + failed ;
    std::cout << std::endl ;
    std::cout << std::endl ;
    
    const char* color = passed == total ? COLOR_GREEN : COLOR_RED ;
    std::cout << color << "  Test Results: " << END_COLOR << "\n"                              ;
    std::cout << color << "     ├─Passed : " << passed  << " / " << total << END_COLOR << "\n" ;
    std::cout << color << "     ├─Failed : " << failed  << " / " << total << END_COLOR << "\n" ;
    std::cout << color << "     └─Skipped: " << skipped <<                   END_COLOR << "\n" ;
    
    if( output.value() == output.Verbose )
    {
      if( total_time >= 1000.0 )
      std::cout << COLOR_WHITE << "\n  Total Elapsed Time : " << std::setprecision( 5 ) << total_time / 1000 << "ms" << END_COLOR << std::endl ;
      else
      std::cout << COLOR_WHITE << "\n  Total Elapsed Time : " << std::setprecision( 5 ) << total_time << "μs" << END_COLOR << std::endl ;
    }
    
    std::cout << COLOR_WHITE << std::string( 60, '-' ) << END_COLOR << "\n" << std::endl ;
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
  
  void Manager::initialize( const char* program_name )
  {
    data().program_name = program_name ;
  }

  unsigned Manager::size() const
  {
    return data().callbacks.size() ;
  }

  unsigned Manager::test( const Output& output )
  {
    TimePoint begin  ;
    TimePoint end    ;
    double    time   ;
    Result    result ;
    
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
