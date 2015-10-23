# Demeter's Second Law #
Demeter's Second Law is an evolution of the original `Law of Demeter`. For reference,
the Law of Demeter (LoD) is given here:

* Your method can call other methods in its class directly.
* Your method can call methods on its own fields directly (but not on the fields' fields).
* When your method takes parameters, your method can call methods on those parameters directly.
* When your method creates local objects, that method can call methods on the local objects.

This is not completely practical and ideal. To improve this system, transform the law
slightly:

* You can invoke all methods on all types that are visible to you.
* An accessible type is a class in a subdirectory of the current directory, and the
subdirectories of the root directory.
* Transitive subdirectories are not accessible.

## Examples ##
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
Practicality and theoretical safety are the main focus of Demeter's Second Law.
