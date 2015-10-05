## Code Generators Revisited ##
*Description*: Can we use code generators to implement a simple language?
*Discussion*: There is an idea that lingered on my mind since I started thinking more about code generation. After all, that's what compilers and interpreters basically do.
They take our simple-to-read/edit code and turn it into a tonne of non-simple-to-read/edit code.
This really is at the core of what a programming language is. I wonder if we can
make the entire language a code generator by making all actions actual code generators.
My first idea is to use the # operator to designate a code generator. Here is an
example:

	#var `32u a; 64u b; sml.String c;'

Note how we use m4's string notation, starting with a backtick and ending in a normal '.
The `var` code generator is a builtin one. This allows us to avoid all future variable/keyword collisions.
Especially if we state that code generators created by the user must start with a capital letter.

Let's continue looking at some code:

	#var `32u a; 64u b; sml.String c'
	#inc `a'
	#call `c = `Hello World''
	#call `System.out.print(c)'

This becomes interesting as we expand the code to a function:

	#function `(32u upper, 32u lower : 64u in) enter
		#expression `a = #upper `in''
		#expression `b = #lower `in''
	'

or even a #class! I wonder how that will work.

	#class `name=MyClass
		#function `(32u upper, 32u lower : 64u in) enter
			#expression `a = #upper `in''
			#expression `b = #lower `in''
		'
	'

One thing to like about this is the simplicity and how everything is implemented per-codegenerator.
The thing to dislike about this is that there's too much noise from code generation.

Too much of anything is per definition bad. I really like the idea of code generation,
but this is probably going too far. What can be done to make it good? To make it beautiful.

*Conclusion*: Rejected


