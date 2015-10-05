## Lambda ##
*Description*: Introduce the popular lambda functionality
*Discussion*: Lambdas are incredibly useful to the programmer, hence their popularity.
A lambda is a simple, inline function that can be assigned to a variable or used as
an argument to a function. The current grammar does not appear to support this feature.
Let's explore this. Suppose we want to send a function as an argument.

	(: (32u anything:) in :) fun
	{
	}

	(::) enter
	{
		fun((32u anything:) { return 329; });
	}

The problem here is that it's impossible to discern the () from an expression or a
function definition in the call to function. This requires a small change to the grammar.

To support lamdas, all that seems to be necessary is to add the 'lambda' keyword. The
grammar for a lambda expression:

	Lambda ::= lambda '[' ArgumentList  ']' Signature '{' Code '}'

The reason for making it an expression is because it makes no sense to be used anywhere
but an expression.

*Conclusion*: Lambdas are added as an expression using the lambda keyword.

	Lambda ::= lambda '[' ArgumentList  ']' Signature '{' Code '}'
