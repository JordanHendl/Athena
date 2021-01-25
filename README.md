# Athena
  A C++ Library meant to make testing quick, easy, and also not annoying.

## Project Integration
  KT uses **CMake** for building.

  On Linux, do:

  ```
  mkdir build 
  cd build
  cmake ..
  make package
  Use package manager to install
  ```

  On window, I recommend using the CMake GUI & MinGW-w64 for building. It uses NSIS for installation.

  Athena installs in default locations ( /usr/local/Athena/ on Linux, C:\Program Files\Athena\ on Windows ), and provides a AthenaConfig.cmake.

  To use in your own CMake project:

  1) Append a path to the install to your CMAKE_PREFIX_PATH.
  2) FIND_PACKAGE( Athena )
  3) Link whatever you with against *athena* and it should be good to go!

## Example usage: 
  ```
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
    athena::Manager manager ;
    Object          object  ;
    
    manager.add( "success", &success ) ;
    manager.add( "failure", &failure ) ;
    
    return manager.test( athena::Output::Verbose ) ;
  }
  ```

## Example output:
  ```
  | ✗ | failure -> Failed
      └─Time: 0.871μs
  | ✔ | success -> Passed
      └─Time: 0.082μs
  
      Passed : 1 / 2
      Failed : 1 / 2
      Skipped: 0
  
      Total Elapsed Time : 0.953μs
  ```
