## Hack ##
*Description*: How do we allow platform-specific and low level access in a
general manner?

*Discussion*: GCC's C and C++ compiler supports the asm statement. This statement
allows you to put assembly in the C/C++ code. The syntax supports getting pointers
and other data by setting registers.

This is very useful for low level control. Sometimes the compiler for a machine
has no support for generating specific CPU assembly code. It can be useful to
allow the user to write this code himself.

The current compiler reference implementation (Cxy CRI) compiles to C++ (that's the
plan). It can be tremendously useful to implement a print function by redirecting
to std::cout. There are multiple options. One is to make the standard library
special. The compiler generates special code for that library module.
I think it is better to avoid that, and instead use the following:

	public (: sml.String to_print) printString
	{
		hack
		(
			var_: "to_print",
			cpp_: "std::cout << {0};"
		);
	}

Note how this allows to bind to platform-specific C++ elements very easily. This
requires some hacking on the object level of sml.String. The string is luckily not
garbage collected and the data model is as simple as the C++ version.

The hack statement combines well with the static if, allowing the compiler to
maybe generate C code, or even assembly for a specific processor. Maybe a compiler
compiles to Python and uses hack to generate python code. Some compilers may
generate Java, or ARM, or x86, or,... The list is virtually endless, with hack,
we allow endless possibilities.

*Conclusion*: Use the hack statement as a compiler-specific code generator.


