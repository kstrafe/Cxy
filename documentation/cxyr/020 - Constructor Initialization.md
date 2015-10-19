## Constructor Initialization ##
*Description*: The constructor needs to initialize variables. Some of the class's variables may
be const. This does not allow any modification. How is this solved?
*Discussion*: Some languages decided to remove const (java), others use a separate
mechanism for invoking variables

	// C++
	ClassName::ClassName()
	:
		member1(),
		member2()
	{
		// code
	}

I think C++'s way of doing it is ugly. It works very well, but it's just too ugly.
You know what? Why not go a little flexible here? Why not state that the constructor
has a special privilege of being able to assign to const fields? The problem is that
composing classes may invoke the standard constructor. Then, a new entity is assigned.
This is rather useless. It wastes resources and is ugly. Here is an example.

	const cl.MyClass my_class;

	(: this ) CurrentClass
	{
		this.my_class = cl.MyClass(arg_: 300);
	}

This causes two instantiations of the object my_class. That's raher annoying. How
is this avoided? As stated before, the C++-like syntax is to be avoided due to cleanliness
reasons. I don't want to introduce an "init" keyword just to initialize a field.
The above example can easily be optimized, but I suppose it feels hacky to anyone
writing this way. The nice thing about the above is that it does not require any
more language constructs.

As long as the module (class) is pure, the first initialization can be optimized
out. You know what, var declarations can be used! Since variable shadowing is not
allowed, this will just override the original declaration.

*Conclusion*: Use `var` to override constructors of fields. Allow the constructor
to ignore const fields. Normally we'd need to create a function or constructor to
construct a non-const version, return it, and set it. This just obfuscates stuff.

Example:

	var {
		32u _my_number;
		8s __max_size;
	}

	(: this ) doStuff
	{
		var {
			_my_number = 100;
			__max_size = 0;
		}
	}


