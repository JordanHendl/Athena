Karma test library.

Example usage: 
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


Example output:
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
