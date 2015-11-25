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
