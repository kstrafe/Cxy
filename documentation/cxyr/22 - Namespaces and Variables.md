## Namespaces and Variables ##
*Description*: How are namespaces (packages) and variables separated?
*Discussion*: This has always been an irk to me. Forcing the _ on variables
just seems so scary. I've never really user that a lot. Although I think
being clear in code, I think that maybe that is going too far.

In any case, how can we distinguish between a package and a variable without
distinguishing between their lexemes?

lexeme.Lexeme will always make lexeme a package, since we can not access
a class of a variable.
lexeme.something will always make lexeme a variable, since we can not access
a field of a package.
lexeme.doSomething(), will also make lexeme a variable.

Is there an ambiguous case? Function arguments will always be variables, and
so we're left with variable declarations. In that case, it will always be a
package name. There is, however, one ambiguity..

	var sml.String a = sml.getString()~string;

The question is: is "sml" after the '=' a package or a variable? Oh I guess I
tricked myself! that sml is obviously a variable since the subsequent token is
a method! Hmm... this seems to be a good case for allowing non _ variable names.

Can we dynamically assign stuff or in any way or shape _use_ a package? As far
as I know, a package can only be used as a prefix to a class. So in essense,
we can make x.Yname a complete lexeme, instead of having three different lexemes...

This change makes functions a LOT cleaner imho:

	(:) enterProgram
	{
		sys.Out.printLine(string: "Hello world!");
	}

instead of:

	(:) enterProgram
	{
		sys.Out.printLine(input_string: "Hello world!");
	}

In any case, the name is unambiguous. I also suspect that any input arguments can
easily be looked up or remembered, such that it is up to the API provider to
name the variables correctly, without forcing underscores. In fact, enough information
comes from "printLine" and "string" to deduce that "We print this string as a line".

Let's look at a few examples of the code getting cleaner:

	(32u out : 32u in : pure) computeFibonacci
	{
		return out: if (in <= 1) in else computeFibonacci(in: in - 1)~out + computeFibonacci(in: in - 2)~out;
	}

	(300u out : 5u in : pure) computeFactorial
	{
		out = in;
		--in;
		while (in > 1)
			out *= in;
	}

	(sml.String canonical_path : sml.String path : pure) foldRelativePaths
	{
		// Imagine we get the path "../abc/../qwerty/xyz"
		// The second comment contains the state of the special-case path "../../"
		var ref sml.String cap = $canonical_path;
		var 64u start_at = 1;
		loop
		{
			// Let '|' denote from, ".|./abc/../qwerty/xyz"
			// ".|./../"
			var 64u index: index, 1u found: found = cap.findString(from: start_at, substring: "../");

			// We have the index as follows, "../abc/|../qwerty/xyz"
			// "../|../"
			if (found)
			{
				// We search backwards from, "../abc|/../qwerty/xyz"
				// "..|/../"
				var 64u parent_index, 1u parent_found = cap.findStringReverse(from: index - 1, character: '/');

				if (parent_found == false)
					// "|../../", we haven't found a '/', so we a set the start_at to "../.|./"
					start_at = index + 1;
				else
					// Now the parent_index is given as "..|/abc|/qwerty/xyz", together with index - 1
					cap.eraseIndex(from: parent_index, to: index - 1)
			}
			else
				break;
		}
	}

	// Without comments
	(sml.String canonical_path : sml.String path : pure) foldRelativePaths
	{
		var ref sml.String cap = $canonical_path;
		var 64u start_at = 1;
		loop
		{
			var 64u index: index, 1u found: found = cap.findString(from: start_at, substring: "../");
			if (found)
			{
				var 64u parent_index, 1u parent_found = cap.findStringReverse(from: index - 1, character: '/');
				if (parent_found == false)
					start_at = index + 1;
				else
					cap.eraseIndex(from: parent_index, to: index - 1)
			}
			else
				break;
		}
	}

Let's also allow automatic up-casting (from less information to more). Any non-existing information
can be 0 bits.

I really like the factorial code above. Notice there is no return variable? Yeah... that's planned.

So whilst writing some code, I also got the idea of adding the "ifs" statement. It's really simple,
but allows any expression to be added. It's just like a case, but does not use constant
expressions. Hmm, why not then just allow a case with any expression?

	switch (something)
	{
		case a + b * c: statements;
		case u - d: statements;
		...
	}

