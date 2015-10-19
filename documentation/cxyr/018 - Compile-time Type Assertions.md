## Compile-time Type Assertions ##
*Description*: How do we know the type of an object in the code?
*Discussion*: We already know the type of any object at its
declaration point. However, it may be difficult to remember that information.

C forces the programmer to state the "class" its type during method invocation.
The reason for this is because C does not permit

	struct X obj;
	obj.someThing();

unless that someThing is a function pointer:

	struct X obj;
	obj.someThing(&obj);

If you want to pass the object, you need to put it as a parameter. This is
redundant. To get to the point, this is what many C libraries do to overcome
the fact that C is not object-oriented like C++

	struct X obj;
	X_someThing(&obj);

Every time you invoke, you know the exact function required. That's actually
very nice. It forces you to check your types. It also avoid some errors. Let's look
at how this would be done in C++

	std::string a;
	a.find("something");

Note that there is no information about a's type on the second line. This is not
really a problem. Unless the second line is actually 700 lines down. Sometimes,
when a lot of different types with similar methods are declared, it can be dazzling
to keep track of what's going on.

	std::map a;
	std::set b;
	std::string c;
	std::vector d;
	MyClass e;
	// ...
	a.find(...); b.find(...); c.find(...); d.find(...); e.find(...);

You get the idea. Now, it's useful to be able to assert the types in your algorithm.
I would go as far as to say that this can aid the programmer and allow the compiler
to perform some checks.

	std::map a;
	std::set b;
	std::string c;
	std::vector d;
	MyClass e;
	// ...
	a.std::map::find(...);
	b.std::set::find(...);
	c.std::string::find(...);
	d.std::vector::find(...);
	e.MyClass::find(...);

Note how it is immediately visible what the types are (I've not considered the template
parameters here). This will keep the information fresh inside the programmers' mind
as well as allowing the compiler to perform those awesome type checks.

I'd call what C++ does for "static ducktyping", because that's essentially what it
is.


So how is Cxy going to solve this specific problem? Should types be enforced? Let's
look at the possibilities

	sml.String a = "Type assertions are ok";
	a.sml.String.replaceSubstring(to_replace: "ok", with_string: "probable");

The version that does not use type assertions:

	sml.String a = "Type assertions are ok";
	a.replaceSubstring(to_replace: "ok", with_string: "probable");

Is this a non-issue? I'm not entirely sure. The latter does allow compile-time duck
typing. Is that at all desirable or nice? It is certainly useful in some cases
to be able to switch out the type and observe essentially the same behaviour. For
example, the sml.String type could be replaced by custom.UTFString, and still work
perfectly fine in most cases. It's just that: "in most cases". Spooky unknown effects
may come from this. In addition, a programmer reading the specific algorithm needs
to hop around to find out the types. In addition, the programmer needs to keep those
types inside his head whilst studying an algorithm. This can also be considered a
case for dynamic language-like duck typing (like in python). This language is statically
typed for safety reasons.

The idea of forcing the type to persist is mainly an assertion from the programmer.
This prevents changing the type declaration and getting weird behaviour because the
methods don't work in the way that is expected. Can we still allow a kind of duck
typing whilst forcing type assertions?

Well, the implementation could take a parameter specifying actual implementation
behaviour. In fact, this can be done using static ifs, allowing the compiler to
generate optimal code by removing useless code.

It's a nice idea, I guess... Certainly, you always know what is going on. What a
variable _is_. That's a good thing. I'm afraid that it will be abused with using
constructs:

	using sml.String as x
	var x a;
	a.x.replaceSubstring(to_replace: "ok", with_string: "probable");

The problem is that "x" does not give the coder any information. The coder will
have to find the using declaration, which is just as useful as finding the actual
var declaration! So using is out of the window.

What about going one step further? What about type-asserting method arguments and
returns?

	var sml.String a_;
	a = __myFunction(in.sml.String: a)~a.sml.String;

Actually, that gives us a tonne of information. We know the return type immediately.
Imagine the following scenario

	getBuffer()~buf

vs

	getBuffer()~buf.usr.Buffer

Holy shit! I immediately know what is returned! I don't even need to go to the getBuffer
function. This also allows one to write two cases:

	getBuffer()~buf.usr.Buffer
	getBuffer()~buf.sml.String

Providing return type overloading using the same name, instead of what the context
expects. I think this may be tremendously useful. Don't forget that the different
classes are unambiguously available in subdirectories, root, or granted. This gives
us an immediate place to look for that class. So far the idea is nice. I really like
it. It's just that duck-typing part that I kinda miss. Given a constexpr-like can
be given, it feels hacky and not clean.

First, let's write some code in Cxy that we port from the C++ SFML library.

	// This should be a fully compilable program, given the library is installed

	(:) enterProgram
	{
		var sf.RenderWindow main_window(
			vid_mode: sf.VideoMode(x_size: 800, y_size: 600),
			window_title: "My window")

		while (main_window.isOpen()~is_.1u)
		{
			var sf.Event window_event;
			while (main_window.sf.RenderWindow.pollEvent(event_.sf.Event: window_event))
			{
				if (window_event.sf.Event.getType()~type_.sf.Event == sf.Event.CLOSED)
					main_window.sf.RenderWindow.closeWindow();
			}
			main_window.sf.RenderWindow.clearWindow(with_color.sf.Color: sf.Color.BLACK);
			main_window.sf.RenderWindow.displayWindow();
		}
	}

I'm not sure about the syntax of where and how the type is specified. Will have
to think about that.
Think about type-inference.

	var x_ = sml.String;

This is the declaration form that was discarded at first. What's nice is that it
also works on non-type declarations as type names:

	var x = y.y.Obj.getString()~str.sml.String;

You can infer from the right side what the type of x will be just by looking at
the type assertion.

Okay so, there is a distinction to be made. Variables declared locally can easily
be found and haven't shown that much of a pain in other languages. What's not easy
to find out are the input or return parameter types.

You know, it's 7 AM and I've been sitting here all night thinking about this bullshit.
I've gotta tell ya, honestly, from my own experience, this isn't actually a problem.
The core of any problem is in the dependencies. In C++ there may be typedefs,
shadowing, and define shenanigans at work. That causes ambiguous names to pop up.
The reason types are hard to track is just that. They're hard to even find.
In Cxy they're easy to find. Check the function scope, is it defined somewhere? No?
Check the functions' arguments. No? Check the class declaration or the class that
is referred to.

	sml.String // Okay, inside sml/String.tul
	a // Must be locally defined somewhere in the method
	this.a // A field of the current class
	Class.a // Defined as a global/glocal inside Class (granted)

Yeah, so finding the type is going to be a non-issue. Even if it is an issue you're
probably having a function that's too big. Break it up.

*Conclusion*: This entry is rejected for being detrimental and obfuscating.


