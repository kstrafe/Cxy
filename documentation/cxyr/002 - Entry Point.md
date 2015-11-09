## Entry Point ##
*Description*: Where does the program officially _start_ execution?
*Discussion*: Let's look at a few potential cases.

1. `label:`
2. Start from the beginning of a file (from the top to the bottom).
3. Use a standard function e.g.: `int main(int argc, char *argv[]){}`
4. Use a cyclic dependency graph.

The label is too assembly-esque and resembles a jump. Neither is it 'callable', only
goto-able. The start of the file is not really useful as it does not allow us
exactly differentiate function declarations from actual running code.
Case three defines a function as the start of the program.  I think
this makes people understand the code less. A cyclic dependency graph is very
versatile (computations based on what is used); however, it does not look like
something that is simple to manage. If it is complicated for a computer to
parse: it is also complicated for humans to understand and reason about it.

We add the requirement that each file can have its own enter function and the
compiler can compile with respect to running a certain enter function.
Reason: Sometimes it is useful to have 2 different enter functions so either can
be run. They can be declared in the same project without copying a lot of the
code.

*Tangent*: Function Declaration.

*Conclusion*: Entry points are defined as

	(:) enter
	{
		Code
	}
