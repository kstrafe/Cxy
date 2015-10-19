## Variable and Type Declaration ##
*Tangent*: See `Plain Old Data (POD) Types` for declaring PODs.
*Tangent*: See `Naming Scheme`.

*Description*: The style that is desired must be easy to parse. For consistency,
types must avoid `cdecl`. Type declarations declare an object of a type.

*Discussions*: Type reads from left-to-right.

1. `const int *const variable;`
2. `variable is const ptr const int`
3. `const ptr const int variable`
4. `variable: const ptr const int`
5. `variable: int = 100`

Number four and five are a little more tricky to parse. Note that `variable_` will
be an identifier. The problem with this statement is that we can allow arbitrary expressions
as statements (function calls mostly). We can not know until we parse the ":" token
that this is a declaration. This makes the grammar more error-prone. The same goes
for a human understanding. The human needs to read until he gets to the ":". Only then
will he understand that it is a declaration.

Number one and three are also interesting. I'd like to avoid the star in one because
it can be ambiguous with the multiplication operator. The last option is number three
(note that option two is removed along four and five).  This is LL(1) parsable (a predictive
parser). The FIRST(statement) is different for each production. Reads easily from left
-to-right.

=== Kevin R. Stravers -- 15/07/2015

Due to the the parser being LL(1), and expressions accepting types as their first token
, this causes a conflict. An LR(1) parser easily fixes this, but it is decided that
an LL(1) parser is to be used. An example of the ambiguity.

	a.Ab a;
	a.Ab.a();

The former is a declaration, the latter is a function call (an expression) on a function
pointer `a`. To avoid the ambiguity, we can use a tie-breaker in front
of this.

	var a.Ab a;
	expr a.Ab.a();

Either one of them, or both. Either way we want expressions to be executable in
within the function scope. It is thus rather inconvenient to have to resort to
writing `expr` all the time. In addition, the `var` syntax allows us to add the
following to our basic types.

	[3, 32u].element_count;
	(const 32u).max_value;
	ptr.max_value;
	ptr.bit_count; // Platform-specific.

This allows using these expressions as statements. Our declarations will look
like.

    var 32s x = 100, y = -400;
    var alpha.AlphaManager manager;

The nice thing about this is that we (humans) can quickly scout the code for
declarations. Especially when var is highlighted with some semi-obnoxious color.

===

=== Kevin R. Stravers -- 02/08/2015

The newly added concepts conflict somewhat with field declarations in that the first
token is the ClassName. To rememdy this, var will also be used for fields.

A simplifcation can be made. Instead of redundantly specifying var, we can allow

	var {
		MyType my_type(arg_: 10);
		32u a_number;
		sml.String uri_string;
	}

===

=== Kevin R. Stravers -- Mon 05 Oct 2015 09:19:14 AM CEST

A second simplification seems feasible:

	var 32u {
		a = 345;
		b = a * 2;
		c = b * a;
	};

The form of this statement is `var Type { assignments };`

	var sml.String {
		a = "a";
		b = a + "b";
		c = a + b * 30 + "cccc";
	};

This also works for arrays. There are no clashes with other datatypes as none contain
curly braces.

===

=== Kevin R. Stravers -- Wed 14 Oct 2015 12:15:29 AM CEST

I've been reading the python3 grammar and it's interesting. The type system is wholly
implied. It always makes me reconsider my own language. Fundamentally, a computer
can be idealized to be a bit processor. It takes some bits, and it processes them
into another set of bits. The bits may be mutated, or not. I wonder if the language
can be made into a general bit processor. Of course, it'd be beneficial to avoid any
type problems. As of now, we have a general bit declaration:

	var 32u a;

It allocates at least 32 bits for a. This is a nice thing to know. Maybe bit indexing
can be useful as well. If the memory model is defined as such that a bit array may
point to another location where a bit array starts, then you can basically build an
entire turing machine.

	var 8u a = 0b0011010;

When a is used as a pointer, it will just point to a location in memory.

	var 8u b = @a;

Using the @ as the dereference operator, 8u takes the 8 bits sequentially from the
location a was pointing to. I like this idea, but it's too impractical to have any
value in a real language. Certainly, it can be useful, but this forgoes type checking,
which is very important. I'm wondering whether the type system and syntax are sound.

Currently, functions and types are left-to-right readable entities. One thing that
comes to mind is C++'s member syntax. I find this syntax to be ugly, especially for
member function pointers. It looks like this:

	void (ClassName::*fptr)() = &ClassName::function;
	ClassName a;
	(a.*fptr)();

This is absolutely horrendous. My proposition is the following:

	var ptr (: ptr ClassName this) fptr = ClassName.function;
	var ClassName a;
	fptr(this: $a);

If you remember that 'this' is merely an alias for the above signature of ptr This
this, then it should be easy to remember the syntax. There's no need to make this
function also work like a.fptr(). That makes fptr look like a member, which it is
not.

So how sane is the type system? I just removed the requirement for 'ptr' with any
function. I think it's implied that a function's actual entity is a pointer anyways.
Talking to a friend of mine about the language, he said it would be nice if you could
make a circle with your index finger and thumb when looking at the code and say 'smooth'.
I agree with that, that's why I remove the ptr requirement. To make the language more
terse, I suppose the 'ptr' keyword can be turned into a `*` to make it more familiar
to C and C++ programmers. After all, it never interferes with multiplication or casts.

	var **32u;
	var ptr ptr 32u;

