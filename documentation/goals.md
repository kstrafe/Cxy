## Goals of Cxy ##
* General purpose programming language
* Comparatively quick compile-time and running speeds
	* Clean LL(1) grammar
	* Deterministic Memory Management
	* Zero-cost abstractions
	* Optional run-time bound checking etc.
	* Easy parallelization
	* Compile-time runnable code
* Minimal Boilerplate
	* Compiler `classpath` similar to Java
	* Using classes as filenames
	* Using directories as namespaces
* Guarantee non-machine specific algorithms to work any machine with sufficient memory
	* Plain Old Data (POD) types must work on any machine
	* Objects using POD types must work on any machine
	* Optional 'safe' tag to denote defined behavior
* Self-documenting code
	* Const correctness
	* Optional Functional Purity
	* Forced named parameter passing
	* Names part of function signatures
	* Strong, static typing
	* No ternary operator: if-else can be inside an expression too.
* Ease of coding
	* Nested block comments
	* `Conservative` Operator Overloading (Can not define new operators. Same operator precedence as for arithmetic.)
	* Function overloading
	* Template Metaprogramming
	* Mass Array Operations ([32s] + 32s will add the right argument to each element in the array, creating and returning a new array)
	* Aspect-Oriented Programming
* Obvious Behavior
	* No infix/postfix ++ and -- expressions (due to uncertainty of order of evaluation)
	* No boolean short-circuiting
	* Strict evaluation
	* Encapsulation to prevent unwanted code tinkering
	* No inheritance, only pseudo-inheritance

# Demeter's Second Laws #
Demeter's Second Laws are an evolution of the original `Laws of Demeter`:

	* Your method can call other methods in its class directly.
	* Your method can call methods on its own fields directly (but not on the fields' fields).
	* When your method takes parameters, your method can call methods on those parameters directly.
	* When your method creates local objects, that method can call methods on the local objects.

This is not completely practical and ideal. To improve this system, we edit the laws
slightly:

	* You can invoke all methods on all types that are accessible to you.
	* An accessible type is a class in a subdirectory of the current directory, and the
	subdirectories of the root directory.
	* Transitive subdirectories are not accessible.

Here is a file hierarchy:

	root/
		Main.cxy  - Has access to a/, b/, but not ab/ and bb/
		a/
			Aa.cxy  - Has access to a/, b/, and ab/, but not bb/
			Ab.cxy
			ab/
				Aba.cxy  - Access to a/, and b/
				Abb.cxy
		b/
			Ba.cxy  - Access to a/, b/, and bb/
			Bb.cxy
			bb/
				Bba.cxy  - Access to a/, b/, but not bb/, and not ab/
				Bbb.cxy

This allows global types to exist and be used. This allows global variables as well.
Practicality and theoretical safety are the main focus of Demeter's Second Laws.
