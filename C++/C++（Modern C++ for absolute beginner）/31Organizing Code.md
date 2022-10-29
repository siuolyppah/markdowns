# Header Files

- Header files are source code files where we ==usually put various ***declarations***==.

- Header Guards:

  ```cpp
  #ifndef MY_HEADER_H
  #define MY_HEADER_H
  // header file source code
  // goes here
  #endif
  ```

  



# Source Files

- Source files are files where we can ==store our ***definitions*** and the main program==.



# What does Complier do

1. The compiler ==stitches== the code from the header file and the source file together and produces what is called a translation unit.
2. The compiler then uses this file to create an object file.
3.  A linker then links object files together to create a program.



# Namespace

- If we have several separate namespaces with the same name in our code, this means we are ==extending that namespace(rather than redefining it)==. 

- A namespace ==can be spread across multiple files, both headers and source files==.