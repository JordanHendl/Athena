# (K)arma (T)est Library.
  A C++ Library meant to make testing quick, easy, and also not annoying.

## Project Integration
  KT uses **CMake** for building.

  On Linux, do:

  ```
  mkdir build 
  cd build
  cmake ..
  make package
  sudo rpm --install <kt_rpm_name>.rpm
  ```

  On window, I recommend using the CMake GUI & MinGW-w64 for building. It uses NSIS for installation.

  KT installs in default locations ( /usr/local/KT/ on Linux, C:\Program Files\KT\ on Windows ), and provides a KTConfig.cmake.

  To use in your own CMake project:

  1) Append a path to KTConfig.cmake to your CMAKE_PREFIX_PATH ( I recommend copying KTConfig.cmake to a cmake module directory in your project folder )
  2) FIND_PACKAGE( KT )
  3) Link whatever you with against *karma_test* and it should be good to go!

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
    karma::test::Manager manager ;
    Object               object  ;
    
    manager.add( "success", &success ) ;
    manager.add( "failure", &failure ) ;
    
    return manager.test( karma::test::Output::Verbose ) ;
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
