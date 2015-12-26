## Musings On Variables ##

*Description*: Question the fundamentals of the declaration of variables and assignments.

*Discussion*: The common way to assign some object to a variable is to state it like
so:

	a = b;

The problem with this becomes apparent when assigning to multiple variables using
function return values.

	a: a, b: b = f();

First of all, we can't be certain what production we're in by looking at the first
symbol. An alternative is to use

	var {32u a: a, 5u b: b} = f();

This works just fine, but it's limited to declarations only. How can this be made
truly dynamic as well as powerful?
An option is to say that: well, since expressions are always first anyways, we just
force the declarations second:

	f() => a: a, b: b;

This could work very well, in addition, it avoids the need for the var keyword, even
though it would break a lot that's already implemented.

	f() => 32u a: a, b: b;

What about just raw declarations though?

	=> 32u a;

Function arguments currently have the potential form:

	f({a: b, c: d}: h())

There's something very clean and neat about that. I wish it were possible to have
this in a normal run-scope as well. The problem is that {} clashes with the scope
operator.

	{a: b, c: d} = h();

Just like:

	var {32u a: b, 5u c: d} = h();

Maybe it's even necessary to remove var altogether and just use {} in this specific
context.

	{32u a: r, Type b: b} = f();
	{8u char};
	{a: normal, char: color, b: some} = g();

That is indeed a possibility, but is it desired? It would be really cool to see compound
assignment as it makes the language a lot cleaner. Unfortunately, I can't use python's

	a, b, c = f()

since that would violate the idea of having statements. I could have the function
in the beginning though:

	f() => a: a, b: b, c: d;

I like the idea, operator => is not yet used. Really considering this. The grammar
would then still be LL(1) without sacrificing anything important. The grammar rules
would look like

	EXPRESSION '=>' NAMELIST ';'

We gotta remember that expressions are often far longer:

	f(:g()~a, control: true, expr: SYMBOLIC_EXPRESSION) + 3
		=> 32u d;

What if this is used to express assignment? It would make the language far more elegant,...
I kinda,.. like this idea. It would also avoid the use of the 'var' statement. Although
that kinda goes against the idea of making the code readable. I guess we can always
just search for => if necessary. I like this idea a lot. It fixes the issues that we're
currently facing. Removing the var keyword and all.

	plot(:f, style: DASHED_LINES, color: [127, 0, 0]) => math.Plot g: out;

Oh wait, isn't there a problem with math.Plot there? The namespaces will collide with
the actualy variable names! This means that we don't have LL(1) parsability. To solve
this issue, let's examine how var can be used.

	var math.Plot a;  // Generate an empty plot object
	{a: out} = plot(:f, style: DASHED_LINES, color: [127, 0, 0]);  // Generate the plot

The problem is that {} is a 'code-block', unless we make those only possible inside
while/for/if,... The problem is that the following is ambiguous:

	if (a > b)
		{a: out} = f();

To rememdy this problem, we can use the => but prevent object creation therein. Let's
also look at something else.

	extract {a: out} = f();

The exception here is the expression evaluating to a single outcome:

	a = f();

My problem with this is that it is not at all elegant. It's a hacky solution. I don't
particularly enjoy it. We can also make everything an extraction. Python's way is nice
and we want explicit (named) returns for readability reasons.

	= {a: out}: f();
	var {32u a: out}: f();
	[5, 32u].size;

