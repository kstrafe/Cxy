## Classes and Data Structures ##
*Description*: How are classes defined, described, and implemented?
*Discussion*: I'll assume you're familiar with object-oriented programming. It has been
an incredibly useful tool to abstract away functionality in objects.  A thing to notice
is how OOP (Object-Oriented Programming) is emulated in C.

    typedef struct _ClassName
    {
      float x_coord, y_coord;
    } ClassName;

    float ClassName_getLength(ClassName *class_name)
    {
      return sqrt
      (
        class_name->x_coord * class_name->x_coord
        + class_name->y_coord * class_name->y_coord
      );
    }

    // More functions...


This requires the user of the class to explicitly state which function he uses:

    ClassName name;
    ClassName_init(&name, 3.f, -5.f);  // Allocate
    printf("%f", ClassName_getLength(&name));
    ClassName_destroy(&name);  // Deallocate

While being explicit is a good thing, it is sometimes best to have a duck-typing pattern
that C++ employs in its templates:

    ClassName name(3.f, -5.f);  // Already automatically init'd
    printf("%f", name.getLength());
    // Automatically destroyed


Notice how much shorter, clearer, and safer this version is. The one thing it forgoes
is type declaration. You will not be certain what the type of an object will be deeper
inside the code:

    // Lots of code
    name.setSomething("X");
    // More code

Is this a problem? If we choose appropriate names, this problem generally resolves itself.
Personally, I tend to use plural for collection types (lists, vectors, arrays, maps,..),
and singular names for entities.

Our programming ought to be such that modules are of a medium size. At max a thousand
lines. Our functions ought to be no more than 100-200 lines verbatim.

This allows us to still understand and comprehend what the types are - and hence -
what the operations do. It also allows us to write sufficiently terse code that does
not become an annoyance. Also notice that when we have an instance, its type is always
defined from either: the entry point (function signature), the class's fields, function
-local data declarations, and last, a glo(b|c)al.  Only the glo(b|c)al is a little
tricky to find out. The other make it easy to find out which class you're working with.
Even then, we can probably insert static assertions of glo(b|c)als that do two things:
They assert that their dependency is of a specific name (a type). They document what
they need.

Let's not forget that we wanted a directory structure for the namespaces or packages
and filenames for the classes. An idea I've been playing with is to remove all class
boilerplate code. Suppose that a file contains the following code.

	private 32u counter = 0;

	public (:) countSomething(32u number_count)
	{
		counter += number_count;
	}

	public (32u counter_value:) getValue
	{
		return counter_value: counter;
	}

As one can observe, there is no syntax for declaring a class. There are only files,
and there is one class per file. Can code generators create classes?  They certainly
can, but they have to create the files in order to implement the specific class.

=== Kevin R. Stravers -- Mon 19 Oct 2015 01:54:22 AM CEST

It is appropriate to have a method of specifying constructor delegation in classes.
C++ uses a special place right after the constructor declaration and before the definition.

	ClassName(arguments)
	:
		element1(...),
		element2(...)
		...
	{
		// Definition
	}

This is something that I'd prefer to be avoided. Instead, a different, simpler method
has to be devised. Let's look at a normal constructor:

	(: this : pure) This
	{
		// Definition
	}

Now, how can the constructor be used to set certain variables? Specifically const
variables that should not be able to change in running code. How is it done? Let's
see...

	private var {
		const 32u my_value;
		Class member;
	}


	public (: this : pure) This {
		construct my_value(: 100);

		construct member(: 10, name: "ten");
	}

I think this can actually be a valid thing. Upon leaving the construct member, we
can just say that the object becomes default-constructed. I like the idea, it requires
no new special syntax like C++. It also allows you to use the variables from the constructed
elements more easily. C++ has the problem that its declaration is stiff and declarative.
You can't work with that. Construct allows you to place constructions anywhere. The
classic problem in construction is that we can't allow for multiple branches to exist
inside the constructor. It is unknown at compile time which branch is taken and hence
whether a default or non-default constructor is called. This is a dataflow analysis
problem. C++ goes around this by putting constructors in their own little section.
My biggest problem with this is that it requires the programmer to create a function
for,. say, a recalculation:

	This (int a)
	:
		someobj(a),
		someobj2(func(someobj))
	{}

	SomeObj2 func(SomeObj a)
	{
		while (a...)
		{
			...
		}
		...
		return result;
	}

I'd like it to be like the following:

	(:this, 32s a) This
	{
		construct someobj(:a);
		while (a....)
		{
			...
		}
		...
		construct someobj2(:result);
	}

That is cleaner and in many cases more neat. We can't express everything in mere expressions.
Even if we could, it's just unwieldy and impractical.

Now, of course, this requires one strict condition: construct statements can never
be placed inside branches.

This means constructs can not be iterated, if'd, while'd, whatever. That's all invalid.
This can be implemented in the semantic analyzer.

===

*Conclusion*:
Classes and their objects are defined as in C++. Their methods accessed via the instance,
and not the name of the methods (like in C).
