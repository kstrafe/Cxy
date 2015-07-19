## Unnamed Language ##

[![Join the chat at https://gitter.im/BourgondAries/Unnamed-Language](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/BourgondAries/Unnamed-Language?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# The Best. Language. Ever.
>We want to make _the best_ language, ever.  
The cleanest. The nicest. The fastest. The most consistent. Language. Ever.  
We want raw control with assembly, and ease of coding with scripting.  
We want EXTREME power... with EXTREME templates.  
This language will be. The best. When it is done, all other languages will disappear.  
It is: The Final Programming Language

## Why another language?

Because the languages I've seen don't offer me what I want. I want control. I want blazing speed.
I want to be able to abstract away boilerplate code nicely. I want a single simple build system,
I want dependency tracking. Functional and modular purity. DETERMINISTIC DESTRUCTION!
Strict enforcement of dependencies!
This is what makes the _cleanest_ language ever. So clean that ANYONE can EASILY join a
totally new project with ease!

## Have *AWESOME* ideas? Want to contribute?

Send a pull request or send me an email at macocio [at] gmail [dot] com.  
I'm also available on Skype as 'macocio'.  

# How this project is organized

The repository for The Unnamed Language compiler. This repository contains the following folders:
* code - where the code related to the compiler is stored. Tests are also stored here and must start with "test" in their filename.
* codegenerators - code that generates code. Useful for generating switch-to-string code.
* documentation - the documentation of the language and the compiler
* libraries - here we store external and internal libraries. These are pieces of code that can be run. They are considered independencies.
* tests - contains input and expected output files with a test_runner script. The script checks if the output is equal to the excepted output. It also runs unit tests.

The makefile contains the instructions to build the project.
`make` just compiles and puts the executable in binaries/tulc.exe
`make tdd` runs the tests in ./tests. You should only run tests this way as the test_runner script is not suited for direct calling.

## Clear Overview
The UL dependency rule is enforced in this compiler. Modules (.cpp and their
corresponding .hpp files) can ONLY depend/include on:

1. A file in a subdirectory of their current directory.
2. A file in a subdirectory directly from root.
3. Be provided information from a parent.

This means that dependencies are kept clean and local. Spaghetti is avoided.

## Current status!
The BEST language ever uses simple, consistent lexing and parsing.  
Hand-written of course for crisp minimalism!  
Visual tools:
What on earth DOES IT MEAN?!
The green nodes are verified to be completely PURE subtrees.  
Blue nodes are part of a pure subtree, and can later be verified or
cleaned to be pure.  
Red nodes are not pure.  
Purity primer:  
```
functional purity: same input into function -> same output. Always  
modular purity: object creation with same operations -> same object
```
We can only guarantee purity if the object never uses a global.  
![Dependency Graph](http://puu.sh/j53Oe/f02ecda62c.png)
