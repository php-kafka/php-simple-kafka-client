# How to contribute
All contributions to the project are welcome.
Please keep the following in mind when contributing:

## Branches
Pull requests should be made against the main branch, which supports both PHP 7 and PHP 8.

## Testing
Tests are in phpt file format in the tests directory.

### Using your own machine for building and testing. 
Tests can be run by following compilation and installation procedure 
and executing `make test`.  
To run integration tests, make sure you have Kafka instance running.  
Then, rename `test_env.php.sample` to `test_env.php` and adjust it  
with values proper for your kafka instance.
