## Function Declaration ##
*Description*: Decide function declaration syntax.

*Discussion*:
There are a wide variety of different methods for defining functions. Here are a
few.

1. C-decl: `ReturnType Name ( Arguments ) OptionalArrayIndex { Statements }`
2. C-style: `ReturnType Name ( Arguments ) { Code }`
3. Java-style: `Access class Name { Access static ReturnType Name ( Arguments ) OptionalExceptions { Statements } }`
4. Haskell-style: `Name :: Type -> Type \n Name Arguments = Code`
5. Lisp-style: `(defun Name (Arguments) (Expression))`
6. Matlab-style: `function [ ReturnNames ] = FunctionName ( ArgumentList ) Code end`
7. Python-style: def Name ( ArgumentList ) Indent Code Dedent
8. Javascript-style: `function Name ( ArgumentList ) { Code }`

*Tangent*: See `Variable and Type Declaration` for choice of type within the
function signature.

I am inclined to look at a modified Lisp-style, why?
Let's look at what appears practical

	() -> () functionName       // Quite a large signature

	() functionName             // Smaller signature, can this allow multiple
	                            // returns?

	void() functionName         // Can only allow single returns, because there are
	                            // no nested returns

	(32s a, char b)(64s c) functionName  // Note: '(' in the beginning is required to give the LL(1) parser information that it is a function.

	(32s q, 8u c):(64u e) functionName               // Return args on the left.
	((:) b):(30u a) functionName                     // Returning a b void function
	(ptr (:) b):(18u a) functionName                 // Returning a b, a pointer to a void function.
	(ptr (->) b)->(32s a) functionName               // Returning a b, a pointer to a void function, using arrows
	(32u a, 8u b : C c) functionName


Why use arrows when typing is cheaper using a colon? The function declaration will
have to start with a '('. What if we want a variable of - say - a class to be
assigned functions? Use function pointers. This means declarations starting with
`(` are static functions that are nonassignable.
Having a collected signature of functions is very clean and useful. It makes the
language terser and is also easily parsable. Not to mention more easily
understandable due to the nature of nested functions:

	(ptr (:) function_match : 32u table_entry) getFunction {}

=== Kevin R. S. Sat 10 Oct 2015 10:25:28 PM CEST

An important conundrum has arrived. The first unnamed variable of the input can cause
an ambiguous grammar to be created. This grammar comes from the fact that a parameter
list can contain arbitrary expressions. So we can have an expression on the left size, a colon, and an expression on the right side.

	call(a + b : c) // Does not make sense

This does not make any sense. How do we still allow a single unnamed parameter to
be given as an argument in it's parameter list? We could invert the order:

	call(c + a : b) // Makes sense

This allows the grammar to be of the type `EXPRESSION OPTIONAL_NAMED`. This is exactly
what we want. An LL(1) grammar. The question is: is this desirable? First of all,
it appears quite readable. However, it also gets unwieldy to find the name if the
expression is sufficiently long.

	fun(x*x*t.getNumber()+x*x:x,x*x*x+3/5/a^b:y);

Using basic spacing should clear this up. There's something that I don't particularly
like about this form. The idea was for the names to come before the expressions. Something
that does not appear to be possible simply because we want a single anonymous name.
Actually, it can be considered as a comment that actually means something.

	func(700 : number_of_items);

That's actually really neat and useful. Yeah. Let's make it this way.

Addendum:
I have discovered that it's not really that neat. It requires a grammar change that
I don't seem to be able to like. The worst case for the current grammar is to force
the following:

	var float height = sin(:pi);

Where no name in front of the : designates the standard 'in' argument.

	integrateNumericallySimpsons(
		: lambda [](double out : double in){ Math.exp(-in*in); },
		elements: 800,
		from: -1,
		to: 1
	);

===

=== Kevin R. Stravers -- Wed 14 Oct 2015 06:57:43 AM CEST

The problem with the current function declaration is that it may clash in meaning.
Certainly the type declaration is useful and correct, but suppose we have the following.

	var (:) a;
	static (:) b;
	(:) c {};

What is the semantic difference? We know that a is a variable, it is a function pointer.
b is also a function pointer, but its lifetime is of the class itself.
c is an actual method. It seems as if the method is a special case. This requires
some investigation. Where does this clash? Let's look at the current grant list:

	grant Aa {
		(:) a;
		(:) b;
		(:) c;
	}

There is no way to distinguish the actualy localities. In this case, we should be
able to specify whether something granted is a local object-wide variable, a method,
or a static element. Therefore: the same syntax needs to be applied to grants.

Is the syntax for methods really desirable? It at least looks simple enough to new
users to familiarize themselves with it quickly, instead of bombarding them with:

	public method (:) c {}

We can say that public and method are implicit if not otherwise specified. This means
method needs to be a keyword. Honestly, I see no need for that. I can't think of any
case where the 'method' syntax would disambiguate anything.

	grant Aa {
		var (:) a;
		static (:) b;
		(:) c;
	}

===

*Conclusion*: Function signatures are to be written in the following form.

	( Type Name, ArgumentList : ArgumentList ) FunctionName
	{
		Code
	}

An example of this.

	(64u factorial, 32s error_code : 32u to_calculate) computeRawFactorial
	{
		var 64u factorial(value: to_calculate);
		to_calculate -= 1;
		while (to_calculate > 1)
		{
			factorial *= to_calculate;
			to_calculate -= 1;
		}
		return factorial: factorial, error_code: 0;
	}
