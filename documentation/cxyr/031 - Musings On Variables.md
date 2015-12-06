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

This kinda does var a favor. By forcing var, we're giving a strong enough hint.