We can't use [] to denote an assignment list. Neither can {} or () be used. What if
the operators are reversed?

	] 8u y [;
	] 32u x: a, y: b [ = f();

	var {Type x: a, y: b; 32u z: c} = f();

An alternative is to just force the => notation, but I'm afraid that this is not how
we start writing an assignment nor think about the logic.

	f() => var 32u x: a, y: b;
	sin(0.3) => total;
	total += sin(0.3);

As can be seen, it doesn't fit that nicely. How do we solve this yet keep the grammar
LL(1)? Can the left side be made into an actual expression?

	x: a = f();
	32u y: b;
	Type z: c, w: d = g();

In the last two cases, the types notify that we're in a data declaration statement.
What is the grammar?

	EXPR [':' name] {',' EXPR ':' name} ['=' EXPR] ';'

The problem with this is that it appears to allow all kinds of weird grammars. For
example:

	a + b: c = f();

Wait, that's not weird at all, that makes perfect sense! Why not allow an assignable
expression on the left side?! Madness! So how can it be made even better? Let's try
out something:

	EXPR [EXPR] [':' name] {',' EXPR [EXPR] ':' name} ['=' EXPR] ';'

So in this case, we can think of any type expression to be an expression in itself.

	Type x: a, Type y: b = f();

A preferable situation is to allow Type to be mentioned once.

	Type {x: a, y: b} = f();

This fits very well with the currently used method for arguments in functions. The
grammar would need to look like.

	{EXPR '{' RETLIST '}'}+ ['=' EXPR] ';'

This allows:

	32u {x: a, y: b}, z: c, Type w: d = f()~g()~h()~j(: 10);

The problem with this kind of assignment statement that it becomes impossible to decide
the production if we aren't already done with most of the production. We'd need to
scan to the '=' in order to know that this is an assignment. That's the biggest issue
with this idea. We need an LR(1) parser for this. I much prefer not to.

Of course, all statements can be generalized as this. This can only be done by merging
the {} and aforementioned method into a single grammar:

	EXPR ('{' NAMELIST '}'|[EXPR] [':' name]) {',' EXPR ('{' NAMELIST '}'|[EXPR] ':' name)} ['=' EXPR] ';'

This should conclude the truly general expression. It's short, LL(1), concise, and
elegant. I like it! Is this it? Is this the end?

	a: v, b: w += f();

The compound operators also work on it. That is because the 'name' section doesn't
at all bind to the += expression. The nice thing about this grammar is that we can
simply add multiple variables in the same statement in this elegant manner. Can operators
outside the compound assignments be used here?

	32u x = 0, y = 1;
	32u {z: a, w: b} = x: a, y: b - f();  // Still uses the names 'a' and 'b'.

I guess you could say x - f()~a and y - f()~b are called, returning a new 'return set'.
I'm not so sure about this. I'm thinking "let's keep it in the realm of practicality".

	f({x: a, y: b}: x: a...

I'm starting to think that the above is just too impractical. The grammar will hardly
be remembered and it's just not easy to read. There need to be limitations. Just like
the limitation on the depth of conceptualization (1). A nice and practical depth.
This depth saves us from overly engineered complexity. Hell, if you really need an
advanced feature you can use a codegenerator, that's what this language should excell
at. The thing is though, we need named multiple returns as well as multiple arguments.
The argument problem is resolved in a very nice manner, but the variable creation
problem is not. An idea is to use 'var', but it makes the language ugly. Another is
to generalize the expression itself to support it. Instead of allowing one to write
a multitude of complexities like above, let's do something simpler:

	32u {x = 0, y = 1};
	32u {z: a, w: b} = f();
	z: a, w: b -= x, y;

Hmmm... that shouldn't work either. I could say that static methods are simply avoided
altogether, then types can not be used in expressions without some surrounding information.

	32u.size;  // No
	size[32u];  // Yes
	Type.method  // Yes

The parser breaks down at Type.method, because we don't know if the current line is
a variable declaration or part of an expression. As stated, expressions could just
be generalized and we can use that. Semantic analysis then needs to check each expression.

	f({x: a, y: b, z: c}: g(:10 + 3 * 2 - u)~h());

The above is nice, it works and it's very general. In many cases, maybe we want to
process the returning data just a little. What if c needs an added integer? I'm thinking
of adding named lists - in a sense:

	f({x: a, y: b, z: c}: {c: 1} + g(:10 + 3 * 2 - u)~h());

This matches the c value with the corresponding set returned, and adds one to it.
I think this is beautiful. Absolutely beautiful. Terse. Powerful. Fierce. The ideas
of named lists are born. They're static and at compile-time only. Nevertheless, useful.
The issue at hand is the use of {}. It's overused, and named lists just can't be used
anywhere outside function arguments because it's unknown whether either production
is to be used.

To remedy this, we can think of using the diamond <> operator instead. Maybe. Maybe,..

	Type.run() = 0;

could be used together with the general form of an expression. A full expression.
An fexpr! Finally. Am I starting to see? Am I starting to wake up?

	32u {x: a, y: c} = {out: -100} - {c: 1 + 3} * {c: 100} + {a: 200};
	sml.Out << x;

The problem with this is that it differentiates the expression on the left from the
expression on the right. I think it fits the generalized expression form though. That's
a good thing...
From there, the semantic analyzer can find out that the statement actually does. This
will not be too easy. There needs to be a fexpr in the basic grammar. fexprs can not
be nested. The fexprs are truly elegant! Such elegance!

	FEXPR ::= EXPR ('{' NAMELIST '}'|name [':' name]|) {',' EXPR ('{' NAMELIST '}'|name [':' name]|)} ['=' EXPR] ';'
	NAMELIST ::= {name [':' name|'=' EXPR]}  // Names are optional.

basically implies that a second (non-epsilon) expression becomes the new name. That's
actually very clever.

	a + b + c + d;  // Just fill in the first EXPR, leave all optionals empty.
	Type.doSomething();  // Just fill out the first expr.
	Type myname;  // Both filled out, declare a new variable.
	Type {a = x, b = y};  // Both filled out, declare.
	Type {a: x, b: y} = {x: Type(:1).get()} + f();  // Three elements filled out. Declare.
	a: x, b: y = {x: Type(:1).get()} + f();  // Two elements filled, just assign.
	32u {x: a, y = 100} = f();

This plays out very nicely. Expressions can be expressions. 32u.size can be used freely.
And above all, we avoid 'var' or 'static'. The part inside `{x: Type(:1).get()}` is
to be an atom. It must never consist of multiple variables. This way, we manage complexity.
The distinction between = and : in the {} lists is to distinguish an expression and
direct assignment. This is useful in larger expressions.

An idea that popped into my head just now is that I need to imagine a library where
a method returns some structure or class. In this case, both the callee and caller
need to have that structure as visible space:

	Main.cxy
	a/
	|-- Aa.cxy
	|-- b/
	    |-- Bb.cxy

Suppose A contains a method that returns the class Bb. How is this made visible to
Main? One method is to put it in the global scope with respect to the program itself.

	Main.cxy
	a/
	|-- Aa.cxy
	|-- Bb.cxy

Both elements are now visible since Bb is a root object. What if we have some deeper
hierarchy where we don't want that?

	Main.cxy
	a/
	|-- Aa.cxy
	|-- b/
	    |-- Bb.cxy
	    |-- c/
	        |-- Cc.cxy

Bb wants to return Cc to Aa, but it can't! Aa doesn't want to let Main know what it
uses internally, so Cc has to remain hidden to Main. How does Aa do this? We can use
concepts for this:

	Main.cxy
	a/
	|-- Aa.cxy
	|-- b/
	    |-- Bb.cxy
	    |-- Cc.cxy

With the code in Bb being:

	grant Cc {
		(:) performOperation;
		// ...
	}

Aa:

	(:) myAOperation {
		Bb[Cc: Cc] b;
		Cc c = b.getSomething();
	}

With the idea of allowing super expressions, the meaning of class names diminishes
somewhat. Maybe it isn't that useful. I'd still enforce it for the reader's sake though.
Another thing is that the language doesn't have implicit casting like Java, C, or
C++. There is no `(void *)` cast.

Regarding exceptions, let's just idealize them as long unwinding gotos, this way they
can be used for control flow without the pesky overhead. If you need error handling,
create a glocal and assign some string during runtime. The rules still stand.

	CAPITAL for constants
	UNDERSCORED_CAPITALS for anonymous enumerations
	lowercase for namespaces, variables, methods
	camelCase for methods
	PascalCase for classes

	(:) enter {
		alias {
			Key = crypto.Key;
			Encrypt = crypto.Encrypt;
		}
		while (true) {
			try {
				sml.Out << "Write your new key: " << sml.Out.FLUSH_BUFFER;
				sml.String read = sml.In.getLine();                         // Read a single line
				Key key;                                                    // Create a new key object
				key.set(read);                                              // read the key into a key object

				sml.File file("to_encrypt.txt");                            // Open the file to encrypt
				sml.String encrypted = Encrypt[Stream: sml.File].encrypt(   // Encrypt the file into a string
					key: key, stream: file);
				file.open("encrypted.txt");                                 // Open a new file, this closes the other file
				file.write(encrypted);                                      // Write the encrypted data
			}
			catch Key.NO_KEY_GIVEN
				sml.Out << "The key was empty, please try again";
			catch Key.KEY_TOO_SHORT
				sml.Out << "The key given was too short, please try again";
			catch sml.In.END_OF_FILE {
				sml.Out << "End of the stream reached, aborting";
				sml.Application.exit(1);
			}
			catch sml.File.FILE_NOT_FOUND {
				sml.Out << "The input file is not present";
				sml.Application.exit(1);
			}
			catch sml.File.NO_PERMISSION {
				sml.Out << "There are no sufficient permissions to operate on files";
				sml.Application.exit(1);
			}

			// Warnings will show up for uncaught exceptions
		}
	}

Note how I've removed the `:` from function calls with a single argument. I'm thinking
of allowing expressions in arguments to be of the form:

	ARG ::= EXPR [':' EXPR]
	ARGS ::= ARG {',' ARG} | []

This will remove the ugly `:` from calls where it is simply not needed. We can let
the semantic analyzer figure this out for us. I think in most cases, a comma is not
even necessary. I need to add some experimental features. To avoid conflicts, the
parser can just use a lookahead for now. Later on, this lookahead can be removed.
This extends the classes into expressions. Quite interesting. The catch expression
is also quite obnoxious. Scala uses the => operator, but I'd rather not. I'm thinking:

	try {
		// Code here
	} catch {
		// Common catch code
		Key.NO_KEY_GIVEN {
			sml.Out << caught.toString();
		}
		sml.File.FILE_NOT_FOUND
			sml.Out << caught.toString();
	} catch {
		// Other common catch code
		sml.File.NO_PERMISSION
			;
	}

This just has the grammatical form:

	CATCH_BLOCK ::= 'catch' '{' {ENUM BLOCK_OR_STATEMENT} '}'

This is much more elegant and allows using a single line as well. It also allows you
to put common catch code. I like the idea.

Name mangling is important. With name mangling, a lot can be achieved. The programmer
gets notified if a name mangles. That's ultimately a good thing. I like the idea of
underscores and big letters being used for enumerations, all the while enumerations
are also used to jump in code. This allows nonlocal gotos to exist. I really enjoy that.
Something I've never seen in a language is the ability to throw in such a way that
you return from multiple functions. That's extremely pleasant. I think it would be
interesting to construct a kernel language. The current problems that are still in
the language is that the grammar is too large to handle for a normal human being.
Maybe this is inevitable, but it's rather annoying. How would be construct a kernel
language?

	START ::= { FUNCTION }
	FUNCTION ::= '(' { PARAMS } ':' { PARAMS } [ ':' ATTRIBS ] ')' fname ( STAT | '{' STAT '}' )
	PARAM ::= TYPE EXPR

Gets big pretty fast. We have static and strong typing, so types will make so much
more difficult. I think the grammar is fine. A nice cleaning should make it nice. Hell,
I bet I could change most type thinys to EXPR and get away with it. Let's keep the
grammar specific though. So it's pretty clear that enums must have underscores. What
about completely CAPITAL values? We could make those 'constant', but it seems rather
boring. We can do a whole lot of different things. What about glo(b|c)als? I like
that idea. It makes it clear what is global and what isn't. Do we differentiate between
glocals and globals though? There's a big difference. We might as well write:

	32u global[name] = 300;

	(:) enter {
		global[name] += 3;
		f(global[name]);
	}

Thus making the expression 'global' part of the name. This keeps globals very clean
in our code.

	// Class Person
	(: this) constructor {
		glocal[objectCounter] += 1;
	}

	(: this) destructor {
		glocal[objectCounter] -= 1;
	}

As used in

	32u glocal[objectCounter] = 0;

	(:) overFunction {
		Person person;
		sml.Out << glocal[objectCounter];
	}

Alright, so that's the name mangling for globals. Why not expand it to enums as well?

	Classname.enum[enumname];

Then, inside the class, we write:

	enum[enumname];

And that's it. Nothing more, nothing less. Even if it's only used somewhere. I'm not
entirely sure if this is desirable. On one hand we don't particularly enjoy name mangling.
On the other hand, name mangling is useful. I just wish I could make the language
simpler.

I have just devised a solution to the whole variable problem! Instead of letting the
parser do the work, we let the lexer do the work. We exploit the fact that namespace
accesses are no more than 1 in depth. A buffer is created with three slots. If the
first slot is a namespace, the second a dot, and the third a class, then that constitutes
a single type. Another slot can be added to make sure we don't have a pure type. The
reason for this is so that member expressions aren't mistaken for types.

	my.Type.getSomething();
	my.Type element;

So the entirety of `my.Type.` is turned into a single token. The reason we need to
do so is because the parser can't look ahead to the dot. In the former case, we have
TYPEEXPR, the second being a TYPENAME. This requires another lexer stage. This will
be awesome! There are no limitations now. Static overloads of operators are not allowed,
neither are static -> operator on a type. Maybe I need my::Type just like C++, but
I'm quite unsure.

	my::Type::getSomething();
	my::Type element;

It does look quite different though. I like the quickness of the dot operator. It's
fast to write compared to ::. Actually :: isn't too bad either. I like the idea. It
disambiguates clearly, although the compiler will have no problem recognizing the
correct variable. It's more friendly to the human eye. Maybe that's just my C++ bias
showing though. I have an idea:

	my::sin(0.3)

Can be valid, we create a standard class per namespace called "construct" or something.
This class will be implied by the :: sign. This allows the wonderful syntax:

	math::Standard::cos(math::Standard::pi);
	math::cos(math::pi);

Are equivalent. The name of the class though, we need to be able to send the class
into some kind of template:

	MyType[math::Standard] mytype;

Because that's useful. We can also call it the Default class:

	Accum[math::Default] accumulator;
	mytype.cos(math::pi);

I really like this idea. It gives the :: operator purpose. It allows for smooth libraries
to exist.

	sf::drawToWindow(sf::RenderWindow...);

I really enjoy the idea. The single depth of the namespaces also mandates that this
becomes completely possible. One issue I have with this is that the default class
can't really evaluate other members of the package. By that I mean that it can only
observe modules lower than itself. Those methods should probably be implemented in
the respective classes then. Say we have a collection package:

	col::Array[32u] a(7), b(9);
	a.fill(3);
	a <> b;  // Swap operator
	sml::out << b.toString();

Instead of using col::swap or anything, we could either do a '.swap' on the objects
or just implement a swapping operator. Another thing is that now, [] can be a type,
and we don't yet know if it will be an expression or not. That's a big problem, since
arrays can be nested indefinitely.

	[5, 3, 1u]::size;  // How can we know this is an expression and not a declaration?

We can use the parser to evaluate this. If we have:

	Type name  // We have a declaration
	Type ::  // We have a member method

The problem with this is that we can't decide this before we're inside the expression.
An LR(1) parser could handle it, but not our LL(1) parser, unfortunately. How can
this be remedied? The same goes for templated types btw, it's not possible to use
a simple 3-buffer to find out if we have a declaration or not. It seems like we have
to stick to the fexpr in order to avoid var. Another question pops up: what is the
standardized way of assigning variables? Surely we can allow the following:

	32u {a: x, b: y, c = 3}, 8u d: z = f();

Or should this be split up into

	32u c = 3;
	32u {a: x, b: y}, 8u d: z = f();

The idea is to create 'assignment lists'. This means that anything of the form `x: y`
will have x be assigned to the y in the rightmost expression. To disambiguate the
above example with `z = f()`, we ... oh, it's already good.

	8u a: b, 32u c = g() = f();

This will bind the result of g to c, but f is assigned to the entire group. I think
this looks rather confusing. Maybe there's a way to avoid this entirely. We don't
want newcomers to be utterly confused by complicated grammar. We don't want to limit
a single data type per declaration, that would not make you able to extract all return
parameters in one sentence. So what else can be done? Do we just roll with it?

I've just implemented this entire scheme and it appears to work quite well. No trouble
so far. It's a slightly complicated grammar, but I think it can be understood quite
easily. So this is how we create expressions. Quite simple. For scopes I think I'll
just use `label { ... }`. Seems to be the simplest way to do it. This way we'll avoid
the problem with the FIRST set of {} on the expressions. In addition, {} needs to
be added to expressions.

Instead of thinking about specific assignments, why not create a general syntax for
expression fetching?


	v: out, e: error = sin(:0.4328);
	sml.Out << v << "with error" << e;

Should be something like:

	(sml.Out << :out << "with error" << :error;): sin(:0.4328);

This avoids temporaries. Maybe something like EXPR =>?

	sin(:0.4328), {c: 33} => sml.Out << :out << "With error: " << :error << " message: " << :c;

This can keep function return names anonymous: they're not assigned to variables.
In addition, it is easily generalized to, say, assignment statements or declarations:

	sin(:0.4328), sin(:85) => 32u {x = out[0], e = error[0], x2 = out[1]};

We can just say that the return values shadow the surrounding values.

	sin(:0.4328), sin(:85) => sml.Out << out[0] << error[0] << out[1];

Looks nice to me. I like the idea of local manipulations like this. We can then avoid
having to assign the returns. How about naming the commas? Or what about naming the
returns as collectives?

	getWindow(): x, getViewport(): y => sml.Out << x.getSize()~width / y.getSize()~width + y.getSize()~height;

As you can see here, getViewport is only called once, assigns all returns to a struct
'y' which is later used in the expression. I like the idea because it's powerful. Maybe
all expressions can be of this form...

	100: x, 32 * 10: y => 32u {x, y};
	getWindow(): x => 32u {h = x.getHeight(), w = x.getWidth()}, 32f r = x.getRatio();

	// The classical approach
	ref Window x = getWindow(); 32u h = x.getheight(), w = x.getWidth(); 32f r = x.getRatio();

What this syntax does is that it allows you to use type inference. Something that
I consider somewhat dangerous. Luckily it's only for a single statement, this limits
the damage that can be done by recklessly inferring. I think I need to stick with
the 'experimental' method. It does not allow type inference. Neither does it allow
you to recklessly get values. I think all returned items should be checked against
some type. Unless you send it to another object directly. In that case, the above
method is actually pretty good. getWindow().getHeight() is legal, why not allow it
to be easier to write? We could say we limit a single depth of the function call,
but that seems rather extreme. This would not allow joys like

	sml.Out << currentWindow->getWindow().getHeight();

That's non-sensical. I therefore think it's fair to have the inferring syntax for
a single statement. The value will not be reused or changed. Maybe multiple returns
are a mistake... Maybe it just complicates too much. We could always just accept a
tuple back. Anyways, no, type inference can go away, we _need_ explicit types badly.
They keep code correct, and prevent bugs when erroneous edits are made. We could also
go overboard (I've talked about this before) and do not allow inline expressions.
That would be incredibly interesting. While not extremely safe, it would eliminate
all kinds of type errors.

	32u a = f() + 3129/31 + sin(923)~error;  // Expressions like this only allowed after = sign.
	sml.Out << a;  // Can't do anything other than use variables directly.

	// sml.Out << a + 1;  // Error!
	type[a] b = a + 1;
	sml.Out << b;

How practical is this? There's a fine line between type inference and being extremely
explicit. Maybe even too explicit! Let's look at some possibilities. Another possibility
is to avoid types in normal statements completely, but to allow them anonymously like
so:

	312/3 * 5: x => sml.Out << x;
	f(): y => 32u y;
	new ClassObject: obj => ptr ClassObject;
	vector.get(): v ==> v.x: x, v.y: y, v.z: z => sml.Out << x << y << z;
	1: a ==> a + 1: a ==> a * 3: a => sml.Out << a;  // Any idea what a prints? 6 sml.Out << (1+1)*3;

I don't know.
I don't know if I can like this. Doesn't seem too practical. It's clean though. That's
the one positive thing. Mhm. I can't enjoy this kind of syntax. I do wish for there
to be a limit to nesting of calls though. Doesn't seem very productive, and it's nigh
unreadable. If we ban any kind of recursion we are at risk for being too cocky about
it. If we have to store every intermediary... phew that's going to be a pain in the
ass I tell you what.

	label {
		ref Window window = getWindow();
		< 32u h = window.getHeight();
		32u w = window.getWidth();
		< 32f r = h.divide(w);  // Can't do h.divide(window.getWidth())
	}

Where the < operator exports the declaration out of the scope. We wanted to use height
and ratio, but never w. Basically, the whole idea with allowing expressions to be
nested is an implicit type inference. Although the types match, the programmer never
sees the actual types:

	give(buffer.getValue());  // What does getValue() return? What does give take?

While elegant and short, the following sets the types in stone:

	InterestingLong x = buffer.getValue();
	give(x);

My initial idea was to implement type checks inside expressions. The problem with
these is that they increase coding noise a lot. Something that distracts the programmer
and makes him work less efficient. I just can't decide. It's difficult. What can I
do? I know. I need to objectively measure. Certain traits need to be quantified. Then
I can make informed choices.

Definitions:
* Noise - The amount of characters-to-goal ratio (code-golfing languages win here)
* Readability/Clarity: Inverse of Noise
* Simplicity: One divided by the amount of possible sentences (152 sentences => 0.658%)

I want to minimize noise but also have high readability. These two go against eachother.
Maybe passing around function returns directly is not that bad. Considering that the
callee will type check the incoming messages. This means that the caller doesn't need
to worry about the types. What if the caller calls a method on the returned item?
It would be very beneficial for the caller to be aware of the type, so that it is
always sure that the correct type is given. Consider the case where a different type
is given that has the same methods. Would this be acceptable?

Ideally, we'd love to remove redundancy as much as possible. For example, some new
ideas:

	new-statement: newptr[3] newptr[4] 32u x;

This implicitly creates a variable without having the syntax like:

	ptr ptr 32u x = new[ptr 32u](6){new[32u](8){0}};

The anonymous object:

	anon x = object.getSomething();
	otherobject.sendSomething(x);

The anonymous object can't be manipulated from this context. It's anonymous. No known
operations on it, the size is unknown, everything is unknown. The compiler will figure
out the type. The anon object is just a message that you can pass on. That's all.
The internet agrees that type inference is only useful where the type is already redundant.
Therefore, we need a new-statement (instead of new-expression) to remove redundancy.
The anonymous objects can be used so a higher class can pass around data without knowing
what it is.

With the addition of the anonymous object, the whole language becomes a lot easier.
What about extracting elements though? We have this pseudo-type with tuples when returned.
The same goes for arguments to functions. We could just say the following:

	anon x = f();
	g(x~a);

This would solve all problems, although it wouldn't allow an easy change in value.
I'm a fan of the fexpr types. They're powerful and versatile! Let's keep those. What
about anonymous tuples, just like anon? Can that be used? I don't know to be honest.
I wonder what can be done. Would we like some kind of type inference on the tuples?
That's an interesting question.

	ptr ptr 32u x = new(100){new(30)}

I like this, new basically infers the type from its context. The outermost new infers
the type to be ptr ptr 32u, the inner one has one ptr stripped. That's a very interesting
syntax. It's simple. Elegant. Nice. This does however posit that these must be on
the exact same line as a declaration.

One thing I'd like to have - which consistently fails - is to have a unified grammar.

	ENTER ::= { UNITS }

	UNITS ::=
		ACCESS '(' [PARAM {',' PARAM}] ':' [PARAM {',' PARAM}] [':'  ATTRIB ] ')' functionName '{' { STAT } '}'
		| ACCESS TYPE variableName = EXPR ';'
		| 'granted' ClassName ( '{' { TYPE name ';' } '}' | 'pass' )
		| 'granted' variableName TYPE
		;

	ACCESS ::= [ 'public' | 'private' | 'restricted' ] [ 'global' ];
	PARAM ::=
		TYPE '{' { variableName [',']} '}'
		| TYPE variableName
		;
	ATTRIB ::= [ 'const' ] [ 'pure' ] [ 'safe' ];
	STAT ::= FEXPR | IF | GOTO | CONSTRUCT;

	TYPE ::=
		primitive                                                  // 32u, 64s, 1u
		| '[' EXPRESSION ',' TYPE ']'                              // [8, [3, 8u]] arrays
		| ('const' | 'ref' | 'ptr') TYPE                           // const/ref/ptr prefixes
		| [ namespace '::'] class [ '[' GARG {',' GARG} ']' ]      // my::Type[Con: 32u]
		;

	GARG ::=
		TYPE
		| className ':' TYPE
		| variableName ':' EXPR
		;

The big idea is to allow a similar setup to Java and C++ classes. Methods need to
be added, as well as data. The data follows a similar pattern of TYPE name = EXPR.
The thing is that function signatures are types as well. Ideally, such signatures
should signal that we have an actual method and not a 'function value' or function
pointer.

	alias St = sml::out;
	grant Name {
		32u a, b, c;
		(:) control;
	}

	grant va 32u;

	// Per-object member
	ptr (:) x lambda { sml::out << "x!" << sml::endl; ++counter };

	// Per-object member, e meaning 'exception' on overflow.
	64ue counter = 0;

	// Per-class member
	(:) enter {
		x();  // print 'x!'
		x = lambda(:){ sml::out << "y!" << sml::endl; };
		x();  // print 'y!'
	}

Maybe the declaration statement can be unified.

	MEMBER ::=
		TYPE name '{' { STAT } '}'
		| TYPE name ['=' EXPR] ';'
		;

This simplifies the parser a bit. To solidify the inline-redundancy-avoidance: we
can specify the proper grammar.

	DECL ::=
		TYPE name = EXPR

I'm thinking that when we process the right side, a context is sent to the expr. This
context will bind to operators as 'new' or lambda such that they do not need to be
stated explicitly:

	ptr MyClass a = new(20);
	ptr (ref MyClass b : ) f = lambda [old] {
		return b: old;
	};
	ptr MyClass {x: a, y: b} = {a: new} + f();

One problem that is still not fully resolved is that I may want an expression in the
name section:

	32u x: a + 3 = f();

I'm thinking that this is rather confusing, I like the idea of keeping the left side
clean from expressions.

	32u x: a = {a: 3} + f();

Will add 3 to a. I'm really not sure what's preferable. The former seems more terse.

	32u {x: a, y: b} = f() + {a: 1, b: 3};

Not liking it too much.

	32u {x: a, y: b} = f();
	x += 1;
	y += 3;

That's a lot more clear to be honest. The problem of casting also comes up. How do
I do the following?

	8u {x: a, y: b} = f();

Should I cast inside the left side? Should I create a special cast operator?

	8u {x: cast[8u](a), y: cast[8u](b)}, 32u z: c = f();
	x: cast[8u](a), b: cast[8u](b), z: error += g();

	is the same as
	anon o = f();
	8u x = cast[8u](o~a);
	8u y = cast[8u](o~b);
	32u z = (o~c);

	anon o = g();
	x += cast[8u](o~a);
	y += cast[8u](o~b);
	z += o~error;

Maybe that is the most elegant solution! Because we turn multiple variables into single
variables on the left side... but it does obfuscate the code a lot. The readability
is just horrendous.

	32f {x, y} = {0, 0};
	x, y += {1, 3};

This idea uses placed names instead. It's syntactic sugar for:

	32f {x: 1, y: 2} = {1: 0, 2: 0};

Not sure if I like this. Maybe a Vector class is more useful in this case. I need to
make up my mind on this. I can simplify this by a lot. I think... Let's say we have
the following syntax:

	32u {x: a, y: b} = cast[32u](f());

Which for each case translates to the name wherever it is encountered

	32u x: a = cast[32u](f()~a);
	32u y: b = cast[32u](f()~b);

and NOT

	32u x: a = cast[32u](f())~a;
	32u y: b = cast[32u](f())~b;

The latter makes no sense, you can't cast an entire list of variables. What if we have
multiple types of variables and do not want to cast them all? Well too bad I guess.
Same goes for decls of this kind:

	32u x: a = f() + g();

is equivalent to

	32u x = f()~a + g()~a;

It doesn't allow me to asses each variable individually.

	32f {value: out, error: error} = sin(math::PI);
	if (error == 0)
		debug sml::err << "Algorithm doesn't correctly calculate error";
	out += 3;

I kinda like the idea of allowing expressions on the left side. It feels like the
arguments:

	f({x: cast[32u](a), y: b}: g());

On the other hand, the KISS principle tells me I should only use it to extract values
and that's it, so the above becomes

	64u {a: a, b: b} = g();
	32u n = cast[32u](a);
	f(x: n, y: b);

I think I need to stick with the KISS principle. The only thing I definitely like
is that function arguments can be directly substituted by expressions from the returns
from other functions. Or should that be illegal as well? The entire issue lies with
the 'anonymous' tuples. How we define operations on these. How these are created in
expressions. Maybe I should just do away with the whole complication of the grammar.
I don't particularly enjoy it myself. It's terse but it seems like a waste of effort.

I mean, what language actually does this in a practical manner? In most cases, we
define a class and the operations on it. The class is basically a very well-defined
tuple with well-defined operations.

	64f {a = 0, b = 0}, 64f c = 0;
	label { a: x, b: y, c: z += diag(); }  // Add the vector.

Yeah, in most cases, specific classes will be made to describe the return value type.
The above will most likely be:

	Vector vector;
	vector += diag();

Much simpler to understand. Where no such thing is desired is e.g. the calculation
of the sine or cosine values, where we may be interested in the error of the approximation.
In other cases, some kind of flag can also be passed down the returns, which is useful.

	1u t: timeout, 32u count: out = performActions();
	if (timeout)
		return ...;

This prevents us from making classes for every single return. C++ solves the conundrum
using std::tuple. I'd rather stay away from such functionality. Let's just say we
do it as follows: we can declare-and-extract values (using assignment and other compound
operators. And we leave it at that. No special syntax. There's just extraction, then
you can do whatever you want. with the results. A tuple has the following semantics:

All operations on a tuple redirect to the variable named 'out'.
An exception is the ~ extractor. It extracts the proper name.

That should be sufficient.

	// Manual extraction
	32f a = math::sin(math::PI);
	32f e = math::sin(math::PI)~error;

	// Terse extraction
	32f {a: out, e: error} = math::sin(math::PI);

For an extractor, we can apply a rule that states that only extractions can occur inside
the extractor, assignments can not occur.

	32u {value, counter: change, nice = 3} = f();

Instead, the correct form will be:

	32u {value, counter: change} = someobj.getDiffs();
	32u nice = 3;
	// 32u nice(3);

So all values are assigned when we perform an extraction. It does mean that only a
single function call is allowed on the right side. We can't really operate on the tuple
directly. What if we want to return by proxy?

	(32f {out, error} : 32f in : pure) sin {
		out, error: error = sin(in, error: 0f);
	}

	(32f {out, error} : 32f {in, error} : pure) sin {
		// Use maclaurin to compute the value
		out = in - pow(in, exponent: 3)/6 +
			pow(in, exp: 5)/120 - pow(in, exp: 7)/5040;
		error = pow(x, exp: 9)/326880;
	}

In the case of the sine function, the error is taken to be the largest possible error.
Ideally, it ought to be computed from x itself, which is an expression of cosine, which
translates into a sine (so back into this expression). The above shows an interesting
return method. We just let the values be assigned. The end of the scope returns. This
also allows returning unallocated objects. For example, an unallocated integer:

	(32u {out, other} : : pure) get {
		out = 1;
	}

Will not assign a value to other. This means that the callee:

	32u {out, a: other} = get();
	sml::out << a;  // Undefined behavior

This may be a good argument for making returns explicit, but there's a very good use
for not making them so. This specifically allows us to return unallocated PODs, something
that can save time.

So if you can only use a single call on the right side of an extraction, we get some
interesting grammar...

	// Using a different get
	{32u {out, other: something}, 8u e: error} = get();

This can in fact be done with a { lookahead, but that will clash with the single-statement
branches. So it's undesirable. The grammar becomes a lot more straightforward though.

	extract 32u {out, other: something}, 8u e: error = get();

I don't like extract, it isn't elegant. I like the former fexpr method better than
this.

	32u out, a: something = get().doSomething();

Works nicely in combination with both new variables and replacing old variables. This
looks stunning! Allowing arbitrary expressions should be fine, as they merely act on
the 'out' variable anyways. Yeah, should be just fine. The question that remains is
how the semantics of get().doSomething() are. Suppose both return tuples of (Class out, 32u a).
Then we should get multiple a. The first out (from get) is used when we did .doSomething.
Let's just say that the call that didn't use the .. hold on. Isn't + technically a
member call? Well, should that exclude the following?

	32f {a, error: error} = sin(PI) + 1f;

Since the 1f is the last call, no other items are returned. This is difficult. What
can be done? We can limit the right side to a single member expression. The `+` expression
will return a single 'out' that can be used. So since it has the highest prevalence,
that out will be used.

	32f a = sin(PI) + 1f;  // Single out there anyways

	32f {a, e: error} = sin(PI);
	a += 1f;

If you still want the error, you can extract it like the above. Hey I can't handle
every single use case. There are too many of them. Should the same be done for function
calls? As in:

	f(g());

I'm thinking this just binds g's out to f's in. That's fine.

	f({x: a, y: b}: g());

I'm wonderinf if the above should instead be:

	Type {x: a, y: b} = g();
	f(x: x, y: y);

This does impose one limitation that may be useful: we get to know what the types are.
The equivalent in C++:

	TypeA a = g1();
	TypeB b = g2();
	f(a, b);

The latter has one more character. Mainly due to named-x of my language. So what should
be done? You know, the return value should be quite obvious. An idea is to create an
inline-documentation generator. For example what types some arguments are. This aids
development and readability, whilst avoiding writing this tedious documentation during
coding. Hand-written documentation is often times lacking and is neglected when changes
occur. It's important to let the compiler do a lot of the work.

	32u {a, r: remainder} = 300 \ 200;
	sml::out << a << ' ' << r << sml::endl;

Should output `1 100`. This will probably be fine. So we've got fexpr, functions, modules,
methods, access, statics, purity, io,... seems like everything is in place. At least
I hope so. The semantics of constexprs and enums need to be made. All enums are `LIKE_THIS`.

	MY_ENUM;
	ERROR_IN_DEPTH;
	IO_READ_ERROR = 3;

	(:) enter {
		try {
			throw IO_READ_ERROR;
		} catch {
			IO_READ_ERROR {
				debug sml::err << "io reading error, aborting";
				sml.abort();
			}
		}
	}

All enums are thusly declared in the same class. Parents can access them by writing

	SubClass.IO_READ_ERROR

Which can be thrown. The subclass's enum is not visible to superclasses. A program
wide mechanism should be in place. The enums can also be placed in the grant list.

	grant Aa {
		IO_READ_ERROR;
	}

	(:) myFunc {
		throw Aa.IO_READ_ERROR;
	}

The enums have to be generated in the class where they are defined, of course. You
can't state `sml::Main::IO_READ_ERROR = 5`. It would be nice to automatically propagate
a class to the lower classes such that reading is easy, but this breaks demeter's
law. It would be really useful to have internal enumerations for - say - a library.
Then again, it would also be incredibly useful if you can nest the library where it
is needed instead of making it global. Grants solve this issue. A possiblity is to
allow grants to 'pass' at all times:

	grant MyClass;

This will just pull out whatever's needed without stating the requirements. If the
class does not present any requirements, compilation fails. This can be useful for
further propagation into the system, using successive grants. We could also state that
the sml library is always automatically granted, and not allow global access to the
top level.

	::Main::doSomething();

is not allowed anymore. Should this be changed to be so? I like the purity of it. That's
for sure. Eh, grant merely becomes a comment then.

	// Grant the class Aa, it uses .substring(from: 32u, to: 32u);

Then, the callee (creator parent) uses:

	ThatClass[Aa: sml::String] myclass;

Should grants always be forced? By not allowing global access, we can actually force
a very clean paradigm of programming. Where copying one folder structure simply doesn't
affect the correctness of all containing classes. As long as all grants are given,
the program will behave correctly. What can be done is to let the compiler generate
a message upon "ungranted class". The compiler can probably also state what the grants
are using static analysis. Hmm, is there then any use for the grant list? Well, it
can be used to actually see what the grants are. It is redundant information isn't
it? Why not just allow the compiler to do something like

	cxyc --grant lib/something/deeper/My.cxy

And get a list over the grants. It can even read deeper into other classes that get
grants. So you get all the information necessary. Yeah it's redundant. Out with it!
The only problem is that the semantics of the grants is still unexplained. Other items
as purity luckily come through. This also destroys the idea of globals by global positioning.
What happens instead is that we just send in a class with a static variable. What if
we want to send the variable directly?

	static 32u myglobal = 3;

	(:) enter {
		SubClass[global: $myglobal] a;
		a.increment();
		sml::out << myglobal;
	}

	// SubClass (granted)
	(: this) increment {
		@global += 1;
	}

Should output 4. Wait, doesn't that make everything pure? Can we send in a package?
I think that's wrong to do at the moment. That seems too powerful. In most cases, who
needs the entire package? Even if, maybe the design should've been different. Another
thing is that the type of the `myglobal` variable or `global` is not specified in the
SubClass class. That's actually quite nice. Because we don't mandate specifics of the
type, the type can be a wide range of different variables. For example in the above
I sent in the pointer to the `myglobal` variable. This means that 'global' is a variable
of that class of the exact same type 'ptr 32u'. This seems extremely powerful. Just
imagine what you can send in there. You can send any references with similar semantics
as the 32u.

	static math::SuperInteger myglobal = 3;

	(:) enter {
		// SubClass[global: myglobal] Fail because you can't derefence.
		SubClass[global: $myglobal] a;
		a.increment();
		sml::out << myglobal;
	}

The interesting thing is that this can only be permitted when using compile-time static
variables. For normal class-local variables, this is not permitted. Is the function
using it still pure though? It gets a variable as an extra parameter, in a sense. Interestingly,
this allows you allows you to substitute pretty much all variables. So what is an impure
function? Anything using a static granted? That seems pretty accurate actually. sml::out
is an automatically static-granted variable. We all know that it changes state. So
are static variables that are sent in also impure? This would suggest yes. Are there
any cases where sending in variables via concepts is not desired? A deeply nested structure
where the second-top and the deepest levels both need something. You now need to feed
the entire chain downwards. That's rather tedious. Luckily, you should specify what
you need from the start (bottom-up coding). Even then, it's only a one-time-thing.

So what is there to do with the variables that should be CONSTEXPR? Well I suppose
making them all-caps is the only option. It isn't a bad decision. Hell, the templates
can even take in class-local variables. Then we have glocals!

	32u counter = 0;
	(: : pure) constructor {
		SubClass[name: counter] object;
	}

This solves the entire glocal/global problem. Yes, we do have to pass them on, but
once they are in the objects, they're pretty much good. Hell you could create an object
and just call the static methods (`this` is just ignored).

	(:) enter {
		32ue counter = 0;
		math::Mathematics[Float: 64f, counter: counter, MAXTIME: 100] m;
		m.sin(math::PI);
		m.calculateSomething();
		// ...
		sml::out << counter;  // Could be non-zero!
	}

All kinds of nifty optimizations may lie here. The most important one is that the
static variables can just be replaced in the code generators. That makes the class
instantiation extremely powerful. One thing is that the lvalue must not use dynamic
placement:

	(:) enter {
		1u choice = 1;
		[2, 32ue] counter = [0, 0];
		Class[counter: counter[choice]] c;
		// ...
	}

The problem is that choice can't be read inside the `Class` object. This implies that
choice must also be given. Instead of explicitly giving it, why not implicitly do so?
Of course, in this case counter has to be an lvalue. What the templates do is directly
replace the name by the expression. The natural question to ask is; what is pure?
I don't know anymore. I really really like the fact that everything seems so pure.
So are given references impure to touch now? They're explicitly given. Maybe we need
to limit purity to IO. Builtin IO, that is. Anything using hack is not pure? Sounds
reasonable... actually that sounds perfectly reasonable. Considering how hack allows
IO in the first place by calling the system. Finally! We have something concrete. Another
interesting tidbit is the standard module library (sml). This will be passed implicitly
everywhere. Its IO 'global' variables will be impure to access. This separates truly
pure Cxy from the hacky Cxy. I like it!

	// Main.cxy
	32u counter = 0;
	(:: pure) constructor {
		construct counter(0);
	}

	(:) enter {
		32u CO = 3;
		sub::Class[Type: This, AFF: CO] value;
		value.go();  // Would fail if CO != 3
	}

	// sub/Class.cxy
	if [AFF == 3] {
		(:) go {
			sml::out << "Woah, what's going on?!";
		}
	}

Would be cool if we could send in namespaces. That'd make a big difference I think.
Unfortunately, namespaces have the same names as variables. Most of the time at least.
The problem with allowing namespaces is that it obfuscates local folders. What if one
overlaps with the given namespace? No, that's ugly and you then need to consider the
subfolders. That is ugly. Let's not do that. So we stick with normal variables, classes,
and constexprs. Any pure function should be computable during compile-time. This allows
the compiler to make sure that constexprs solely use other constexprs and pure functions.

	ComplicatedClass CLASSNAME;  // Valid as long as the constructor is pure

This is often not so trivial in C++, where basically only PODs can be constexpr. So
what can be done now? I think I just need to shear off the excess fat from the grammar.
Currently, the grammar is quite big. I'd like to make it more elegant. Where do I start?
I guess we could take a look at the type expressions and expressions in general. Hopefully
the lexer can aid us here. What I actually not really like is the non-separation of
variable names, functions, and namespaces. There's just something that irks me deeply.
A possibility is to prefix all namespaces with :: or another operator. Methods are
variables so those are probably fine, although they have wildly different semantics.

	static (:) something lambda { sml::out << "hey you"; };
	static ptr 32u value new(10){3};

	method (:) enter {
		something();
		value[3] += 4;
		sml::out << value[3];
	}

Yeah that's the nice thing about variable declaration. It's that you have a single
name followed by something else. This can easily be exploited. I think the grammar
needs a beautification of some sort. How do we cleanly define methods and values of
a class?

	method (:) enter { }
	member (:) renter lambda { };

It makes sense though, although 'method' and 'member' are quite annoying to have to
write all the time. That's why I like the more context-sensitive syntax.

	Signature (Type)
	|   Name
	|   |          Method code
	|   |          |
	(:) methodName { /* code */ }

	Signature
	|   Name     Initializer
	|   |        |
	32u variable = 0;

As can be seen, the method code can be considered an implicit lambda-initializer.
This seems far-fetched. I'd like to have a general syntax that can also infer the
type.

	ptr 32u a = new(10){0};
	(:) m = lambda[a]{ sml::out << a[9]; };

I like this more. It's cleaner and doesn't require any special syntax. All that's
needed is for the right side to be aware of the right side. It just infers the type
from the left side. I hope that's manageable. Now, as for methods. What should be done
there? We know that we always have one class per file. This makes programming easier.
We know that top-level types will be class members. Now, there ought to be a distinction
between methods and variables. Scala uses 'var' (or val) and 'def' to distinguish them.
My main issue is that the syntax seems to be incompatible. For example, for normal
variables we use '=' or a constructor. For methods we use a direct { sign. I think
this needs to be changed

	32u a = 3;
	(:) enter = {

	};

The problem with this is that it's actually quite ugly. It requires a semicolon and
treats methods as variables (which they are not). We could summarize using the following
rules:

	Each member that has a direct function type is a method.
	Each member that is not a direct function type is a variable.

Ideally, the distinction between variable and method should be minimized.

	MY_ENUM; ANOTHER_ENUM = 3;
	32u CONST = 391 + ANOTHER_ENUM;

	static 32u varname = 3;
	(:) enter {
		++varname;
	}

What about static and non-static members? We could just say that: if never accessed
in a certain way, they are static/non-static. I think the best thing to do is to move
`(:)` up in the 'type' hierarchy. Let it be its own 'function signature'. With this,
we can easily distinguish between methods and members. Let's look at an example:

	static ptr (:) fptr = {};  // Function type inferred

	(:) enter {
		fptr = lambda[](:){};  // Function type specifically stated

		// Illegal, no expressions in type[name]!
		// fptr = lambda[]type[@fptr]{};
	}

The lambda should always return a ptr to the signature it's using. This way, we can
ensure that the correct type is given. This also implies that the signature isn't a
type directly, but merely a specific side-thingy. I don't know how to explain it. It's
nice really. I'm thinking static may also be used on functions. The reason is that
'this' really makes it more ambiguous. Hell if you wanna cast it you can just add the
'this' variable if you want:

	(::pure) doStuff {
		debug sml::out << "Hello!" << sml::endl;
	}

	static (:) enter {
		This object;
		object.doStuff();
		ptr (: this : pure) fptr = doStuff;
		fptr(this: object);
	}

There's something I don't really like here. First of all we're disconnecting the signature
from its original intent: to convey the actual signature. Yeah, static just belongs
inside the signature.

	(: this : pure) doStuff {
		debug sml::out << "Hello!" << sml::endl;
	}

	(:) enter {
		This object;
		object.doStuff();
		ptr (: this : pure) fptr = doStuff;
		fptr(this: object);
	}

There, much better. Not the most elegant solution, but pretty elegant. Like in the
middle of elegance on the scale of elegance,.. right? Anyways, this is how I like it.
What if the class is from a subclass?

	(:) enter {
		SubClass subclass;
		subclass.doStuff();
		ptr (: SubClass this : pure) fptr = SubClass::doStuff;
		fptr(this: subclass);
	}

That works out quite wonderful. Yeah, I'm starting to really like this. It's also easily
parsable. We can always distinguish between TYPE and SIGNATURE. That's really freaking
nice. I hope I can continue with this. Okay, so that's out of the way. What about actual
static variables though? Those aren't addressed yet. Do we still need the static keyword?
Perhaps. I'm not sure. It doesn't really affect the type of the variable. Maybe name
mangling could work for static variables... Imagine a mangled static name:

	32u {MANGLED, local};

	(: this) run {
		local = MANGLED;
	}

	(:) run2 {
		MANGLED = 34;
	}

Just that the CAPITALS are already taken by constexprs. If only there was an alternative.
Yeah maybe static is the best bet for variables.

	static 32u unit_counter = 30;
	32u current_id;

	(:) constructor {
		++unit_counter;
		sml::out << unit_counter << sml::endl;
		construct current_id = unit_counter;
	}

Looks pretty much fine actually. And I can always add `static { ... };` if needed.
Yeah, compromise. There's no name mangling for statics apparently. I guess that's fine.
The only name manglers are pretty much enums and CONSTEXPRs. Well, classes and variables
also mangle their names, but not as strictly. We could also just say to hell with static
variables and just create an object instead. That's actually very elegant! Just imagine:

	32u counter = 0;

	(: this) constructor {
		construct counter = 0;
	}

	(:) enter {
		This local;  // Always alive during the program anyways.
		SubClass[counter: local.counter] subclass;
		// ...
	}

That is so elegant... I love it! That's absolutely fantastic. It also allows scalability
since we can just clone the object and repeat the previous run. I think this concludes
the search for 'statics'. Any other topics to consider? We've removed grants altogether
and now just use direct substitution. There are variables and methods, functions can
also exist. CONSTEXPR and `ENUMERATED_VALUES`. What more? Alias of course. I think
it's for the best to just do away with alias altogether. I mean it may be nice for
a short time but we can hardly do any direct substitution with it. Well, it kinda removes
the hell of large namespaces/class names, which is nice really, but I don't think that
it will pay off in the end. The reason is that we just cheat ourselves of the true
variables. Especially with the alias statements that allow this:

	alias ClassName = namespace::ClassNameName;

We'd start looking at the granted ClassName whenever we'd see it. That's what I don't
like about it. Secondly if we allow

	alias nm::Cn = namespace:: ClassNameName;

then whenever we encounter "nm::Cn", we'll be looking for that folder, it may even
clash with the folder name. That's definitely not desirable in a language that is
supposed to be easily readable. Yeah, let's avoid that ugly mess! Then, the language
becomes quite simply actually...

	methods
	variables
	grants
	static ifs

Using static ifs for code generation seems very appealing, but it doesn't bring out
the full capacity of code generators. I'd love to see the code generator that was discussed
in the beginning in place. But that comes later. First we need to implement this. Well,
basically de-plement what has been implemented. Expressions need to be cleaned. What
ties types to expressions is the ability for arrays to contain expressions of CONSTEXPR
types. They can't contain non-constexpr expressions. So yeah, I just need to deplement
some of the stuff and that should be the end of the grammar.

	8ue counter = 0;

	(:) enter {
		sml::out << "sample program" << sml::endl;
		This object;
		try {
			while (true) {
				object.counter += 1;
			}
		} catch {
			sml::UNSIGNED_OVERFLOW
				sml::out << "Unsigned integer overflow detected"
					<< sml::endl;
			sml::SIGNED_OVERFLOW
				sml::out << "Signed integer overflow detected"
					<< sml::endl;
			any
				sml::out << "Caught an unknown signal!" << sml::endl;
		}

		sub::Class[counter: object.counter] object2;
		try {
			object2.check();
		} catch {
			sml::UNSIGNED_OVERFLOW
				sml::out << "Exactly what we expected" << sml::endl;
			any
				sml::out << "Caught an unknown signal!" << sml::endl;
	}

	// sub/Class.cxy
	(: this : pure) check {
		counter += 1;
	}

I really like the idea. It's clean and tractable. I really like it. Really really like
it.

	private 32u {a, b, c}, [100, 8u] buffer, sml::String holder;
	public ...

Those look nice as well, goes hand-in-hand with the variable declaration syntax. Even
the extraction syntax!

	restricted 32f {value, error: error} = sml::sin(sml::PI);

	(: this, 1u test: pure) constructor {
		if (test)
			construct value, error: error = sml::cos(sml::PI);
	}

I've been thinking. I don't like restricted/public/private that much actually. So an
idea popped into my mind: every non-method is private. Every method is public. However,
every `ptr SIGNATURE CONSTEXPR = { ... };` is considered a private method. Hmmm...
it does look kinda ugly to be honest.

	ptr (32f out : this : pure) COMPUTEINTERNAL = {
		out = sml::sin(0.1);
		out += 1.0;
	}

	(32f out : this) get {
		out = this.COMPUTEINTERNAL();
	}

Nah, there has to be something better than this.


	private (32f out : this : pure) computeInternal {
		out = sml::sin(0.1);
		out += 1.0;
	}

	public (32f out : this) get {
		out = this.computeInternal();
	}

Actually this looks pretty much fine... doesn't look bad at all, but perhaps it'd be
best to put the access specifier somewhere else. I'm thinking after the type signature.
But honestly, it doesn't look bad. We can just say everything is public by default
such that enter becomes easy to learn (upon being introduced to the language). The
nice thing about not putting access in the signature is that it doesn't actually matter
to the semantics of the program:

	(:) enter {
		(::private) name = This::staticFunction;
		name();
	}

If you can return the function, then it loses all forms of specification. It's absolutely
unnecessary. Alright.

	ACCESS TYPE namelist [More] = EXPR ';'
	ACCESS SIGNATURE name '{' code '}'

Is the basic setup of a class. Simple? Yes. Elegant? Yes. Short? Yes I'd say so. For
a strongly and statically typed language. Then, the bodies of functions are like:

	STATEMENT
	{ EXPR [name ':' name] }+ ['=' EXPR] ';'

I need to really capture the essence of the grammar, let's try again:

	DATA ::=
	ACCESS { TYPE (
	               name [ ':' name | '=' EXPR ]
	               | '{' { name [ ':' name | '=' EXPR ] }+ \{ ',' } '}'
	              )
	       }+ \{ ',' } [ = EXPR ] ';'

Good, that's out of the way. I think that's exactly what I want! Now for the methods

	SIGNATURE ::= '('
	                (
	                 { TYPE (
	                         name
	                         | '{' { name }+ \{ ',' } '}'
	                        )
	                 } \{ ',' } [ ',' ]
	                )
	                 ':' $1 ':' ['const'] ['pure']
	              ')'
	ACCESS SIGNATURE name STATEMENT

Our enums:

	[ ACCESS ] { ENUMNAME ['=' EXPR] }+ \{ ',' } [ ',' ] ';'

Statements:

	STATEMENT ::=
		'if' '(' EXPRESSION ')' STATEMENT
		| '{' { STATEMENT } '}'
		| 'goto' name ';'
		| 'label' name ';'
		| 'try' STATEMENT 'catch' ...
		| 'while' '(' EXPRESSION ')' STATEMENT
		| 'for' '(' STATEMENT ';' EXPR ';' STATEMENT ')' STATEMENT

Where $1 denotes the first capturing group. The regex, not what has actually been
captured. So the regex is just copied there. `\{}` means to interpolate the previous
`{}` group wtih the sequence within. `{ 'a' } \{ ',' }` would accept:

	(nothing)
	a
	a, a
	a, a, a

And so on. The trailing comma is not allowed since we don't have lists like most other
programming languages. C++ for example allows the trailing comma in enums:

	enum A {
		a,
		b,
		c,
	};

But we don't have enumerations in that sense. Our enumerations are ended by a ';',
which makes the use of trailing commas not worth anything. And trailing commans in
arguments are just ugly. The trailing comma is not necessary. The only reason they
are in enums is that you can easily copy the last line and add an entry. For expressions
containing raw arrays, we may allow a trailing comma. Let's look at the use case:

	[8, 32u] a = [
		1,
		2,
		4,
		8,
		16,
		32,
		64,
		0,
	];

Longer lists are even more complicated. It's always nice to have the ability to trail
the comma. But only for lists like these. We don't need them in arguments. Or do we?

	myFunctionName(
		a: this.something()~height,
		somecomplicatedname: alfa + beta / gamma \ THETA,
		[3, 4, 5, 6, 7],
		control: [1, 2, 3],
	);

Yeah,... that's actually pleasant to have. Just in case you need to add another argument.
Are commas really necessary for lists though? I suppose they can always be optional.
I still have unanswered questions. For example, isn't [ the start of a type? And types
can be evaluated in expressions for having static members. Hold on... wait a second...
can it be solved by... no. Creating an object?! This would create a name but solve
the entire conundrum... hold the fk on man :O.

	ClassName name;
	name.doSomething();

That doesn't really aid readability though. As in, you have a static local object
and you're not using it that much. What about a class-local constexpr one?

	Classname NAME;

	(:) enter {
		NAME.doSomething();
	}

No I think it's more useful to be able to call statics directly. But how do we reconcile
the use of [] inside an expression? [8, 32u]::size could be useful... right? C solves
it by using the sizeof operator. I think it's an interesting approach. It works on
any class. '::size' looks more like some static method. I think we need an operator
for this just to solve the entire problem. Secondly, we need to look at other topics.

	[3, 32u] a = [size[type[a]], 4, 6];

Should give an array that's `[96, 4, 6]`, because `3*32` is 96 bits of information.
I mean, what information can you get out of primitives? Well you can get the maximum
number from 32f, or 32u, or 50u. But you can also calculated that using size:

	32u MAX = math::pow(2, size[type[MAX]]);
	32u MIN = cast[type[MIN]](cast[32uo](MAX) + 1);  // Overflow back to the least

We'd like to know the size of something, the maximum, minimum, whether it's a primitive,
what kind of primitive. This is useful for generating code.

	if [sml::primitive[GIVEN]] {
		// ...
	}

It also makes me think that perhaps, it'd be a good thing to have nestable grants.
With this I mean that you can send in partial grants. I'd love for unit tests to be
incorporated in the compiler. The idea is that every class that gets instantiated has
a set of tests:

	test myTest {
		if [size[GrantedType] < 8]
			fail "Granted Type has a size less than 8 bits";
	}

The compiler can run the test for each distinct subtype. This can be very useful in
removing errors and applying your own semantic checks to the class. We should obviously
run the lowest classes first.
I guess that's solved then, the primitive types don't really fit into expressions.
So we can still have the array syntax without a problem!

	if [has[Granted, (:) enter]];
	if [has[Granted, public 32u a]];

I'm not sure about the above. Weird things happen in metaprogramming. Instead of SFINAE
I'd like to use something that's more direct (and takes considerably less time). So
how about metaprogramming? Most issues have now been cleared up, grants are easier,
expressions are simpler, statics are resolved. I'm still wondering if we can eliminate
types from expressions and encapsulate them in "size" or other operators.

	# Main
	CLASS ::=
		{ DATA | ENUM | METHOD | STATIC_IF }*;

	DATA ::=
		[ ACCESS ] { TYPE (
		                   name [ ':' name | '=' EXPR ]
		                   | '{' { name [ ':' name | '=' EXPR ] }+ \{ ',' } '}'
		                  )
		           }+ \{ ',' } [ = EXPR ] ';'

	ENUM ::=
		[ ACCESS ] { ENUMNAME ['=' EXPR] }+ \{ ',' } [ ',' ] ';'

	METHOD ::=
		[ ACCESS ] SIGNATURE name (
		                           '{' { STATEMENT }* '}'
		                           | STATEMENT
		                          )

	STATIC_IF ::=
		'if' '[' EXPR ']' '{' CLASS '}' { 'else' 'if' '[' EXPR ']' '{' CLASS '}' }* 'else' '{' CLASS '}'

	# Auxiliary:
	SIGNATURE ::= '('
	                (
	                 { TYPE (
	                         name
	                         | '{' { name }+ \{ ',' } '}'
	                        )
	                 } \{ ',' } [ ',' ]
	                )
	                 ':' $1 ':' ['const'] ['pure']
	              ')'

	STATEMENT ::=
		FEXPR
		| WHILE_STATEMENT
		| IF_STATEMENT

I've got an idea for type information! WOOT! Let's go:

	type[Name]~size
	type[Name]~name
	type[Name]~max
	type[Name]~is_primitive
	type[Name]~is_class

That's absolutely beautiful! The static ifs allow us to conditionally exclude and include
code. This is very useful.

	32u ISVALUE = 33, OUTVAL = 5;

	if [ISVALUE == 34] {

		(:) enter {
			sml::out << "It's 34" << sml::endl;
			if [OUTVAL == 5]
				sml::out << "Also outval is 5" << sml::endl;
			else
				sml::out << "Outval isn't 5" << sml::endl;
		}

	} else {

		(:) enter {
			sml::out << "It's 33" << sml::endl;
		}

	}

As can be seen, conditional compilation simply removes anything that isn't worth adding.
This means that there are no semantic checks on the code that is being eradicated.
There are however syntactic checks, since that never changes. `type` being used for
type information is awesome.

	EXPR ::=
		'(' EXPR ')'
		| EXPR BINOP EXPR                                # Arithmetic: a + b, c % d, e * f, precedence implied  P: 1
		| UNOP EXPR                                      # @dereference, $$const_ptr, -negative                 P: 2
		| EXPR '(' ARGLIST ')'                           # Calling a method, very high precedence               P: 3
		| [ namespace '::' ] [ classname '::' ] member   # Static member expression
		| EXPR '.' member                                # Getting a member                                     P: 5
		| EXPR '->' member                               # Getting a member                                     P: 5
		| EXPR '~' name                                  # Extracting from a tuple                              P: 4

I'll continue the grammar later.
Interestingly enough, new can be considered an automatic function class:

	(ptr Type out : ) new {
		32u {bits, rem: remainder} = type[Type].size \ BITSPERWORD;
		hack("malloc " + (bits + rem));
		out = hack("last");
	}

Something along those lines. I need some kind of pseudo-pure class for new. Anything
that returns a ptr to something could be considered pure, if it calls new it's no
problem. Just comparing the pointer in a non-equality way should be impure. New is
an operator so perhaps it's cool, but I'd still like to have a class for this.

	(ptr Type out :: pseudopure) new ...

Could be it, a method that we declare 'pure' solely on the basis of trusting us (the
programmer). Another idea is to let exceptions be pure. This way, exceptions that
aren't pure are only caught in the impure section. Normally, if a pure method throws,
then it's based on the input arguments. However, new may throw as it implicitly links
itself to a global store or memory. We could say that memory is implicit and always
supplied automatically though. This would make new pure. Yeah, we don't need specific
pure/impure enums, since the impure functions can only throw in such a way that enums
are already caught in impure code. Let's just say that new is indeed pure.

	private POINTERS_TRUE;

	(:: pure) enterPure {
		try {
			ptr 32u {a = new{0}, b = new{0}};
			// if (a > b)  // Impure
			if (a == b)  // Pure
				throw POINTERS_TRUE;
		} catch {
			sml::OUT_OF_MEMORY {
				throw;
			}
		}
	}

	(:) enter {
		try {
			enterPure();
		} catch {
			POINTERS_TRUE {
				sml::out << "Pointers have the same values" << sml::endl;
			}
			sml::SIGINT {
				sml::out << "Interrupt signal caught" << sml::endl;
			}
	}

I like the cleanliness of the above. Only issue I have is that I can't assign a variable
to the enum. The enum has no name. Perhaps `This::Enum` could do. Or `enum[This]`
with `enum` being an alias for `enum[This]`.

	private SYSTEM_ON, SYSTEM_OFF, FAILED_TO_START;
	(:) enter {
		enum a = SYSTEM_OFF;
		try
			startSystem($a);
		catch {
			FAILED_TO_START
				sml::out << "Failed to start the system" << sml::endl;
		}

		if (a == SYSTEM_OFF)
			sml::abort();

		// ...
	}

Creating an object in order to use static functions seems very appealing actually.

	(:) enter {
		math::MathematicsEnginge me;
		sml::out << me::sin(me::PI) << sml::endl;
	}

It'd remove the grammar lookahead ambiguity as well. It also solves the idea of using
different subspaces:

	(:) enter {
		sml::Mathematics m;
		sml::out << m::sin(m::PI);
	}

I like the form of the assignment operator using captures:

	expr: name, expr: name, exrp: name = expr;

It's very powerful and simple. With types it becomes slightly more complex. Although
I suppose with a type in the beginning we know what the form of the code is going
to be. So we don't need an fexpr like the extremely versatile one. For expressions
starting with a namespace name:

	(:) enter {
		sml::Mathematics m;
		m::sin(m::PI);
	}

Using a three-sized lexer buffer we can identify [name '::' name]. And state that
this is in fact a 'static member', and turn that into a single token. I'm a little
unsure. To remedy the problem, we could say that sml is both an object as well as
a package. This way we can allow using sml::out, whilst also support sml::Vector.

	math::Mathematics math;
	math::computeSomething(math::PI);

Should be allowed. Depending on the token directly after the name, we can just see
if it's a type or not. What I don't like about it is the following:

	math::Mathematics math;  // How do I know this doesn't hold state?
	(:) enter {
		math::doSomething();
	}

I need to actually look at all invocations of math::function in order to see if there
are any '.' operators used on it. The biggest problem for me is that if the object
has any constructors, those will be called on the declaration of the object.

	(:) enter {
		sml::Mathematics M;
		M::sin(M::PI);
	}

Oh,.. oh. Doesn't look bad. I like this. It uses the object as a constexpr. This
means that non-static `(: this)` methods can't be called on it. It should also
make all members static. The members that are used, at least. Hell I bet I could get
away with just using the dot operator, but that won't show I'm using a static method
if we are using an actual object.

	(:) enter {
		sml::Mathematics M, m;
		m::sin(M::PI);
		M::cos(m::PI);
	}

The semantics of a constexpr variable are such that you can't change any member variables.
What happens to sml::out though? It doesn't exactly hold state, it just outputs to
the given system stream.

	(:) enter {
		OUT << "Hello World!";
	}

Where OUT is a constexpr granted to the main class. That could be powerful. OUT could
also be a granted object like:

	out << "...";

or even

	sml.out << "...";

Where sml is a global object that contains the out object. I think this is the most
correct version. This makes me consider whether it'd be good to just say we pass
everything down, even sml classes.

	// Main.cxy
	(:) enter {
		sub::Class[String: sml::String, Vector: sml::Vector] subclass;
		sml::out << subclass.doSomething();
	}
	// sub/Class.cxy
	Vector[32u] control;

	(:) constructor {
		construct control([1 2 3 4]);
	}

	(String out :) doSomething {
		out = control.toString();
	}

I don't know, seems a little tough. It does keep each class very sustainable though.
As in, you can easily find out requirements for any class. Luckily you can use `type[name]`
to alleviate some of the troubles.

	(:) enter {
		sub::Class[
			String: sml::String,
			Vector: sml::Vector,
			Control: sml::Logger,
			Graphics: gl::GraphicsHandler,
			]
			SUBCLASS;

		type[SUBCLASS] alpha;
	}

This does give quite a convincing case for whole-namespace insertions though. I'd
love to be able to make the language as clean as possible. The problem I have with
C++ is that you need to define a global library. I like it when it's more local.
Imagine classes just giving arbitrary types downward. That's so awesome. The primitives
are of course very important. One thing that can be done is to make sets, maps, and
similar utilities primitives.

	(:) enter {
		set[32u] a;
		a += 3;
		a += 6;
		if (a[6]) {
			sml::out << "a contains 6";
		}
		a -= a;  // Remove all elements

		map[32u, 1u] b;
		string c;
	}

Another idea:

	private LOG_BOUNCE;
	(:: pure) enterPure {
		32u a = 5;
		bounce LOG_BOUNCE a;
		++a;
		bounce LOG_BOUNCE a;
	}

	(:) enter {
		try {
			enterPure();
		} bounce {
			LOG_BOUNCE name {
				sml::out << name;
			}
	}

What this does is that it allows enterPure to to continue after the bounce without
directly calling any impure functions.

I mean, just think of the classes when they have their grants. You just compile --grant
and get a list of all necessities. You know that this class will work. Of course,
100% likely when all the methods in the class are pure. This is excellent for building
libraries. What if a library wants to use a vector internally? They can just pass
it from the top level downwards. It doesn't seem very practical though. I mean I
get that primitives are defined everywhere, but there are so many important data
structures that should also be everywhere. An example is the array. Incredibly important
to have that accessible at all times. The thing is though, most library authors use
an internal version of their own array, because the provided one wasn't good enough.
Heard that story time and time again. Another thing is the allocator that will be
used. For example, having a fixed-size array for std::vector instead of a dynamically
growing one. Of course, one could also just create a specific version of vector using

	(ptr Type out :: new) create {
		32u {size, remainder: remainder} = type[Type]~size \ OCTETSIZE;
		if (remainder)
			size += 1;
		hack("void *a = malloc(" + size + ");");
		hack("assign", "out a");
	}

We can say this is an allocator, it just allocates some memory. It could also be
a cyclic allocator that overwrites previous bytes, but I'm a little uncertain. Perhaps
it's best if the allocator is implemented in a custom class instead. This will solve
a lot of trouble. Even then, a custom allocator appropriating class can easily be
built.

	[1024, 1u] bits;
	10u at = 0;

	(ptr Type out : this, 32u in, ptr any location : pure) allocate {
		if (location < at)
			throw INACCESSIBLE_LOCATION;
		out = cast[type[Type]](at);
		at += Type~size;
	}

	(:) enter {
		col::Vector[Alloc: This] vector;
		col::Map[Alloc: This] map;
	}

This way, we can create cheap and simple allocator classes and pass them on to the
appropriate subclasses. In a loop, we can do:

	Allocator[32u] allocator;
	while (some_condition) {
		ptr 32u a = allocator.allocate();
		@a = 130 + condition_variable;
		// ...
	}

Basically a garbage collected loop. We allocate a large amount on the stack, and
use the allocator as an abstraction for 'allocating' objects.

An interesting aspect of any sorted array is that the addresses of the objects could
be used to sort the elements. This is currently an impure operation. new has always
been pseudopure, but I'm considering just making it pure. Really keeping it to 'hack'
to be impure:

	SortedArray[ptr 32u] sorted;
	(: this : pure) myfun {
		for (32u i = 0; i < 1000; ++i) {
			ptr 32u x = new{0};
			sorted.insert(x);
		}
	}

Could give different arrays each time. That should be fine. Hell we accept side effects
due to glocals, why not allow this indeterminism? The action of taking a value's
pointer and comparing it shows something that is not deterministic. One can base
decisions on those values. We can just imply that the system is given as an argument.
Yeah, that solves it. We just give an instance of the memory to the program. If the
same instance is given, then we should also get the same values. Of course we never
get the same instance. Oh well. Compromise.

I think the semantics for functions {} need to be inspected.

	new[32u](10, address: delete(old)){0 + 1};
	lambda[a](:){ /* code */ }

I think the syntax for these expressions could just be special.

	(:) name { /* code */ }
	32u name2 {0};
	32u {x: a, y: b} {a: 0, b: 0};

I think perhaps these rules are for themselves. Do I have everything then? Is this
sufficient?

	public (: this : pure) inc {
	}

Oh, idea, instead of having some functions, why not just use 'restricted' on methods?

	restricted (: this : pure) myMethod {

	}

If a method is restricted, that means that it can not be changed. This makes all
methods true lambdas. The problem with that is that the methods can be changed from
the inside. In that case, the method would need to be const.

	const (:) myFunction = {
		// Why use an equals? Because it's seen as a variable.
	}

I really would like to unify the whole semantics game into a simple system. Every
file is a class, every instantiation creates an object. The object contains data
and methods. Or it contains just data, but methods are specialized data.

	(:) enter {
		32u a = 0;
		ptr (:) x = lambda [$a] { @a = 10; };
	}

There has to be something called "class-scope", for which the semantics are slightly
different than statement-scope.

	name::Type {x: a, y: b}, 32u z: c = f();
	(: this) method {
		this.z = 10;
		label name;
			--z;
		if (this.z > 3)
			goto name;
	}

I'd like to get to the core of the language. This would remove a lot of statements.
The only ones left are assignment, declarations, expressions, and if/goto/label.

Man I just wanna have this all neat and finished. I feel like it's come a long way
already. Just an idea about minimalism though. Why not remove enums altogether and
instead just throw random names as you like?

	throw whatever_i_want_dude;
	throw myError;

Or just do not find it necessary to declare enums. This would simplify catching the
errors as well. How is the grammar fully LL(1)?

	Type member;

	(: this) constructor {
		construct {
			member(1 * 8);
		}
	}

	const ptr (: this) function = lambda (: this) {
		// Statements
	};

The interesting thing is, making methods constants, then we have a very 'pure' model,
meaning that the approach is minimal. We can idealize the signature-first syntax
as syntactic sugar :).

	sub::TypeA {a, aa}, sub::TypeB b;
	32u length = 0;

	(: this) constructor {
		// Statements
	}

Yeah, I think that's about right.

	try {
		32ue a = 32ue~max + 1;
	} catch {
		NUMERIC_OVERFLOW
			sml::out << "overflown integer!";
	}

I think I can roll with this. The syntactic sugar, yeah. Looks fine. Just need to
polish the semantics a little and the language will be done. Especially the enumerations.
Those are important. So an enumeration is associated with some specific class. What
about primitives? I've though about this a bit. I think it's best too keep primitives
just the basic units (integers, floats), and add arrays to the mix. Pointers are
also included. So if you have a super awesome custom vector, then just return the
pointer to the array with the size, or turn it into a suitable array. Hell you can
even return a nested array. Should be easy. Yeah that's feasible.

I've been thinking about purity. It's nice to have, absolutely. The problem in making
hack impure is that we can't do awesome floating point optimizations in some cases.
For example, the fast inverse square root may want to use hack in order to speed
up the computation. In addition, the code is already 'pure' since things are explicitly
granted to the lower class.

	(:) enter {
		32u value;
		hack("asm
			movl $10, %eax;
			movl $20, %ebx;
			addl %ebx, %eax;
			out: value, %eax"
		);
		print value;
	}

Maybe we just need the most basic of io, just printing, errorring, and file handling.
It is desirable to be able to do some assembly programming deeper down. Also, the
grants kinda destroy some of the purity that was thought of. In C++ you can include
whatever you want and it will work. I'm thinking that's not in line with Demeter's
laws. How is this solved? Knowing that an algorithm is pure is a very nice thing.

I just want to see a library with a top class being 'pure'. That would be very comforting.
It lets me know that this library, when given the same input, never diverges from
that input. This is why I believe bounce handlers should be given. No. Instead, let's
make impure grants be callable from pure code! Genius? Let's examine.

	(:) enter {
		name::Class INVERSE;
		sub::Class[fast: INVERSE::fsqrt] myclass;
		myclass.run();
	}

That looks actually pretty neat. You inject impure code where it is requested. Not
only does this keep the impurity at the top level, it also allows you to inject optimizations
and other niceties. As long as the subclass doesn't use hack directly, it's fine.
It's just like going out to enter, getting the result, and then inputting that result
back into the class.

This means that the subtree is pure, it doesn't use anything machine-dependent, meaning
that cutting the subtree off and using it elsewhere should always work. I think that's
it. Are we done? Defaults of course. So the [] allows us to override definitions?
Seems like something nice. I mean, if it's desirable then sure, why not? Meaning
that not using the fast inverse square root will just fallback on the pure algorithm.
I like that idea. It's very clean. Hmm, you could basically inject anything then.

	private (:) cool {
		sml::out << "Hey!";
	}

	(:) enter {
		sub::Class[myFunction: cool] a;
		a.myFunction();
	}

That would be scary, I suppose it's best to make any grants only accessible inside
that class. If you want a reference, just create a function for it.

	project/
		Main.cxy
		code/
		impure/

Having projects like this basically makes code your free world. Impure code is separated
from pure code. I think this will work out very nicely. The compiler also needs to
grant something to Main:

	(:) compilerEnter {
		project::Enter[File: internal::File] ENTER;
		ENTER.enter();
	}

We could idealize the output stream as a file too.

	(:) enter {
		Io a(Io::STDOUT_FLAG);
		a << "output";
		Io b(Io::STDIN_FLAG);
		b.readLine();
	}

Wait, are grants of Io downward pure? Io can just be labeled impure. But does this
make printing and file operations in granted classes 'pure'? Well yes! The hacks
were granted. I guess enter can also be pure then! So long it doesn't have hack in
its hierarchy, all is well. This seems so clean. I love it! Io is a granted class
then. Nice. Granted by the compiler for a specific machine. This makes the writers
of the compiler only liable for a tiny IO library. That's it. The Io library should
support stdout, stdin, and files. stdout/in are files, essentially.
We just define a simple io library and we're done. Cool.

So to finalize the grammar,... let's bring back what was written before. A final
grammar. Let's do it.

	// Class-Scope
	CLASS ::= { [ ACCESS ] ( DATA | ENUM | METHOD ) } ;
	ACCESS ::= 'private' | 'public' | 'restricted' ;
	DATA ::=
		{ TYPE (
		        name [ ':' name | '=' EXPR | ARG ]
		        | '{' { name [ ':' name | '=' EXPR | ARG ] }+ \{ ',' } '}'
		       )
		}+ \{ ',' } [ ',' ] [ '=' EXPR ] ';' ;
	ENUM ::= { enum  ['=' EXPR] }+ \{ ',' } [ ',' ] ';' ;
	METHOD ::= SIGNATURE name STATEMENT ;
	SIGNATURE ::= '('
	                (
	                 { 'this' | TYPE (
	                                  name
	                                  | '{' { name }+ \{ ',' } '}'
	                                 )
	                 } \{ ',' } [ ',' ]
	                )
	                 ':' $1 [ ':' [ 'const' ] [ 'pure' ] ]
	              ')' ;
	ARG ::= '(' { EXPR [ ':' EXPR ]
	            | '{' { name ':' name }* \{ ',' } '}' ':' EXPR
	            }* \{ ',' } [ ',' ]
	        ')'

	// Type and Expressions
	TYPE ::=
		[ TYPE ] 'type' '[' name ']' | 'ptr' TYPE | 'ref' TYPE | 'const' TYPE
		| '[' { EXPR }+ \{ ',' } ',' TYPE ']'
		| 'enum' '[' TYPE ']
		| typename '[' { TYPE [ ':' TYPE ] | EXPR [ ':' EXPR ] } ']'
		| primitive ;                     // 32u, 8ue, 16so, 3sb, 32f
	STATEMENT ::=
		'if' '(' EXPR ')' STATEMENT
		| '{' { STATEMENT } '}'
		| 'goto' name ';'
		| 'label' name ';'
		| 'try' STATEMENT 'catch' '{' { STATEMENT } { enum STATEMENT } '}'
		| '++' EXPR ';' | '--' EXPR ';' | 'hack' '(' STR ')' ';'
		| ;
	FEXPR ::=
		{ EXPR [ [EXPR] ':' name | '{' { name [ ':' name ] } '}' ] } \{ ',' } [ '=' EXPR ] ';' ;
	EXPR ::= // Precedence decreasing
		EXPR '.' name
		| EXPR ( '+' | '-' ) EXPR
		| EXPR ( '*' | '/' | '\' ) EXPR
		| EXPR '..' EXPR ;

So ideally, we'd have everything pre-determined by using an LL(1) lookahead, instead
of determining things after-the-fact. Maybe ':' needs to be brought back. We aren't
an LR(1) parser! Ech! Post-processing. Post processing fixes it. A current problem
is that type expressions with type~something aren't processed correctly. Man that
automatic passing mechanism gives a lot of butthurt. It's just annoying to work with.
Let's say you can't use types directly in expressions, this means that whatever starts
with a type becomes a valid assignment. I'm considering not allowing field-initializers
since I'm afraid they can cause confusion. Say you have a variable set to 1, but
the constructor says to construct with 0. What is used? How does it relate to other
operations and impure operations? Let's just say the constructors must not branch
and will simply create a new object. In the beginning, the object is not constructed
at all. This allows us to save computing time. The nice thing about field initializers
is that they're so well understood, languages use them. People love them. They're
fast and easy. What about the ambiguity:

	a, b = f();

We could disallow the = per unit. I really don't want the := operator in the language.
My solution is to not allow combinations of ':' and '='. The method of using:

	EXPR '=>' { TYPE name [ ':' name] | EXPR } ';'

Could be interesting, but we get the same problem with EXPR/type confusion. This
is the core of the issue. EXPR can have the same FIRST set as TYPE. How is this solved?
One could have a universal type prefix for types inside exprs.

	type[[32, 64se]]~
	::[32, 5, 1, 8u]~size + ...

I like the use of '::'

	::MyClass::coolFunction(300);
	::Io::write(::MyClass::coolFunction(300));
	::name::Space~size + 9;

Some kind of element that differentiates the FIRST sets. '::' is rather large. It
can be shortened to '?', or some other token.

	<MyClass>~size
	<name::Space[32u]>

I kinda like that. You're never going to nest these, the only trouble are the <>
operators, these will clash. Yeah I can just say that types aren't allowed in expressions
and that everything must be an object in an expression. The initial solution. I really
wish to be able to separate types from expressions. That would be beautiful. The
problem is just having this FIRST set being incompatible. Ugh,..

Now ARG is clashing with the lists {}.... I really need to revise the grammar so
it doesn't have these lookahead clashes. Hopefully the grammar can be augmented.
It's the idea that matters (the design of the language). Not so much the syntax.
At the heart of the issue lie expressions and types. Types can be a part of an expression,
types may include expressions (arrays, grants). Expressions may include types. This
leads to a clash. Yeah I think the solution is just to remove types from expressions
altogether. Let's just use CONSTEXPR objects instead.

	auto x = f();
	32u a = x~a, b = x~d;
	type[a] A;
	A + 3;  // Not a type because of the CONSTEXPR

Now the remaining conflict is the {} and {} list in arguments. Using [] would clash
with arrays:

	[ ...  // Array declaration or expression?
	array[32u, size: 10]  // Clear as day.
	[] 100 20 [] 5 32u a;
	[10, 32u] a = array { 1 3 9, 9 };

I like that last one. It's a special operator of sorts. Why not allow {}? Because
it clashes with scope. Maybe expressions should not be allowed all alone on a line.
Maybe var is the true solution to all problems. But it does not justify extraction.

	= f() =>
		Type {a, b}, c: d;
	[3, 32u] a = {1 2 3};

I really wish to make all operations easy to parse by using an appropriate lookahead.

	var 32u d;
	var {Type {a: x, b: y}, my::Class c: z}, d = f();

This kinda does var a favor. By forcing var, we're giving a strong enough hint. So
we could just idealize using that.

	10u a, b(130), c;
	a, b, c = f();  // { EXPR }+ \{ ',' } [ '=' EXPR ] ;
	a: derp, b: out, c: control = f();  // { EXPR [ ':' name ] }+ \{ ',' } [ '=' EXPR ] ;

I really like that last one. It's so clean. It also filters initialization from actual
expressions (assignment). By disallowing '=',... we could still salvage the fexpr:

	Type {a, b: c}, d: y, 10u e: x = f();

I think that's quite hard to read. I'm skilled in reading Cxy but I find this hard
to decode

	Type {a, b: c},
		d: y,
		10u e: x
		= f();

No, I don't like this at all.

	f() {
		Type a = control;
		32u b = out;
		c = d;
	};

What about that extractor syntax? Just ... no? `EXPR '{' { STAT } '}'`. Nah man.
The extraction syntax is very much liked:

	x[3 + i]: my_name, extractor.name: controller = name.getClassMember()~ground()~holdUp();

I really enjoy that syntax. It's just python but with names. I want to make the language
more simple. Especially with the variable declaration syntax as it is.

	Type mytype, yourtype(0.3), ourtype(399, count: 50);
	32u a(30);
	ptr 800u(new(20){123});

This would make the grammar a whole lot simpler. Also we can prefix types with '::'
to make them usable in expressions:

	::Type::CONSTANT  ...
	for (32ue i(0); i < ::name::Space[Aa]::CONSTANT; ++i) {
	}

To be honest, using i(0) feels awkward. Not being allowed to assign upon creation
feels slightly awkward. I think I can get used to it though. It's better practice.

	Type mytype;
	mytype = ::Type(0.3);

So what about arrays? `[` vs { vs (. array{} is something I like.

	#GRAMMAR

	// Class-Scope
	CLASS ::= { [ ACCESS ] ( DATA | METHOD ) } ;
	ACCESS ::= 'private' | 'public' | 'restricted' ;
	DATA ::= TYPE { name [ ARG ] }+ ';' ;
	METHOD ::= SIGNATURE mname STATEMENT ;
	SIGNATURE ::= '(' ( { DATA | 'this' ';' } ) ':' $1
		[ ':' [ 'const' ] [ 'ctor' ] [ 'dtor' ] [ 'pure' ] ] ')' ;
	ARG ::= '(' { FEXPR }* \{ ';' } ')' ;
	FEXPR ::= { EXPR [ ':' mname ] }+ \{ ',' } [ COMPOUND EXPR ] ;

	UNOP ::= '@'|'$'|'$$'|'--'|'++'|'!'|'!!' ;
	BINOP ::= '+'|'-'|'%'|'^'|'&'|'|'|'*'|'**'|'^^'|'&&'|'||'|'>='|'<='|'=='|'!='
		'>'|'<'|'/'|'\'|'<<'|'>>' ;
	COMPOUND ::= BINOP . '=' ;
	OP ::= 'op' '-' name | BINOP ;
	UN ::= 'un' '-' name ;

	EXPR ::= UNA_EXPR [ OP EXPR ] ;
	UNA_EXPR ::= { UN | UNOP } PAC_EXPR ;
	PAC_EXPR ::= [ '::' TYPE '::' ] MEM_EXPR ;
	MEM_EXPR ::= CALL_EXPR [ ( '~' | '.' | '->' ) MEM_EXPR ] ;
	CALL_EXPR ::= RES_EXPR TRAIL ;
	TRAIL ::= [ ( ARG | LIST ) TRAIL ] ;
	RES_EXPR ::= name | mname | '(' EXPR ')' | string | integer | float | LIST
		| 'lamda' [ '[' { name } ']' ] [ SIGNATURE ] '{' { STAT } '}'
		| 'cast' '[' TYPE ']' '(' EXPR ')' ;
	LIST ::= '[' { EXPR } ']' ;

	TYPE ::= const TYPECONST | TYPECONST ;
	TYPECONST ::= ( 'ref' | 'ptr' ) ( TYPE | SIGNATURE )
		| 'array' EXPR TYPE | [ pname '::' ] cname [ GRANT ] ;
	GRANT ::= '[' { ( TYPE | cname '=' TYPE | FEXPR ) ';' }+ ']'

	// Statements
	STAT ::=
		'goto' name EXPR ';' | 'label' name ';' | 'try' STAT | 'catch' STAT | 'raise' ename ';'
		| 'hack' '(' string ')' ';' | DATA ';' | FEXPR ';' | '{' { STAT } '}' ;

	// Lexer, names are given in lowercase.
	digit ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;
	integer ::= { digit }+
	primitive ::= integer ( 's' | 'se' | 'so' | 'u' | 'ue' | 'uo' )
	lower ::= 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z'
	upper ::= 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z'

Some of the changes that are planned. As can be seen, typedecls are very simple now.
What I don't like is that signatures may contain `Type {a, b, c}`. This is dissimilar
compared to the `DATA` clause.

	(Type a, b, c; 32u out; 32f e(0.34);  :  anon in, second; 8u cool;) doSomething {
		cool: out, out: something = passOn(anon);
	}

I like it! The only ugliness is that the colon is barely visible. Also this means
I can use the 'data' declaration inside the signature. Awesome! Oh, and guess what,
the constructor is included in that, so standard variables! Smooth. That's really
smooth man. I'd suggest just putting in two spaces around the ':' marker. To make
it clear where it is. That's easier to scan. Another idea I have is to let everything
you pass as a [] argument to classes - passthrough automatically, such that libraries
are easier to integrate. Nobody likes just passing around mindlessly. However, it's
still possible to overwrite the classes in the lower subclasses:

	Type my type from(0.9);  // Local type decl.
	32u control variable three(50) ABSOLUTE(10);  // Local and constexpr variable
	HAS_CONTROL NO_CONTROL LOST_CONTROL TOTAL_CONTROL(4);  // Enumeration

	(:: pure) enter {
		Io out(::Io::STDOUT_FLAG);
		out << "Simplify!";  // Simplify is of type `ptr 8u`, and is null-terminated.
		String myString("Simplify!");
		out << myString;  // Simplify is of type `String`, and is not null-terminated.
		out << myString * 2;
	}

	(32f out;  :  32f a b c x y z;) dotProduct {
		out = a*x + b*y + c*z;
	}

Looks pretty nice without the commas as well, maybe I'll keep that, and replace the
semicolons with commas? Maybe. No. This is beautiful. I absolutely love it. It looks
so clean and refreshing compared to C++. Just spaces. Easier to write, easier to
read, easier to organize. So arrays, what about them? `{}` is out of the question
as it clashes. `[` is out of the question, it clashes. `(` is out of the question,
it clashes. What's left? Any groupers? Anybody?

	[10, 8u] wow hey you;
	(:) enter {
		wow = ('a' 'b' 'c');
		hey = ('h' 'e' 80 10+48*3 0-3*8);
		you = (0-3 0-6 0-9);
	}

Only thing I dislike is that - expressions clash with the unary minus operator. It's
still pretty darn clean though. I like that.

	EXPR ::= '(' { EXPR }+ ')' ;

Becomes a rule. Maybe '--' can be a prefix operator. But that mixes badly with the
iterative -- already. `%-` can be seen as the 'blocking negation'.

	[10, 8u] wow hey you;
	(:) enter {
		wow = ('a' 'b' 'c');
		hey = ('h' 'e' !+80 !-10+48*3 !-3*8);
		you = (0-3 0-6 0-9);
		wow = 'a' 'b' 30 0 !-3;
	}

Using % is kinda ugly though. Maybe \ will do. Hey! It's also an escape sequence?
But it doesn't do well for expressions, because we expect a binary operator. Perhaps
something like # could be used. I like ! though. It also gives a nice delimiting
feel to the expressions.

	(: ptr 8u ar; 32u length; 8u in) doSomething {
		// do something
	}

	(:) enter {
		doSomething(ar, length: length = 1 2 3 4 5 6 7 8; 8+9;);
		32f a, b;
		a, b: error = sin(0.321;);
		ptr 32u a; 32u l;
		a, l: length = 1 2 9 7 !-2384 'd';
		for (32ue i(0); i < l; ++i)
			::Io << a[l];
	}

Using FEXPR inside arguments... I don't know anymore. It's ... beautiful.

	[10 32u] a(1 2 3 4 A+B f() c()~d+3/GNUM ::math::exp(::math::EULER, power: 3) 0 0);

I kinda like it. It's easy to type. I suppose it's not recommended to put several
big expressions inside. Also what about the array size type? Use zero for automatic
detection? Or just leave out the number altogether?

	[32u] a ( 1 2 3 );  // Size 3.
	(:: pure) enter {
		a[1], a[2]: second = f();
	}

The comma in the expression list is to avoid writing an array (since that's a valid
expression). I think we're pretty far now. FEXPR inside arguments. I like it! This
will be so clean holy shit. I think I need to write some more example code in it
though:

	public (113ue out : 5ue in) factorial {
		out = in;
		--in;
		while (in > 1)
			out *= in;
	}

	private (String out : 113u in) toString {
		out = ::String::parse(cast[128u](in));
	}

	private (32ue out : Array[32ue] in) sum {

		out = 0;
		Array[32ue] partial_sum(size=in.size);
		partial_sum[0] = 0;
		for (32ue counter(0); counter < in.size; ++counter) {
			out += in[counter];
			if (counter > 0)
				partial_sum[counter] = partial_sum[counter - 1] + in[counter];
		}

	}

	(:) enter {
		try {
			sml.out << factorial(30);
			debug "what's going on man";
		} catch {
			::Sml::INTEGER_OVERFLOW {
			}
		}
	}

Where sml is a granted object. Since grants are automatically down-propagated, there
is no problem in just using this all over the place. So we got everything that's
needed for a practical language! Good. Though, I find it kinda verbose to be honest.
Maybe I should compare code to the C++ equivalent or Python equiv.

	# The above
	fun factorial(n):
		m = n
		n -= 1
		while (n > 1)
			m *= n
		return m

	fun sum(arr):
		out = 0
		arr = [0] * len(arr)
		partial_sum = [0] * len(arr)
		for i in range(0, len(arr)):
			out += arr[i]
			if i > 0:
				partial_sum[i] = partial_sum[i-1] + arr[i]
		return out, partial_sum

My code is somewhat bigger in size. It's somewhat more verbose. Then again, it has
types. Types add a lot of information. Now to polish the grammar. We have statements,
these are independent of the 'core' of the language, which only has 3 statements:

	STAT ::= FEXPR ';' ;
		| 'label' name ';'
		| 'goto' name EXPR ';'

Where the expr in goto is an integer or boolean result. This allows all kinds of
things like loops to exist. So the core of the language is really:

	Data declaration + Granting
	Method declaration
	Assignment/FEXPRs
	Gotos

That's pretty minimal if you ask me. I just need to work a little on the FEXPRs.
I really like them, that's for sure. Now let's look at function calls:

	object.myFunctionName(name="do whatever you want"; size=3; control, inside: out = cool());

That looks really smooth actually. Really smooth. I like it. What about default arguments?
These tend to look like:

	(: 32u a(1023) b(40), ptr Type c(new)) doSomething { ... }

Now, what's unnerving is that the initialization is inside the type of the function.
I'd really like to have it on the outside, but it gives us such useful information
about the function. In addition, it's really useful when calling a function and you
just want to add a single parameter, suppose I start with the following:

	(32f out : (32f out : 32f in) in, 32f from, to ) integrate {
		...
	}

Which is called using

	integrate(
	          lambda (32f out : 32f in) { out = ::Math::exp(!-::Math::pow(in, power: 2)); };
	          from=0;
	          to=10
	         );

But one day we want to add a parameter which is the following

	(32f out : (32f out : 32f in) in, 32f from, to, enum[Math] method) integrate {
		...
	}

Now you can't use the function because method has to be given. Instead, this is desirable:


	(32f out : (32f out : 32f in) in, 32f from, to,
		enum[Math] method(::Math::SIMPSONS_METHOD)) integrate {
		...
	}

This makes the default argument be simpsons method (it's a method to numerically
integrate a function. I really like the idea of having this ultra simple unifying
syntax. The thickness of it! Maybe overloads will do though. How will casts affect
overloads?

	ptr (: 32u takes, these, arguments; 32f with, these, names)
		a(cast[(: ...)](myObj.memberFun));

Where the cast expansion ... is the same as in the signature of a. Should a be auto?
My intuition says 'yes!'. How do I formalize the method of automated casting for
initializers? Any initializer will cast to the appropriate type (or try to). Problem
with that is that I could assign an integer to the type and it would fail at call
time. Difficult decisions... What if we don't allow overloading? What if we just
say that each function has only one signature. But, you can of course avoid setting
an argument. This means that the argument wasn't passed. The function can use that
information to specialize certain cases. This does however bar us from using different
types for the function signature. That's rather sad, I'd love to see the following:

	(: 32f in) name ...
	(: 64f in) name ...

Just depending on the in parameter. I like the idea of overloading. Just gotta keep
in mind how casting affects it. So overloading is fine. Is it sufficiently elegant?
Anyways, as we know, the top-level class-signature is syntactic sugar for just declaring
a CONSTEXPR member variable to the object. That's the equivalent of what it does.
Granting is a powerful system as well. You can't really fight it. I'm concerned about
the manageability though. The thing is, sometimes you won't really know where a class
comes from. It's just there. You know it doesn't come from directly above, it may
come from multiple levels up. You gotta scout the class out. Rather annoying actually.
What about constructor semantics? I'd like to see that come to fruition. Just call

	MyClass myobject;
	myobject.construct();  // Actually initialize the object

This will require no 'construct' syntax or anything similar. I like it. It's simple.
But what about the destruction? Will destruction always take place even if the object
has not been constructed? That's rather antiintuitive. An explicit destructor goes
against the point of having a destructor at all. I think it's fair to say that all
new objects call the default constructor. What about const-initialization in the
constructor though? Well, we could do like Java does and just leave that the constructor
free to change any const member. Also, do we need to call all composite constructors
before the constructor starts? What if we want to construct them with something interesting?
Say a const or something like that. How is that done? Do we just need to re-call
the appropriate constructor on it? Wouldn't that be a waste?

The whole language is syntactic sugar for a class-definition language with lambdas.
That's the deal. DDL, Data-Definition Language.

	DATA ::= TYPE { name [ ARG ] }+ ';' ;

Simple. Then we have the MDL, Method-Definition Language.

	METHOD ::= SIGNATURE mname STATEMENT ;

Also quite easy to grasp. Really, I just need to define the semantics of enumerations,
constructors and various other entities. Going back to constructors. The default
constructor is always called for a local object (or new). For PODs, it'll just default
to a garbage value. Shouldn't be a problem. Ok. Now, the body of the constructor.
Do we allow 'construct' statements? I think it's fine. As long as they're not in
branches. In a sense C++ has the right idea about initializing _all_ variables in
the beginning. This means that you won't use initialized variables in the constructor.
Although I reckon it's easy to catch, if we use anything involving that name before
its 'construct' invocation, then we have an error. One option is to simply say we
individually 'init' every composition.

	Array[32u] a(size: 10);
	(: this; 32u newsize(10)) construct {
		a.resize(newsize);
	}

This is a vastly simplified model. Does not require special semantics or syntax.
Well except for allowing setting of references and consts inside the constructor.
A possibility is to call whatever method or set whatever whilst being a const method.
We could introduce the 'ctor' word:

	Type member(0);
	(: this : ctor) {
		member.setConstMember(1000);
	}

With that, a 'create' function can only be called by a data declaration or by another
create function. This allows cascading downward in compositional problems. I suppose
the dtor doesn't need this functionality. The dtor can just look at its own class
and decide. Perhaps it can do some internal cleanup, but that should not be necessary.
The dtor does need to be called implicitly though. 'delete' won't do, but using a
method might.

	3912u a(cast[3192u](::Object()));

Maybe we have a big class and just cast it into an unsigned. Not really sure if this
is possible though. I suppose the 'dtor' tag could be introduced. I suppose a constructor
'returns' an object, although it more so builds it in-place. I guess the ctor has
an implicit this on both in and outputs. So using a 'dtor' tag just gets called at
the end of an object's lifetime. Objects casted into, say, unsigneds, won't destruct
at all. That is, if the bits are totally exchanged. Or are they? Since interchanging
them essentially means to completely destroy the old object. If a ctor method has
a name, then it is not a constructor, but it needs to be called by a ctor method.

So we've got everything I think. const-member assignment... wait. Why not just call
the ctor and dtor 'construct' and 'destruct'? This would mean that the ctor and dtors
are callable from anywhere. Full freedom. This brings a dilemma though. Where does
the chain start? I guess the var decl invokes. Then, that ctor can call different
ctors. That seems reasonable I suppose.

What about enumerations? Do we use ::Class::enum or something to denote the type?
I like `enum[ClassName]`. I suppose each class has its own 'signals' that it can
throw. Or it can throw any other signal. But I think it's preferred if a class throws
its own signals. Should we allow throwing others' signals?

I really like the simplified language. It gives me a kind of peace that I did not
have before. It's really nice... I just need to internalize the language as it is.

	Type a b c(damage=10) d(enchanted=::Type::RARE_ITEM);
	ptr Array[32u] e(new{in, size: size=1 3 5 7 9 11});
	a: control, b = myFunction();
	try {
		b.runTests(c);
	} catch {
		::Type::RARE_ITEM {
			b.increment();
			c.strengthen(2);
			retry;  // Jump back to the start of the try block
		}
	}

	(:) enter {
		::Io << "This is a message";
	}

One thing that does annoy me is that there is no way of getting a ~size from an array
expression. Should be possible with parentheses. I suppose if I want an empty double
array I'd just write

	(() ())~size

Where we expect the outer array to contain only one element, and be castable from
and to the expected types. Maybe C did it right. It doesn't try and protect you using
consts constructors like I have. I like ctors and dtors and all, but it seems to
be a waste to spend too many resources on something. I like to see the current enums
as 'signals'. We're back at 'undeclared enums' again. I'm thinking let every class
throw what it wants, and it'll be seen as a signal. What about enums? Enums are very
useful, but I think they deserve a different spot in the language. We can use constants
for enums. The problem with that is that there is no designated enum type then. There
are only integers. I want to make coding in this go as buttery-smooth as possible.
Not having to declare the signal is pretty nice. Of course, if you want to raise
a different signal you'll have to trigger that signal. Probably by going into a class
and just raising it from there. Suppose we don't have enums, only signals. What now?
What's left to add?

Enums? Enums are so incredibly useful for programmers. I really enjoy them.

	(:) myMethod {
		raise CAN_NOT_CONTROL_THIS_AWESOME;
		raise I_JUST_CREATED_A_NEW_SIGNAL_NOTHING_YOU_CAN_DO;
	}

It's an interesting concept, but it removes enums from the equation. I'm thinking
a code generator is perfect for an enum. Just imagine the following:

	#Enum(VALUE NAME NAME2 NAME3 NAME4 NAME5)

Just expands into:

	private 3u value;
	public This VALUE(0) NAME(1) NAME2(2) NAME3(3) NAME4(4) NAME5(5);

	private (: 3u in : ctor pure) constructor {
		value = in;
	}

	public (: This in : ctor pure) constructor {
		value = in.value;
	}

	(1u out : this; This rhs : const pure) == {
		out = this.value == rhs.value;
	}

	(String out : this : const pure) name {
		switch (this.value) {
			case 0: out = "VALUE"; break;
			case 1: out = "NAME"; break;
			case 2: out = "NAME2"; break;
			case 3: out = "NAME3"; break;
			case 4: out = "NAME4"; break;
			case 5: out = "NAME5"; break;
		}
	}

	(: this : pure) setVALUE ...

Etc. Should be working. I just wanna clean up the grammar a bit. We've got a pretty
simple language with few productions. I like the simplicity of it. Although some
of the semantics irk me just slightly.

	def x(32u a b c  :  32f d e f)
		sml.out << "Control message" + d + e + f;

We could create a 'macro' like this:

	def <a> <sig>
		<statement>

Gets turned into

	const ptr const <sig> <a> = lambda [@] <sig> <statement> ;

I like the idea. But I also like the cleanliness of the former method of just writing

	<sig> <a> <statement>


	Set[String] set;
	set.add("cat");
	set.add("dog");
	set.add("human");
	if set.has("cat")
		::Io << "set has the cat element";

Hmm... because expressions can't really spill into anything, we can just avoid the
() around ifs at all. They can be completely optional. I like that.

	while 1 > 4 and set.has("cat")~out
		debug "I think I may prefer 'and' over '&&'";

I do enjoy 'and' and 'or'. They're far easier to write as well.

	if a and b and c and d
		::Io << "cool!";

The problem with an expression like this is that array expressions must be parenthesized.

	if 1 2 3
		::Io << "what";

That's a problem, so how can it be resolved? Perhaps it's best to just add an array
operator of some sort.

	call(1 2 3);
	if 1 2 3
		call(1 2 3);

	if #(#(1 2 3) #(4 5 6) #(7 8 9))
		::Io << "I think [] can be much nicer actually.";
	::[::[ 1 2 3 ] ::[ 4 5 6 ]];

I just want to remove the :: from them though. Maybe we need to rethink the entire
array declaration syntax. Perhaps it's best to just write:

	Array[32u, 10] a([1 2 3]);
	Array[Array[32u, 10], 10] b([[1 2 3 4 5 6 7 8 9 10] [...] ...]);

What I like about this is that it allows us to write the [] freely in expressions.

	if 1 2 3
		debug `won't work because 2 and 3 are taken to be a statement, which is syntactically
		flawed.`

	ptr 32u a; 32u len;
	a, len: length = [1 2 3 8 9+3 getSome()];
	[10; 32u] c;

So we don't allow cross-linking operators, which prevents the need for a comma. What
I mean is that we don't allow dual unary and binary operators. This allows us to
write lists without using commas. Something I find very pleasant. There's just this
clash between actual data declaration and type specification.

	array 100 array 10 MyType a b c d;

I like 'array'. It assumes that arrays are of the form: 'number type' This simplifies
the array type declaration significantly. What can also be done is to allow any number
of expressions to be under array:

	array 10 3 10 MyType a;

This allows you to define multidimensional arrays.

	array[10 30 10 ptr MyType]

The above looks too much like a grant. I'd rather avoid it. So suppose we use this
syntax for arrays. Then we can use [] anywhere else

	myCall(in=[1 2 3]);
	myCall(in, length: size=[1 2 3]);

Wonderful. Is this it? We have arrays, we have ::Type to avoid expr and statement
overlap. What else is there to do? The thing is though, var would prevent the need
for both of these.

	var [8 32u] a([1 2 3 4 5 6 7 8]);
	[1 2 3] + [3 4 5];
	array 8 32u a([1 2 3 4 5 6 7 8]);

We also need an automated cast from the array to an unnumbered ptr.

	ptr 32u a; 32u len;
	a, len: length = [18 19 !-32 !+39 !+get()];
	32u sum(0);
	for (32ue c(0); c < len; ++c)
		sum += a[c];

Is size inference a desirable thing? Perhaps not. Perhaps it is. Maybe we should
just use ptr if the size is indeterminate. The above syntax is actually pretty nice
isn't it? It's short and not that complicated. Contrary to previous endeavours. The
current paradigm is for simplicity. Oh, and for a case where we want to manually
call the ctor/dtor:

	ptr 1u a(new(::MyType~size));
	cast[ptr MyType](a)->construct();
	// ...
	cast[ptr MyType](a)->destruct();

This can be useful, when we create some kind of factory pattern. There is a collision
between the :: and the expression syntax though. Take for example:

	[::A::b() ::C::d()]

How do we know :: doesn't bind to the previous element? We could enfore is in the
grammar by making :: Type just like that, since type only contains :: internally,
it won't clash with other elements.

	32u a(2) b(1);
	32u c(::32u::+(this: $$a; right: $$b));

We could also say:

	@Type::CPTR;

But would that fall under @() or @(Type)::CTPR? I'd prefer the latter, but it mixes
badly with the former. In that case, @ needs a higher precedence than ::. Also,
. needs a higher precedence than @, which makes . a higher precedence than ::. Isn't
that odd?

	@MyType::A.b.c()
	(@MyType)::A.b.c()

No man I think @ will just be confusing to people. :: also nicely encloses types:

	::ptr const ptr array 100 32u::SIZE
	::namespace::Type[Control; name="Raven"; EDGE=23]::SIZE

I really like that one. It acts as a delimiter.

	debug ::String::getInfo();

I really like that one. Okay, so are we do I agree with myself on the array syntax?

	if ::array 1000 32u::SIZE == 32000
		debug "Test correct";

This means that arrays are created by writing []. Arrays are declared by writing
the keyword 'array'. How can we cast arrays to run-time objects?

	array 32u a( [1 2 3 exp expr() intef()~g] );
	array 6 32u a( [1 2 3 exp expr() intef()~g] );

What's the difference? The former stores the size of the array. a.length is possible
to use. The second type is a definitely-static array. It will always have the same
size. I guess we can allow partial array setting:

	array 10 32u a([1 2 3 4]);
	sml.out << a[4];  // Undefined behaviour, as a[4] is not set yet.

The array can't grow. We could make array an object. The thing is, it'd be best if
it were a class. The problem is we need a basic type for [] to use. I like the array
syntax, but maybe it's best to check out other languages.

	array 32u a([1 2 3]);  // Normal initializer
	array 32u a[1 2 3];  // Array-initializer
	MyType[] a = new MyType[123]();  // Java
	a = [1, 2, 3]  // Python
	a = [1 2 3];  // Matlab
	int a[] = {1, 2, 3};  // C++
	$a = array(1, 2, 3);  // PHP
	A = [1 2 3]  // Oz
	a = (1, 2, 3);  // PERL
	(make-array '(1 2 3))

I kinda like the [] start. It's shorter than array and easy to type []. array is
probably fine too. I suppose we can poll the type of a after creation to check the
size. The size is static so it should be fine. Can we pass an array into a method?
An array is a pointer to the start of a set of elements. It contains size data as
well (static, compile-time). We can't always know the length of the array given to
a method. So a method assumes the length zero.

	(: array 32u a) control {
		for (32ue i(0); i < ::type[a]::LENGTH; ++i) {
			sml.out << a[i];
		}
	}
	(:) enter {
		control([1 2 3]);
	}

Should print by using the size 0. An uninitialized array is just an empty array.
It's a pointer. array 32u = ptr 32u. What about signatures?

	(: 32u a(10) b c(0)) fun { ... }

	(:) enter {
		ptr (: 32u a b) a(fun);  // assumes argument c is automatic (zero).
	}

In this case, the function overload is not called, but the default argument is cast
away. Maybe default arguments should be a thing, and not overloading. I like that
idea. The cool thing about it is that static code analysis can remove code that isn't
called anyways.

	(: 1u a b c(true) d(false) e(true)) something {
		if a
			...;
		if c
			...;
	}

The nice thing is that the function `(: 1u a b)` casted from the above will remove
the if c clause completely and just run the code. What is the type of an overload
though? Well not an overload but more so standard params. This also makes the entry
point unambiguous. What about types though? Different type overloads would be necessary
I think... Man this is hard. I realize that methods like print may benefit from overloading,
but on closer inspection we can just write "print(mynumber.toString())". Since you
can only print strings. What about args of the same name?

	(: this; 32u in; 32f in) add {
		this.sum += cast[32u](in);
	}

I don't know how this is supposed to work. Code generators?

	#print(a);

Just gets turned into:

	print((a).toString);

Or basically:

	#print(a + b);

is

	sml.out << (a+b).toString();

I'm not sure if overloading is the way to go. C doesn't use it and that language
does pretty fine. Overloading can be extremely useful, suppose I have a class called
matrix and I want to either multiply by a vector or a constant. Both have different
semantics but it's still a multiplication. My main concern is that it'll be hard
to find the overloaded function, because you need to look at all the types of the
inputs. Maybe it brings more trouble than it solves. Hell you can still 'overload':

	(: 1u a() b() c(); Type d() e() f()) myFun {
		if d.is_default {
			// Do stuff without d, e, and f
		} else {
			// Do stuff with d, e, and f
		}
	}

	(:) enter {
		myFun(a=1; b=0; c=false);
		myFun(d=::Type(0.3); e=::Type(0); f=::Type(0.6));
	}

The nice thing is that static checking can remove the ifs, so no real value is lost.
This does not apply to operators though. I'd love to have operator overloading for
operators. Maybe operators should just keep to their own types. Where it makes sense.

	(Type out : const this; ref const Type in : const) + {
		32u partial_sum = this.computeTotal() + in.computeTotal();
		out.setCumulativeSum(partial_sum);
	}

I like the idea. Keeps the language clean. No need to abuse operators. Plus the method
names hint what's going on:

	datatype.addVector(vectorname);
	datatype.addSet(mys);
	// Alternatively
	datatype.add(set=mys);
	datatype.add(vector=myv);

I think this can work. I think I'm almost done with the entire language. Is there
anything else? I've worked through arrays, overloading, expressions, grants,....
I could inspect statements. As of now, the only statements are fexpr and datadecl.
The core of the statements have been added. It's actually really simple. ifgoto,
and labels. Mix in the FEXPRs and DATA declarations, and the language is complete.
Also added the scope. So you can have multiple consecutive statements. I'm wondering
if lambdas are really necessary. They're big and bloaty. They do stay confined to
a single method though. What I mean is that if they are defined therein, you know
that the lambda is used in that method.

	ref (64ue out(0) : array 64u in; 64u length) sum(lambda {
		for (64u i(0); i < cast[64u](length); ++i) {
			out += in[i];
		}
	});
	sml.out << sum(in, length: length=[1 2 9 8 4 3 6 7]).toString();
	// Prints "40"

Could be kinda useful. Maybe I should write a full grammar. But am I satisfied? Is
everything in-place? Am I missing anything? We've got construction, destruction,
purity, const-correctness, references, ptrs, const variables, arrays, type and size
of a type, signals, hacks, loop constructs, objects, default parameters. CONSTEXPRs,
no enums... We can add some conveniences to the language. While, for, static ifs.
Wait, what kind of statements does C++ have? Expressions, declarations, calls (part
of expressions), for/while/do, switch, return, break, continue, label.

I just noticed that I'm retarded. I haven't noticed this before. The right-recursion
for most of the expressions are actually forcing right-associativity. This is annoying.
Luckily, it's nothing that tree-reordering can't solve! The algorithm for reordering
summing negatives is as follows:

	1) Make the end of the summation chain the root.
	2) Make all right children the parent
	3) Make all right children's right node the current right node

	-
		1
		-
			2
			3

Is a tree for 1 - 2 - 3, it must become

	-
		3
		-
			1
			2

Likewise for 1-2-3-4-5+6-7+9-1

	-
	1 -
	  2 -
	    3 -
			  4 +
				  5 -
					  6 +
							7 -
								9 1

It's annoying that this happened. Oh well. I almost feel like going full LISP-style
again.

	a, b: c = d();
	a += 34;
	b.something(a: b);
	a.+(b).*(3)

This would get parsed very easily, because exprs would be reduced to very simple

	memberexpr = member [ '.' memberexpr ]

It's just not practical. So how can left-associativity be handled? It can be observed
that the tree can be evaluated top-to-bottom wise. This will create a stack. However,
it doesn't represent the correct semantics. Is it possible to find the correct tree
after parsing with an LL(1) parser?

	*[+[1 2 3 -[8]] 100]  // -2 * 100

	(:) enter {
		32u a(+[100 1 2 3 4 5]);
		a = +(1 2);
		a +=
	}

What if we only allow compound operators?

	(:) enter {
		a, b = f();
		b = !-100;
		b += g();
		a -= b;
		// a, b = f();
		// a -= !-100 + g();
	}

That's actually really nasty. Maybe longer variable names are more useful. It also
muddies the distinction between '.' expressions and other expressions.

	(:) enter {
		32u sum(0);
		sum += f(10);
		sum += ((1-3)+2);
		sum += 1.-(3).+(2)
	}

I feel lost. Maybe using them as members is the best thing. Associativity will be
rather destroyed though. A right child of the same order, that child must become
the parent. The child's left child, will become the new child's right child. Tested
it out for a bit. Seems to work... Maybe expressions are safe. I think they are.
I just require post-processing of the parse tree. Well that was easy. What's left?
Maybe there's a collision with lists and actual expressions. You see:

	[a [b]];  // Clash of a[b] with a [b]
	[[1] [2]];

Maybe it's not severe, but imagine a function returning an array, and suddenly you
get `[a() [1 2]]`. That should fail catastrophically. The parser doesn't take into
account that this may happen. How is this solved? The same goes for function calls
`[a() (b+c)]`. What is to say a doesn't return a function? Then b+c would be an argument
into that returned function. This is obviously a problem. While I like space delimited
arrays, maybe it's time to bring out the comma again. [1,3,5,7,3,6]. Maybe it's just
fine. `[a(),(3+5),19]`. I like this. How can array access be made recursive?

	AR ::= RES AR
	RES ::= name
	AR ::= [ '[' EXPR ']' AR ]

	>-
	a <= b;
	a = a < b;
	a leq b;
	a geq b;

Just messing around.. How do I want to code?

	(1u out intersects : const ptr Polygon left right; ptr Axis axis : const pure) findCollision {
		Vector mainvector(computeSupport(left, axis: axis) -
			computeSupport(right, axis: -axis));
		Simplex simplex($mainvector);
		Vector inverse(-mainvector);
		while true {
			mainvector = computeSupport(left, axis: inverse) -
				computeSupport(right, axis: -inverse);
			if dot(left: mainvector, right: inverse) < 0
				intersects = false;
			simplex.unionAdd(mainvector);
			simplex, inverse: inverse, out: origin = nearestSimplex(simplex);
			if out
				break;
		}
	}

I almost feel like expressions should just be removed. It's just something that can
be mistaken by humans. In addition, it's a whole lot of complexity for the entire
language added. At least the base of the language seems pretty solid. Not having
space delimited arrays makes the idea of having space-delimited type declarations
a little more foreign. `[1 2+4/6 3**4/2.control() 932]`. So much could be simplified
by using LISP-expressions.

	32u result(+ (obj (fun name:1 control:2 nothing:3)) 3);
	(= result 100);
	[10 ptr Type] a(new(10));
	result, a: checkpoint = (f name: "control" world: 15);
	hack("C++: std::cout << \"Hello er\"");
	if (< (abs (- x (* guess guess))) tol) {

	}

Expressions do become super simple then. Maybe only make expressions lisp-styled.
Hmm... That's interesting. Making only expressions lisp-styled kinda fits neatly
in the language now. Let's experiment a little more.

	(128ue out : 128ue in) factorial {
		out = in;
		--in;
		while (> in 1)
			out *= in;
	}

	(32f out : 32f in) qrsqrt {
		64s i;
		32f x2 y;
		const 32f treehalfs(1.5);
		x2 = (* in 0.5);
		y = in;
		i = (@ cast[ptr 64s]($y));
		i = (- 0x5f3759df (>> i 1));
		y = (@ cast[ptr 32f]($ i));
		y = (* y (- threehalfs (* x2 y y)));
		out = y;
	}

	(:) enter {
		This obj;
		(<< (sml.out) (obj.factorial in: 10)))
		sml.out << obj.factorial(10);
		(sml.out.<< "cool");
		(sml.out.<< (qrsqrt 12));
	}

There's something otherworldly about this. It's somewhat longer and more tedious.
But how else can precedence be written like this? The above code simply can't be
misinterpreted. The () exprs also fit nicely with the idea of constructors. The grammar
becomes `DATA ::= TYPE { name [ EXPR ] }+ ';'`. The expr simply is the constructor.
I'm not sure... It looks kinda neat. It also destroys all possible ambiguity. I like
that. So we keep all statements. It's just that expressions themselves are s-expressions.
So how can arrays be declared or indexed?

	(:) enter {
		[10 32u] a([] 1 2 3 4 5 6 7 8 9);
		(a.[] 9);
		(a.[] 3) = 100;
		(createWindow name: "My Window" xsize: 300 ysize: 200
			x: (/ view.xsize 2) y: (/ view.ysize 2));
	}

I like it. It does mix the dot operator with a lot of the other operators though.
Not a fan of that. We could say that all operators except the '.' operator are actual
operators. This should also count for the builtin $$, $, and @ operator. There is
precedence if we have `@a.b`. What is evaluated first? @a or a.b? For this, we need
to make one of them a function: (@ a.b). That looks way better. What about the dot
operator though? (. a b) ? That just doesn't look nice nor practical.

	(:) enter {
		32u a(> 3 1);
		a = (~ (factorial number: (+ 1 5)) derp);
		Type b(name: "five");
		Type c(Type::constructor in: 10 control: 0);
		Type[CONTROL: (+ 1 2)] d;
		signal OUT_OF_PROGRAM;
	}

I like the idea. It makes the LL(1) language complete. There is no further need for
tree post-processing either. Hell, it's not even necessary to prune that much. Expressions
used to be highly prunable due to precedence. Now it's a cakewalk.

	(:) enter {
		ptr (100ue out : 100ue in) factorial(lambda { out = when (== n 0) 1 else (factorial (- n 1))});
	}

How does it mix with the when expression? (((a == b) + 3) * 5240). Another possiblity
is to simply allow infix expressions, but not expand on their precedence without
any parentheses.

	(:) enter {
		32u a(1 + ((3 * 9).size(mult: 10)) );
		ptr (100ue out : 100ue in) factorial(lambda {
			out = when (n == 0) 1 else factorial(n - 1)});
	}

The grammar for such a syntax would simply be `EXPR ::= ATOM [ op EXPR ]`. This would
be right-associative, but () can group inside ATOM. Very powerful and simple.

	Type name name2(arg);
	Type2 name3(myarg; control=10) name4 name5;
	Type3[MyTemplate] name6;
	Type4[get=myfunction] name7;
	Type5[MyClass=OtherClass; get=myfunction] name8;

	(:) myfunction {
		32u a(name.getSomething()) b c;
		a, b: special, c = getMore();
		a += 3;
		ptr 32u d([1,2,8,f(),10]);
		label x; {
			array 10 32u e;
		}
		goto x 10 < 3;
	}

It's tempting to just let operators be syntactic sugar for right-to-left evaluated
`a.+(b).*(c)`. Or should that be done from left-to-right? It's also tempting to just
allow operators, and fix the tree.
An idea I've been entertaining is coming from my original implementation of Cxy.
It's the simple 'operation operands' statement list. Very minimalistic and requires
basically no parser. I'm thinking that that idea may come back again, as a bootstrapping
mechanism. Here's an outline

	// Core declaration/operations
	number name  // Declare a new stack-variable. Removed upon exiting scope.
	operation left right  // Any binary operation, mathematical, or otherwise. Returns to 'ans'
	// Flow control
	label name  // Labels this location, to jump/call
	goto name cond  // Jump to the label 'name' if the condition is not zero
	call name  // Call the label, returns here.
	// Dynamic memory
	new number  // Allocate number bits on the heap.
	del ref  // Delete the allocated bits
	// IO
	read  // Read 1 bit from the standard input.
	write name  // Write the bits of 'name' to the standard output.

This allows you to create a recursive factorial function:

	label factorial
	113 myvar
	= myvar ans
	-- ans
	call factorial
	* ans myvar
	return

113 myvar allocates 113 bits on the stack. It then copies 'ans' (a global) into the
variable. It decrements ans, and calls itself recursively. The result of the recursion
is multiplied by myvar. This result is already stored in 'ans'. The function returns.
Simple and bulletproof. The language can be made so that overflows fail the program
with a very clear message. m4 can be used to place other files in there. Why create
this? Well, maybe it's useful. It'll be a fun little experiment. Hell, even object-
oriented programming is possible. The idea is to let the programmer decide how many
bits a data structure uses. He then performs operations on that data structure as
he wishes. This gives a huge amount of power to the programmer. First, pointer and
reference semantics have to be defined. There is also the case of special optimizations
like vector mathematics. If we intend to perform that operation, the compiler should
be able to optimize it.

I think the language is in its final stage of preprocessing. I don't know what more
to improve. I can isolate specific algorithms. Especially the reordering one. That
will be a fun task.

Is the grammar finished? Is everything that has been desired implemented? Expressions
make up the largest part of the grammar in terms of sentences. I need to make ARG
and array access recursive. The difficulty lies in that. The use of [] and () are
in themselves left-recursive, so should we view [ and ( as an operator in that context?

	CALL ::= UNDER_CALL [ '(' ARGS ')' CALL ]

This could work. The '(' is merely regarded as an operator just like all the others.
This gives us

	CALL_EXPR ::= ACC_EXPR { ARG } ;
	ACC_EXPR ::= RES_EXPR { LIST } ;

Will this work? Imagine the code:

	a[4]()[b];
	((a[4])())[b];
	(((a(1))[2])[3])

The parentheses denote parse tree. Oh. I also resolved the problem with !! and !.
Now 'not' is uesd instead. likewise, 'xor' has been added. The words are operators
used for booleans. They can be created for other types too though. Ugh I'm still
pondering operator precedence. I really like to have it, but it requires post-processing.
It's the post-processing that I don't like. I wish the grammar to be easily semantically
correct. What about simply putting all operators on the same level?

	a + b * d;

Due to right-recursion, the right-most elements will be computed first.

	not a and b;

would be wrong, since not (a and b) would occur. I suppose if we want, we can simply
do:

	not(a) and (b);

Let's just expand on the idea:

	10 + 3 * 8 - 4 . max;

4 is an integer of size 2s (standard type is signed). Then, the first operation is
.max. This results in '7'.

	10 + 3 * 8 - 7
	10 + 3 * 1
	10 + 3
	13

This would be simple to implement, and also very friendly. Commoners do not remember
operator precedence. Professional programmers may remember them. For the longest
time I have not. People I have worked with have not. Maybe it's for the best. This
removes ambiguity and possible misinterpretations. How are method parameters affected?
So far only the unary operators take precedence over everything else. That does shorten
the grammar significantly.

	3 + @myptr() / 4;

@ binds more strongly, and () binds even stronger. This means that @ will dereference
the return value out from the myptr() call. It shall be as such. From lowest to highest
precedence:

	Binary operators
	Unary operators
	Type prefixing
	member/extraction
	argument/array access

Since everything is right-recursive anyways, there should be no problem with this
precedence.

	32u a;
	a = @val[3] + 4;

Where 'val' is of the type `::array 4 ptr 32u::`. Still, lists are not space-delimited.
The nice thing about all this is that we can still implement operator precedence
(as in, * before +) post-production, independently of the grammar. Overloading unary
operators with binary operators is still overloading of methods. This is something
that was not desired. Should a special function name be created? What about the address
of an address? Well an address is itself just an rvalue. It has no address except
some random value on the stack. It therefore makes no sense. I think I've gone over
this before. Anyways, let's examine the grammar. What about compound assignments?

	if ((a >= b) && c())
		doSomething();
	a += 3;

Operators are just methods. Wait, what if names are also operators? What if names
can take left and right arguments as well?

	Matrix a(inverse=true; eye=true;)
		b(eye=true) c(100);
	(3 - 1 * a matmul c())
	if (a contains 103)
		.sml.out << "a contains an invalid number";

I'm toying with some interesting stuff here. Allowing one to create custom operators
is incredibly exciting. Instead of being scala and making operators >>=<< weird symbols,
the operators are simply words.

	3 mul 8;
	8 mod 3;

Without precedence anyways, this should be fine. Operators are simply methods. There
is however one problem. It's pretty useful to have overloading. Without overloading,
the operators will be a mess. You mean I will have to define two multiplications?
One for scalar and the other for matrices.

	Matrix a(eye=true);
	32f b(3102.0);

	a * b;  // Ok, overload for 32f
	b * a;  // Err, no overload for floats
	b * b;  // Ok, matrix multiplication with self.

One part of me tells me that it should be fine to define other operators. How can
the above be resolved?

	a scalmul b;
	b * b;

It does solve the specificity of operators. I kinda like that. The question is: should
operators be type-overloadable? Let's examine.

	Mat4 quat(rot=45.0; [1.0,1.0,1.0]);
	Vec3 pt([2.0,3.4,5.0]);
	Mat4 newmat(quat vecmul pt);

the case for overloading appears strong. It seems just so useful... I'd think that
the specific names convey information that is actually important. I really like the
idea of custom operators. It opens up a whole new avenue of creativity. Without overloading,
we need to be specific in our method names.

	::Math::colmax(b matscal 1.3 + a matscal 10.0);

In the above cases, only binary operators are eligible for operator use. What about
unary and even ternary operators? I suppose we need to create a method taking zero
arguments:

	8.neg() + 3;

Which fits quite naturally. I just dislike that method names can not be used for
unary operators. This limits quite a lot. It's always the expressions getting in
the way. It's quite annoying. What about this method of expressions though? Binary,
named expressions. It sounds cool. `@getMat().matmul(@getOtherMat());` Why don't
I just roll with that? I need something simple to implement. Maybe create a lexer
instead. Not sure anymore.

	op(neg) alpha op(plus) beta op(times) gamma;

Everything except expressions are in place. What if operators are just kept like
they are? What if we do not allow  defining named operators? Only symbolic operators.
Any symbolic operators? No, we already concluded that was bad. Named operators seem
really interesting though. It's basically such that an operator takes the entire
right side as an argument, whilst a call nests.

	a.plus(b.plus(c.plus(d)));
	a op(plus) b op(plus) c op(plus) d;

That's actually seemingly useful. Maybe 'op' can be used... That may be a solution.
When do you actually use these operators? Rarely I reckon.

	a op(matmul) b * 100.0;

Maybe do as python does, by not naming methods the direct symbols, but instead __eq__.
This avoids any confusion... Maybe 'op - ' is better.

	@myValue() op-matmul secondMatrix();

I like it. This also allows the use of:

	op-square @myMatrix(start=1; increment=3; eye=true) op-matmul beta_matrix;

That is in fact incredibly clear. An identity matrix, created by a pointer, returned
squared, and matrix multiplied with `beta_matrix`. Yeah I like this. The language
is extremely simple now. It's easy to discern operators. Operations have to be static
as well. We can't allow expressions inside the op-name signature. It is merely an
alias:

	a op-matmul b;
	a.matmul(b);

Now that that is resolved: how do we declare a new unary - method? -- could do the
trick, but it's already used for iteration.

	inc a;
	dec b;
	--4 + 2

We can instead make inc and dec the incrementors and decrementors. This resolves
any potential ambiguity. This makes -- and ++ the respective negation and positivation?
operators. I like the idea.

	op-dec a;
	op-inc c;

Alright. That's now solved. Let's look at some sample code:

	(:) enter {
		sml::String string;
		sml.in op-read string;
		sml.out op-print factorial(string.to32u()).toString() + " is the number";
	}

	(113ue out : 5ue in) factorial {
		out = ::5ue::(in);
		dec in;
		while in > 1
			out *= in;
	}

! is the binary not, !! is the boolean not. I really like the possibility of coding

	if myset op-has 2
		debug "Op contains two!";

It really allows the programmer to write beautifully expressive and clear code. That
is something to strive towards. Now, unary operators are not explicit, so we still
need the comma in the list. I wish to remove the comma. Maybe have something like
'uop' or 'up'? This will 'unify' (not exactly, but the word seems nice here) lists
with the data declaration statement, as it requires no commas either.

	up-inv $getMyMatrix() op-matmul other_matrix;

This seems fitting, as unary operators are supposed to bind more strongly. They have
higher precedence. Should $$ be changed? I like the consty stuff of it, but it looks
a lot like !!, --, and ++. What I don't like is that the double characters are both
in the binary and unary operators. What about the semantic requirement for operators?

I would like to think it automatically takes a const-reference to the argument. That
seems very useful, because we don't want to write:

	sml op-print $a - $b;

We want to write:

	sml op-print a-b;
	sml.print($a.-($b));
	sml op-print a.--();
	sml op-print --a;

Let ',' not be an operator. That would be incredibly annoying. So should we just
remember the types of operators? What about smart pointers? C++ allows redefining
'->' and * (dereference) that allows you to create a class that acts as a smart pointer.

	std::unique_ptr<int> a(new int(0));
	*a = 4;
	sml::Sptr[32u] a(new[32u]{0});
	// @a = 4;  // Not allowed
	up-dref a = 4;

Instead of using @ we can define up-dref to dereference. If @ is not allowed to be
overridden, then we have to define our own dereferencing method. I think that may
be the best.

	array 32u a([1 --4 32 un-square 43]);

Maybe we should just be satisfied with remembering the different operators. Maybe
that's the way. Perhaps it's fine.

	private 32u a b c;

	(:) enter {  // CLASS
		This my_object;
		my_object.a = 1;
		my_object.b = 2;
		my_object.c = 3;
		debug my_object.a + 1;
		debug my_object.c + 1;
		sml.out.print(my_object.a.toString());
		Matrix a(type=::Matrix::TRIANGULAR; grow=2) b;
		b = ::Matrix::eye();
		a = a op-matmul b;
		sml.out op-print a.toString();
	}


