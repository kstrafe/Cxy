## Unnamed Language ##
The repository for The Unnamed Language compiler. This repository contains the following folders:
* code - where the code related to the compiler is stored
* documentation - the documentation of the language and the compiler
* libraries - here we store external and internal libraries. These are pieces of code that can be run. They are considered independencies.
* tests - contains input and expected output files with a test_runner script. The script checks if the output is equal to the excepted output.

The makefile contains the instructions to build the project.
`make` just compiles and puts the executable in binaries/tulc.exe
`make test` runs the tests in ./tests. You should only run tests this way as the test_runner script is not suited for direct calling.