`*` look so much more terse. I wonder if another symbol can be used for the 'const'
name. Suppose % means constant.

	var &*% 32u;
	var ref ptr const 32u;

I'm a little split between the two. Let's create an insanely long type.

	var *[100, &%[10, * 32u]];
	var ptr [100, ref const [10, ptr 32u]];

What about single-statement lambdas? Those are interesting. You see, having a single
statement removes the {} from the lambda, which makes it look nice. Oh, and alias,
alias is new and basically allows you to retype any type or package. Currently only
packages can be aliased, but I think classes could be added as well, but I'm not too
sure about it. It's not a big issue, since the folder can be renamed. Okay so, I just
added support for another alias form:

	alias ClassName = Type

Really, the type can be anything. You can then use it as a class name. This is still
unambiguous since you can not have the same name as a grant.

	(:) enter {
		alias Out = sml.Out;
		ref (: String in) print = cast[(: String in)](Out.print);
		print(:"Anything");
	}

In python this would be:

	from .sml import Out

	def enter():
		Out = sml.Out
		print = Out.print
		print("Anything")

Note the cast, which is important due to print overloading.

In C++:

	int enter() {
		using namespace sml;
		void (*print)(std::string) = static_cast<std::function<void(std::string)>>(&Out.print);
		print("Anything")
	}

I think my language looks clean actually.

	(:) enter {
		alias Long = [5, ptr [8, ref const 32u]];
		var Long a, b, c;
		// ...
	}

Neat. I can't think of anything to criticize it right now :(

	(: sml.String in) print {
		hack("std::cout << in");
	}

Just added the new expression.
To top it all off: we add a builtin 'new' and 'delete' expression. Variable sized
arrays are just not a 'type', since they would depend on the runtime. The following
is used:

	new[Type](Expression)
	delete(Expression)

new returns a pointer to the type and allocates Expression units of that type.

	new[Type]

Merely allocates a single unit of the type.
delete also returns the pointer, but that is the pointer that will be invalid. Why
does it return the pointer? If you need to allocate at that location later, then this
can be useful. We need a placement new:

	new[32u](1, delete(new[32u]));

We can't guarantee that this will always allocate at that position, if it's already
taken by another resource, the call to new may fail.

But how do we add a constructor call in the new operator? We know that delete implicitly
calls the destructor, but we have no way of discerning the constructor of a new call.
In C++:

	new int(1)

Creates an integer initialized to 1. How do we do this? Maybe we need to flip and use
multiple parenthesis:

	new[Type](10)(:variable, a:a, b:b, c:c);

Creates 10 types, with the constructor given.  This implies that the returned 'thing'
is not callable. How do we solve this conundrum? I think the above is pretty clear
to be honest. What it appears to mandate is that operator () need not be overloaded,
but in fact, if you look closely, the entire expression returns ptr Type, which means
that ptr should not have a () available. It should not make sense.

	var ptr 8u a;
	a();  // Makes no sense

So we can implement it as the above, but there's a problem, how can we ensure that

	new[Type](:variable)

is a single-new constructor? As humans it's easy to see due to the : present. We know
that it's not the size of the array. The computer has a little more trouble though.
So why not make the syntax as follows:

	new[32u](1){:variable, a:a, b:b, c:c};

I know it looks freaky, but at least it will be disambiguous. Any other ways? Well
we could put it like so:

	new(1, oldptr)[32u](:variable, a:a, b:b, c:c);

But then the new[] doesn't look like a templated type. I like the idea of looking
at least somewhat like other instantiations. It would be preferable to be able to
remove the entire () section if no ctor or size is given.

	new[32u]

That's just beautiful. So how do we solve it? Let's just exploit the curly braces.

===

=== Kevin R. Stravers -- Thu 15 Oct 2015 02:03:30 AM CEST

An important aspect is to be able to get the results from procedure calls. Preferably
not by calling them and extracting all arguments manually.

	var 32u a: a, b: b = myCall();

This is useful, however, the grammar is currently constructed such that:

	// Multiple declarations
	var { 32u a, b, c; Type d(:); }
	// Single declaration
	var Type e;
	// Multiple decls of the same type
	var Type f, g, h;

So what other options are there? Preferably, this needs to work well together with
functions. Note that as expressions, we allow the brackets to represent anonymous
arrays.

	(32u out:32u {x, y, z}) f {
		return :x + y + z;
	}

	(32u {a, b, c}:) g {
		return a: 1, b: 2, c: 3;
	}

	f({x: a, y: b, z: c}: g())

var {} is currently, used to initialize multiple variables... maybe something else
could be used.

This is currently pretty much the only thing left for the grammar to specify. It's
interesting... So what can be done? What can we do to salvage the compound 'stuff'
like this?

	var [  // Stop right there

Is not usable, [ starts a type. In function calls [ denotes an array expression.

	var {  // Works,.. but what if we use another rule.

Suppose we allow a grammar that has var declarations in this manner, but also allows
an optional assignment to the entire sequence.

	var { 32u a: a; Type b: b; } = f();

This might well work, it's just a little uncomfortable. Oh well. Should be good.

===

*Conclusion*:
	var Type Identifier OptionalInitialization;

examples:

	var ptr 32u value = new 32u(100);
	var const ptr 32u value(new 32u(100));
	var 82s value(123456789123456789123456789);
