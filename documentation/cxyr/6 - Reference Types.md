## Reference Types ##
*Description*: How are references passed around?

*Discussion*: The language ought to be quite verbose in the way that allows one
to infer what is going on without hopping around in different files. For example
, it would be comforting to know whether a function takes in a reference by
looking at its invocation.

	var 32u a = 1000;
	ClassName.doSomething(arg: ref a);

ref could be both an operator, and used in declarations, but that seems rather
ambiguous. This also requires us to consider the dereferencing, address-of, and
reference-of operator. There are some symbols that we should consider...

	var 32u a = 1000;

	$a       // Let $ return the address-of
	$$a      // Let $$ return a const-address-of, because an address of an
	         // address is nonsensical.

	@a       // Let @ reference a variable.
	@@a      // Let @@ return a const-reference, because a reference to a
	         // reference is nonsensical.

	^a       // Let ^ return the actual object pointed to. Stacking ^ makes sense and
	         // just dereferences the pointer further.

Imagine calling a function that takes a pointer to a const variable, but your
local variable is not const. You need to be able to infer from the call that
the pointer sent into the function will point to memory which will not be edited
by the function.

	var 32u a = 1000;
	doSomething(arg: $$a);
	assert(a == 1000);

We can safely say that no matter how that `doSomething` operates, it will NEVER,
ever, change the value of `a` locally. I don't like the ^ notation though. That
seems superfluous. Let's try the following.

	(:) functionName
	{
		var 32u a = 1000;
		doSomething(value: $$a);
	}

	(: cref 32u value) doSomething
	{
		if (value == 300)
		{ doSomethingElse(); }
		else
		{ doSomethingOther(); }
	}

Note that the function actually using the reference does not in any way "dereference"
it. It merely uses it. Once inside a function call though, then we need to be explicit.
This seems rather odd... why would a 'function' like + not require it, but a function
call will? It's thought that the effects of + are not having any side effects. In addition,
such functions ought to be taking in constant references. In any other case, we can
just create a function called "add" instead to make our purpose clear...

To make the semantics explicit: operators +, -, `*`, /, %,... shall take in constant
references.

Let `cref` be syntactic sugar for `const ref`. Here, taking a pointer is
automatically casted to a reference type. That seems to work for local
references as well.

	(:) functionName
	{
		var 32u a = 1000;
		var ref 32u b = $a;
	}

A reference can be considered simple syntactic sugar for `ref X` ->
`const ptr X`. Maybe cref needs to change to refc, since that's more precise.
It is sugar for: `refc X` `ref const X`. Automatically casting pointers to
references seems like a good idea. In no case does it give us any useful
information about the function if we know if it holds a pointer or reference to
our variable. If it's a pointer it can potentially change the pointer value, and
that doesn't matter to us at all, since that's local inside the function. The
best thought to have is to just consider it a reference due to the semantics.

A general note as to why we need $$ and @@. One could state that the const operators
are unnecessary simply due to their semantics in the following example:

	var 32u x = 39120;
	f(x);
	f(@@x);

Here, f takes in a const reference to x. The problem with allowing `f(x)` here is
that the object may be acted upon, and actually change a global state. This is important.
By making this explicit, we can actually provide the programmer with useful information.
In many cases, overrides can be present.

*Conclusion*:
  We use:
  * $ as the address-of operator.
  * $$ as the address-of-const operator.
  * @ as the dereferencing operator.
  * @@ as the dereferencing-to-const operator.
  * A pointer is automatically casted to a reference.
