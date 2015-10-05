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
	(32u a_, 8u b_ : C c_) functionName


Why use arrows when typing is cheaper using a colon? The function declaration will
have to start with a '('. What if we want a variable of - say - a class to be
assigned functions? Use function pointers. This means declarations starting with
`(` are static functions that are nonassignable.
Having a collected signature of functions is very clean and useful. It makes the
language terser and is also easily parsable. Not to mention more easily
understandable due to the nature of nested functions:

	(ptr (:) function_match : 32u table_entry) getFunction {}

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
