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

	FEXPR ::= EXPR ('{' NAMELIST '}'|[name] [':' name]) {',' EXPR ('{' NAMELIST '}'|[name] [':' name])} ['=' EXPR] ';'
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
