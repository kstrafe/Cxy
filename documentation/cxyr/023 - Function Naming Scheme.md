## Function Naming Scheme ##
*Description*: How are functions and methods to be named?
*Discussion*: Currently, functions are named with a forced middle big case character.
An example: `printLine`.

I find this to be a little ugly, actually. Considering that we can have pointers to
functions being called as well:

	var ptr (:) my_func = $someFunction;
	my_func();  // Should work

The problem is that there are now two functions, one with a variable name, that acts
exactly like a function! I'd say that it's probably not that useful to force function
names within this context.

This also causes another question to come up: how do we overload? Well, we can use the
C++ method of overloading function pointer fetching:

	var ptr (32u something : ) f = cast(ptr (32u something : )) { $someFunction };

This causes another question to arise: what to do about this immediate redundancy?
Certainly, the double declarations are quite useless here... What I'm referring to
are the double `ptr (32u something :)` parts.

In fact, that reminds me of a potentially useful shortening.

	// expression
	f~string == f()~string

This avoids having to write () if the function returns at least something...
Can it also be used if the function returns nothing? That would be interesting...
Maybe g~ can be used to indicate that the function does not return anything, since
we're not getting a variable. The problem with this is that `g~ a` can be seen
as if a is the return parameter. Even if there is a space.

	g~;
	g~a;
	g()~a;

Where can void functions occur? Well, they can certainly not be in expressions.
The question is whether there is any ambiguity at all. There does not appear to
be one. In C and C++, the ~ is the bitwise NOT operator. That is a useful operator,
so it need a replacement operator in Cxy.

*Conclusion*: Use any function name, as long as it does not collide with class
names.


