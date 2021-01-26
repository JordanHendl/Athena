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

namespace athena
{
  /** Class to encapsulate a test result.
   */
  class Result
  {
    public:
      
      /** Enumeration depicting possible test results.
       */
      enum
      {
        Fail = false,
        Pass = true,
        Skip = 2,
      };
      
      /** Default constructor.
       */
      Result() ;
      
      /** Copy constructor. Copies the input into this object.
       * @param result The object to copy.
       */
      Result( const Result& result ) ;
      
      /** Copy constructor. Copies the input into this object.
       * @param result The object to copy.
       */
      Result( unsigned result ) ;
      
      /** Assignment operator. Assigns this enumeration to the input.
       * @param result The input to assing this enumeration to.
       * @return A Reference of this object after assignment.
       */
      Result& operator=( const Result& result ) ;
      
      /** Assignment operator. Assigns this enumeration to the input.
       * @param result The input to assing this enumeration to.
       * @return A Reference of this object after assignment.
       */
      Result& operator=( unsigned result ) ;
      
      /** Assignment operator. Assigns this enumeration to the input.
       * @param result The input to assing this enumeration to.
       * @return A Reference of this object after assignment.
       */
      Result& operator=( bool result ) ;
      
      /** Method to retrieve the string representation of this test result.
       * @return The C-string representation of this test result.
       */
      const char* name() const ;
      
      /** Method to retrieve the symbol representing this test result for pass, fail, and skip ( ✔, ✗, ■ ).
       * @return The ASCII Symbol representing this test result.
       */
      const char* symbol() const ;
      
      /** Method to set the result of this object.
       * @param result The input to set this object to.
       */
      void set( unsigned result ) ;
      
      /** Conversion operator, to convers this test result to an unsigned integer.
       * @return An unsigned integer representation of this test result.
       */
      operator unsigned() const ;
      
      /** Conversion operator, to convers this test result to a boolean.
       * @return A boolean representation of this test result.
       */
      operator bool() const ;
      
      /** Method to retrieve the value of this object.
       * @return The unsigned-integer value representing this test result.
       */
      unsigned value() const ;
    private:
      
      /** The underlying enumeration of this test result.
       */
      unsigned enumeration ;
  };
  
  /** Class to handle output options of tests.
   */
  class Output
  {
    public:
      
      /** Enumeration for this object.
       */
      enum
      {
        Quiet,
        Normal,
        Verbose
      };
      
      /** Default constructor.
       */
      Output() ;
      
      /** Copy constructor
       * @param output_type The input to copy into this object.
       */
      Output( unsigned output_type ) ;
      
      /** Conversion operator to convert this output type to an unsigned integer.
       * @return An unsigned integer representation of this object.
       */
      operator unsigned() const ;
      
      /** Assignment operator. Assigns this object to the input.
       * @param output_type The unsigned integer to assign this output type to.
       * @return Reference to this object after assignment.
       */
      Output& operator=( unsigned output_type ) ;
      
      /** Method to retrieve the string name of this output type.
       * @return C-string representation of this object's name.
       */
      const char* name() const ;
      
      /** Method to set this object's value.
       * @param output_type The unsigned integer to set this object to.
       */
      void set( unsigned output_type ) ;
      
      /** Method to retrieve the value of this object.
       * @return The unsigned-integer representation of this object.
       */
      unsigned value() const ;
    private:
      
      /** The internal enumeration of this object.
       */
      unsigned enumeration ;
  };
  
  /** Class to manage tests.
   */
  class Manager
  {
    public:
      
      /** Default constructor.
       */
      Manager() ;
      
      /** Default deconstructor.
       */
      ~Manager() ;
      
      /** Method to initialize the manager.
       * @param program_name The name of the program/product being tested.
       */
      void initialize( const char* program_name ) ;

      /** Method to add a test callback to this object.
       * @param name The name of the test to associate with the callback.
       * @param callback The function to assess as a test.
       */
      template<typename TYPE>
      void add( const char* name, TYPE (*callback)() ) ;

      
      /** Method to add a test method callback to this object.
       * @param name The name of the test to associate with the callback.
       * @param object The object to call the callback function with.
       * @param callback The function to assess as a test.
       */
      template< typename OBJECT, typename TYPE>
      void add( const char* name, OBJECT* obj, TYPE (OBJECT::*callback)() ) ;
      
      /** Method to retrieve the number of tests being performed by this object.
       * @return 
       */
      unsigned size() const ;
      
      /** Method to perform all tests recorded in this objecct.
       * @param output The desired way to output this object's tests.
       * @return The number of passed
       */
      unsigned test( const Output& output ) ;
    private:
      
      /** Virtual class to encapsulate a callback.
       */
      class Callback
      {
        public:
          virtual ~Callback() = default ;
          
          virtual Result test() = 0 ;
      };
      
      /** Template class to encapsulate a function callback.
       */
      template<typename TYPE>
      class Function : public Callback
      {
        public:
          typedef TYPE ( *Callback )() ;
          
          Function( Callback callback ) ;
          ~Function() = default ;
          Result test() ;
        private:
          Callback callback ;
      };
      
      
      /** Template class to encapsulate a method callback.
       */
      template<typename OBJECT, typename TYPE>
      class Method : public Callback
      {
        public:
          
          typedef TYPE ( OBJECT::*Callback )() ;
          
          Method( OBJECT* object, Callback callback ) ;
          
          ~Method() = default ;
          
          Result test() ;
        private:
          
          OBJECT*  object   ;
          Callback callback ;
      };
      
      /** Method to add a callback to this object's callback map.
       * @param name The name associated with the test.
       * @param callback The test callback to use for testing.
       */
      void add( const char* name, Callback* callback ) ;
      
      
      /** Forward-declared structure to contain this object's internal data.
       */
      struct ManagerData *manager_data ;
      
      /** Friend declaration.
       */ 
      friend struct ManagerData ;
      
      /** Method to retrieve a reference to this object's internal data.
       * @return Reference to this object's internal data.
       */
      ManagerData& data() ;
      
      /** Method to retrieve a const reference to this object's internal data.
       * @return Const-reference to this object's internal data.
       */
      const ManagerData& data() const ;
  };
  
  template<typename TYPE>
  Manager::Function<TYPE>::Function( Callback callback )
  {
    this->callback = callback ;
  }
  
  template<typename TYPE>
  Result Manager::Function<TYPE>::test()
  {
    return static_cast<Result>( ( this->callback )() ) ;
  }
  
  template<typename OBJECT, typename TYPE>
  Manager::Method<OBJECT, TYPE>::Method( OBJECT* object, Callback callback )
  {
    this->callback = callback ;
    this->object   = object   ;
  }
  
  template<typename TYPE>
  void Manager::add( const char* name, TYPE (*callback)() )
  {
    this->add( name, dynamic_cast<Manager::Callback*>( new Manager::Function<TYPE>( callback ) ) ) ;
  }
  
  template<typename OBJECT, typename TYPE>
  void Manager::add( const char* name, OBJECT* object, TYPE ( OBJECT::*callback )() )
  {
    this->add( name, dynamic_cast<Manager::Callback*>( new Manager::Method<OBJECT, TYPE>( object, callback ) ) ) ;
  }

  template<typename OBJECT, typename TYPE>
  Result Manager::Method<OBJECT, TYPE>::test()
  {
    return static_cast<Result>( ( ( this->object )->*( this->callback ) )() ) ;
  }
}