It's an interesting idea. It's shorthand for

	if (something == a + b * c)
		statements;
	else if (something == u - d)
		statements;

That's a lot less typing and redundancy. I like it. In addition, if there are only constant
values in the switch, we can optimize it to be a true switch... I like the idea. It makes
coding a lot less tedious.

I'm digressing.

== Fri 14 Aug 2015 03:47:34 AM CEST - Kevin Robert Stravers
One thing that may be important to distinguish are enumeration constants. We know
that the compiler changes these into numbers during compile time; however, are they
fundamentally different from variables at all?

For now, the following rules are established:

	Class names: [A-Z][A-Za-z0-9]*
	Variables: [a-z][a-z0-9]
	Enumerations: [A-Z]+

What if enumerations are implicit inside the class? I'll provide an example.

	// MyClass.ul
	var 32u SOME_NUMBER, ANOTHER, NUMBER, DECLARED, HERE;
	var sml.String my_string;
	var 8u MY, NICE, ENUMS;

Now the `my_string` and other declarations should not affect the enumeration in the
slightest. In fact, the enumeration will continue... or should the enumeration be
limited to a single number that limits the enumerations? That's probably better...

	// MyClass.ul
	var 32u SOME_NUMBER, ANOTHER, NUMBER, DECLARED, HERE, MY, NICE, ENUMS;
	var sml.String my_string;

We can then align the enums to look like an enum declaration in other languages:

	// MyClass.ul
	var 32u
		SOME_NUMBER,
		ANOTHER,
		NUMBER,
		DECLARED,
		HERE,
		MY,
		NICE,
		ENUMS;

The type of the enum is dependent on class here. How can we assure that an enum is
correctly given to methods in their signatures? We could use 32u, but that would
forgo type checking. What can be done is the following:

	// MyClass.ul
	var 32u
		SOME_NUMBER;

	(: MyClass.32u enum_variable) doSomething
	{
		doSomething(enum_variable: SOME_NUMBER);
	}

This could work, but only if we allow single enums per class... What about constants?
It would be nice to have constants be recognizable by some upper case or special
characterization, but it's probably not that important.

One problem with this enum is that it uses 32 bits,... Maybe this is a good thing.
It allows the user to tweak the type manually. What if we want to use this type,
but we want to make it a different size on different platforms? Suppose some platforms
just are more efficient processing a 16 bit value, and others more efficient with
32 bit values? What about signedness? For this, a code generator can be used to
substitute the type...

	// MyClass.ul
	var #EnumSize~
		// Many more enums here
		SOME_NUMBER;

	(: MyClass.#EnumSize~ enum_variable) doSomething
	{
		doSomething(enum_variable: SOME_NUMBER);
	}

I really seem to enjoy this. For what reason? Well it's flexible, you know where
EnumSize is located. In addition, wherever you pepper it in, it can easily be changed.

	// EnumSize.ul
	( sml.String ~ :)
	{
		return ~: "32u";
	}

That reminds me, the 'standard' return parameter ~, without a name. In fact, that
is the name. It's a standard name. I like the idea. It also requires a standard input
though. Why? Because it's cleaner. We don't need to guess simple functions' argument
names. Functions such as computeFactorial, or computeFibonacci, or cos, sin, tan,..

Imagine:

	cos(0.3)~

Should this extract the standard argument, or just

	cos(0.3)

?
The latter looks cleaner. But it may confuse us about void returning functions. The
latter is more consistent when it comes to "not naming stuff". We can allow a single
unnamed parameter and return that we have to call ~. What if we don't call it anything?

	( sml.String : sml.String ) normalizeString
	{
		// ... do some string operations
		return :

Ah, see the problem? There's no way to reference the input parameter! We can use a
standard name instead, but would not that muddy the language? Calling it a symbol
like ~ seems to be more productive. This way, the function implementation can actually
access the in and out variables.

Just had a shower thought.. the issue with ~ is that the current lexer doesn't really
enjoy it, neither does it allow a clear separation between input and output. Instead,
we can opt for something that's easier to recognize as well as more consistent:

	(128u out : 32u in) fib
	{
		//...
	}

	(:) enter
	{
		sys.Out.print(fib(50));
	}

This solves the previous conundrum of allowing a single unnamed parameter! It still
forces all parameters to be named, but allows a single unnamed parameter for convenience.
This unnamed parameter binds to in as an argument, and out on the output argument!

==

*Conclusion*: Allow variables to have the same name form as packages.


