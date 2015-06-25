# Orange
Orange is a systems programming language with the feel of a mixture of C# and Ruby. 

# Hello World
At the moment, Orange has no standard library, so you'll need to link against C library functions to do more 
complicated tasks:

    extern int32 printf(char *s, ...)

    for (i = 0; i < 10; i++) 
        printf("Hello, world %d!\n", i)
    end
    
# Resources

* [Language Guide](https://github.com/orange-lang/orange/wiki/Language-Specification)
* [Tests](/test/)

# Installing 
Installing on OS X and Linux should be pretty simple; after having all of the dependencies installed, building is as easy as:

```sh 
$ mkdir build-orange && cd build-orange 
$ cmake ../orange
$ make all install
``` 

[Building on Windows](https://github.com/orange-lang/orange/wiki/Building-on-Windows) takes a bit more effort.

## Dependencies 

* Boost 
* Flex
* Bison
* CMake
* LLVM
