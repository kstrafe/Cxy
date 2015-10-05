## Solidifying Grants and other Grammatical Entities ##
*Description*: We need to set an example for the full grammar in its current form.
*Discussion*: The granted syntax is interesting. I wish to make the syntax generally
very easy and intuitive. Let's give it a shot...

	granted {
		A has {
			public static var 32u public_named_variable;

			(32u squared_value : this) computeSquare;
			(: this, value) setValue;
		}
	}

Ok, so granted contains a list of granted classes. This is similar to a template
in C++. How do we grant classes? Suppose the above class is named B.

	var B[A: String] object;

This seems unambiguous. '[' and ']' are grouper characters. Having a ClassName[X: Y,...]
is something that can never be confused with an array... or can it?

	var [10, B[A: sml.String]] objects;

That looks acceptable. B[A: sml.String] is a type in itself, and hence [10, B[A: String]]
is also a type. It's almost as if a non-prefixed '[]' means "Array". I'm a little uncertain
actually. It seems that it's mixing too much. The main question is how to grant classes.

	var B<A: String>

is possible, and resembles C++ templates, but then <> need to be reinstated as potential
groupers. Unambiguously, B() is a constructor call on a class. B[] is not anything.

	var [10, 32u] x;
	x[3] = 30102;

	var sml.String[Atom: 8u] y = "derp";
	y[3] = 'c'; // "derc"

In both cases, we can easily discern what is a granted type and what's an array or
index type. Another method is to create a builtin Array type, and write:

	Array(10, Type: Array(5, Type: 32u)) x;

But that appears too verbose. Regarding that, I can't seem to remember what __ meant
public, restricted, and private. Maybe it's better to spell it out...

	(32u : private static) name;
	(32u name : : private static) computeId { return 5; }

Egh, always trying to generalize too much. It's probably fine the way the grammar
is.

	granted

		A has { var { 32u a; } };
		B has {};
		C has {};
		...

	variables

		var {
			32u a;
			64s b;
			...
		};

	methods

		(32u stdout : 32u stdin : pure static) computeFactorial
		{
			stdout = stdin;
			--stdin;
			whilte (stdin > 1)
				stdout *= stdin;
		}

My problem with this is that it doesn't look very clean. For example, the variables
part has the same signature kind as the methods part, but looks vastly different.

	granted

		A has {
			(32u a : public static);
		};

	variables

		(32u a : public static);
		(64s b : public static);

	methods

		(32u stdout : 32u stdin : pure static) computeFactorial
		{
			(32u local_var:) = 100;
			stdout = stdin;
			--stdin;
			whilte (stdin > 1)
				stdout *= stdin;
		}

There certainly is an ugliness to that local variable definition in the factorial
function. Let's try and remove it...

	granted

		A has {
			(32u a : public static);
		};

	variables

		(32u a : public static);
		(64s b : public static);

	methods

		(32u stdout : 32u stdin : pure static) computeFactorial
		{
			var 32u local_variable = 100;
			stdout = stdin;
			--stdin;
			whilte (stdin > 1)
				stdout *= stdin;
		}

The idea with var is to be an LL(1) token in a local declaration. Instead of using var,
we can use an access specifier to the variable. Should this work on functions as well?
Suppose you have the following class.

	local private static Aa a;
	local private static Ba b;
	local private static Ca c;

	(34s weird_return : 1u stdin) yesOrNo
	{
		local 34s a = 3129;

		switch (stdin)
		{
			case true: weird_return = 31293;
			case false: weird_return = -312837;
		}
	}

There's an intricate difference between a method and a variable here. Does a simple
variable have any property of public, private, or static? A static is essentially a
global. Can a function be global as well? Going back to the original meaning of var
seems to be the safest way.

	(290u out : 5u in) computeFactorial
	{
		var 290u temporary  = cast[type(out)](in);
		--in;
		while (in > 1) {
			temporary *= cast[type(out)](in);
			--in;
		}
		out = temporary;
	}

A question comes up with this. It basically assigns () within a var statement as the
signature of a function. This is perfectly acceptable, however; should access specifiers
be added to that signature? Honestly I think it's rather irrelevant. This is my initial
reason for using the __ and _ suffix/prefix notation. What can be done is to set the
visibility of the name right before the actual name.

	glocal private 32u x;

	local private (290u out : 5u in) computeFactorial
	{
		var 290u temporary  = cast[type(out)](in);
		--in;
		while (in > 1) {
			temporary *= cast[type(out)](in);
			--in;
		}
		out = temporary;
	}

This also reminds me of the ability to avoid putting variables and methods in a single
particular order. The thing with codegenerators is that if they generate both code
for variables and methods, and you have multiple CG invocations of those in one file,
then it will be impossible to compile correctly. So that has to be corrected.

I like the idea of prefixing. It keeps the function signature clean. Of course stuff
like purity and const correctness are part of the signature.

Does this imply some lexer changes? Also, what about the casting operator or type
operator. Should they not be in uppercase to avoid the ambiguity of the array access
operator? Should an entirely new operator be applied? From C++ we learn about decltype:

	int main(int argc, char *argv[])
	{ decltype(argc) copy = argc; }

The following is a translation to Cxy:

	(8u return : 32u argc, ptr ptr 8u argv) enter
	{ var type(argc) copy = argc; }

That really seems to seal the deal. By allowing only a single name inside the type
expression, we can easily make this a grammar rule. Anywhere a type can be placed,
a `type(x)` could be placed. What about expressions? type(x.getString()~out) could
be interesting, but we get to what is known as type deduction. I think this is dangerous.
To quote the java docs:

	Humans benefit from the redundancy of the type declaration in two ways. First, the redundant type serves as valuable documentation - readers do not have to search for the declaration of getMap() to find out what type it returns. Second, the redundancy allows the programmer to declare the intended type, and thereby benefit from a cross check performed by the compiler.

This forces the programmer to always assign the returned variable to somethnig locally:

	sml.String x = y.getString();
	var type(x) z = x + " hello";

I like this idea. It allows types to be somewhat transparent, but not fully. It also
keeps everything readable. Not allowing expressions, but only simple names seems to
be the way to go!

Good, that's in order. What's next? Ah, casting! Let's give it a try. It'd rather
be consistent...

	var 43u a = 8796093022207;
	var 32u b = cast[type(b)](a);
	var 32u b = cast(type(b), a);

	var type[a] copy = a;

Hmm,... type[] seems better than (). Considering that appending it to a keyword is
something that is evaluated during compile-time, whereas () is during run-time.

	(::) enter
	{
		var 32u x = -1;
		var 64u y = cast[type[x]](x);
	}

*Conclusion*: Grants are given by the following grammar:

	Grant ::= grant Identifier has \{ {Signature name;} \}

Their semantics allow you to assert incoming types granted by a parent during compile-time.
To grant, the type is written with the grant names in brackets, directly after the
type:

	Type[a: OtherType, b: OtherB,..]


