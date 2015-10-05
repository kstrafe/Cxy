## As A Lisp ##
*Description*: Why not make the language super simple? Like LISP.
*Discussion*: It has crossed my mind multiple times to make the language be a lisp.
After all, all that's really needed is basic text editing functionality. Once that's
in place, one can issue commands to any compiler to compile object files if they do
not already exist. The idea is to make the language simple and interpreted. This allows
a lot more freedom, as well as being able to take dynamic control over the environment.

	(scope (
		(array (bits 32) 100 my_array)
		(array (array (bits 64) 10) 20 name)
		(var (bits 100) cool)
		(++ cool)
		(bits 32 name)
		(set name 2034)
		(++ name)
		(for name (+ name 100) 1
			{
				(array.at my_array 23)
			}
		)
	))

Wow slow down. So far this looks neat and all, but what about reference semantics?
In any case, should reference semantics be defined properly? Or should full referential
transparency be put in place? For me - the C++ programmer - it seems natural to want
some kind of reference system. Certainly, declaring a variable merely puts it in its
scope's store. This means that the name solely refers to a location in the store.
When we call a function, should we just pass the entire scope as a reference, and
say that the parameters are the parts of the store that are accessible to the function,
being aliased of course. Then, any name can just be an alias for another name.

	(function [[array (Bits 32) string]] doSomething [[Bits 32 counter]] (
		(= string (sml.Bits.toString counter))
	))

	(scope (
		(var (bits 32) counter)
		(set counter 0)
		(doSomething counter)
	))

This is starting to look like something. Let [] denote array. It is shorthand for
	(array '(a b c d))

I don't really like the ' notation. It's ugly.

The first array defines the returns, the second the inputs. The function registers
these as simple elements of the array. So it adds 'doSomething' to the global scope.

	doSomething
		out: array (Bits 32) string
		in: Bits 32 counter
		code: ...

Whenever doSomething is invoked, the global lookup can find it and actually invoke
it. Now what's left is to make this concrete and consistent. Ideally, we have a turing
machine with a global store. However, we still love demeter's law. So how is that
put to use? Suppose we have the same folder structure as in the many previous examples.
What does an actual class type return?

	(@ sml String)

is the type sml.String. I suppose a shorthand notation can be used such as: (sml-String)
What is this object? Well, it can refer to the entire class. It just is the class.

	(var (@ sml String) my_string)
	(var sml-String my_string)

Both are equivalent. The virtual machine can just see if sml/String is loaded. If
it is not, it gets loaded. Else an ID is returned. What kind of ID? Well it has to
refer to the appropriate type class. Whenever a manipulation is performed, the type
is checked to be of the correct class.

	(var sml-String my)
	(sml-String-length my) // Returns a Bits 64 type

Of course, a type constructor is useful

	(var (Bits 50) integer)
	(set integer 0)
	(++ integer)

Bits is just a class that takes an argument. That argument is also part of its type.
How is its argument stored?

I can't answer that. In order to do so, we need to look at an individual class and
its instantiation.

	(class MyClass [Bits 40 bitcount] [

		(function [[]] addLots [[]] [

		])
		# Definition here
	])

On one hand, types are a nuisance, on the other, they're nice. The use for types is
to make it unambiguous what input a function takes. Maybe it should be made optional.

	(class MyClass [
		((Bits 32) sum)
		(= sum 0)
		(function addStuff [
			(+= sum (arg 0))
		])
	])

	(MyClass myobject)
	(addStruff myobject 100)
	(addStruff myobject 50)
	(print (myobject sum))

This is equivalent to the following in C++:

	class MyClass
	{
	public:
		int sum = 0;
		void addStuff(int arg)
		{
			sum += arg;
		}
	}

	int main()
	{
		MyClass myobject;
		myobject.addStuff(100);
		myobject.addStuff(50);
		std::cout << myobject.sum;
	}

What the lisp-like class contains is a name and an array of something. Every parenthesis
evaluates into a single object, and hence a single element in the array. The first
element is a variable declaration of sum.

	(Type name) # Declaration of data
	(Function ...) # Function invocation
	(method object ...) # Method invocation

Eh, not really a fan of the method one, seems like magic.

	((. object method) ...)

Seems somewhat more appropriate, but do we really bind the object to the method?
Maybe (object method ...) is more appropriate, as the object acts like a function
taking in any amount of arguments. The first argument is then returned.

	((object method) 100)

This seems to be able to scale very well, considering its simplicity.

	(function . [
		((object method) arg)
	])

I'm still uncertain about the semantics. It's nice to think of a class merely as a
map that returns the argument's name. That seems very... clean. I like that. Now to
look at pointers, references, etc... But wait, what about arguments and their declaration?

Designing the store to be a hash-map could be interesting. Alas, things must be efficient.
Efficiency is important. It always is. Ideally, the language should have the ability
to be compiled into efficient platform specific binary code. This basically forces
the explicit reference to types. Integers are important types. The initial idea is
to introduce simple types that can easily be optimized as cpu registers. Types such
as (bits 32) or (bits 64), even (bits n) where n is <= 64 would work, since we can
allocate the smallest possible count of bytes (and use the smallest possible register
to store the value). I'm still quite afraid that this will make the language more
verbose and perhaps even ugly...

	(function [[]] enter [[]] [

	])

Let's define a function. It's just a function. It returns something, and it gets something.
Is it pure? Is it a method? Does it make sense to take in arrays of arrays as arguments?
Well, multiple return values can exist. That is just practical. The same goes for
multiple input variables. How will this look? Well first we need to look at how to
declare a type. The simplest type is just a set of bits. They can act like any integer.

	(function [(bits 32) (bits 64)] split [(bits 96)] [
		# This function splits a 96 bit value into upper and lower parts.
	])

What's lacking are the names.

	(function [(bits 32) lower (bits 64) upper] split [(bits 96) in] [
		# This function splits a 96 bit value into upper and lower parts.
	])

Now, when bits are evaluated, all the compiler really sees is an array of [32 lower 64 upper].
Of course, it has a different value for those 32 and 64, those merely denote a type.
So now types are added. Good. So what about named parameters and returns?

	(split 31209) # What does this return?

The above can return the actual array. That however is quite inefficient. It would
be better suited to be able to select which return you would like to extract.

	(extract (split 31209) upper) # Here we extract the return argument.

How do we extract both arguments?

	(bits 32 my)
	(bits 64 name)
	(= my name (split 31209))

We can just let = act as a pairwise assignment operator. This fits the model well.
One thing I miss about this is that there are no named arguments. That really helps
readability. Maybe the language can somehow support them. Ah! Let's try the following:

	(split {in 31209} {otherarg 1328})

By simply returning a tuple, the function split is able to identify the correct variables.
Does {} create a tuple then? I'd rather avoid doing (tuple otherarg 1328), that's too
verbose imho. I think we need to clear up how the function then interprets the array of
code.

	(= {lower my} {upper name} (split {in 3218}))

Damn, that looks smooth! In essense: {} - tuple, [] - array, () - call. What about
the tuple semantics? The variable names, will they just be put in there or what? My
idea is to make the first lexeme never refer to another variable, but just be a string
in and of itself.

	{lower my}

Really just means a tuple of {"lower" (reference my)}. So when the function = sees
this tuple, it will make sure to assign the return variable "lower" to the reference
given.

	(function [(bits 108) out] factorial [(bits 5) in] [
		(= out (coerce (type out) in))
		(-- in)
		(for in 1 (-1) it [
			(*= out it)
		])
	])

That's a basic factorial function. It computes the factorial recursively. It can be
called by:

	(factorial 32)

Where "in" always allows one single unnamed argument. Otherwise:

	(factorial {in 32})

Also works.

	(bits 108 myvar)
	(= {out myvar} (factorial {in 32}))

In this case, the following is equivalent.

	(bits 108 myvar)
	(= myvar (factorial 32))

The for loop is just another function, taking in a few arguments, and then it's done!
However, it's important to note that the for function takes in the current scope as
well. For can thusly not be defined by any other function. It must be builtin.

	(while (> out in) [
		# Do something
	])

The while statement is a little more tricky. The major problem with it is that the
boolean predicate is something that must be given and be re-evaluated at each iteration.
To do this, we introduce lambdas.

	(while ($ > out in) [
		# Do something
	])

Lambdas are basically the following identity:

	(($ op)) = (op)
	op = ($ op)

The lambda operator, $, creates returns a function. This way, while can just take
in a function with already-bound variables to use. Maybe the entire array needs to
be a lambda as well? You know, to avoid having to reference the same scope.

	(function [(bits 108) out] factorial [(bits 5) in] [
		(= out (coerce (type out) in))
		(-- in)
		(for in 1 -1 it ($
			(*= out it)
		))
	])

I'm not entirely sure about this. It would be nice to just clear up the connection
to the scope. I need to take a walk... maybe that'll clear my head. Alright I'm back.
I've thought about the problem and it appears to boil down to the fact that operators
as ++ are in fact methods of the variable. Hence, they are able to edit the value.
However, the method also takes in a reference to the value it is going to edit, thus
ending in a loop of references. That train of thought doesn't hold very well.. So
what's the deal?

A name is just a name. It refers to a variable. `(var (bits 32) xy)`. `xy -> val`.
xy is simply a pointer to a value. Writing `(print xy)` basically sends the pointer
to val into print. The question is, should this be a ptr const? If it were, then we'd
be safe to assume a casting in the following:

	(-- (unconst xy))

But this does not make sense, casting from const to non-const should be illegal.
Hence, it should be the other way around. `xy` is a pointer, and that's all.

	(var (bits 32) xy)
	(= xy 100)
	(-- xy)

Sending it to a function taking in a const would be to cast the pointer:

	(var (bits 32) xy)
	(= xy 100)
	(mycall {argname (const xy)})
	(mycall2 (const xy))

This will have to be forced, to make it obvious that a function does not modify the reference.
What about implicit scope insertion? The for function should be able to handle it...
Or is for really a construct rather than a function? The same would be applied to
while. They're not functions, they're constructs. A function can not be applied to
these entities. So how do we add these constructs into the language? Should they be
in the language? Many functional languages have no such constructs. I think they [
such constructs] can be incredibly useful. Make it a built-in function. Make it so
that it takes the entire scope implicitly. What about lambda captures?
($ op), will need to capture the actual operation and the pointers to the variables.

	($ > value 100)

can be considered the same as ($ > @0x33ff 100). It returns a function with preset
arguments.

	(var (bits 32) xy)
	(= xy 0)
	(for 0 100 1 it [

	])

In the same way, the class and function functions are also not really functions. They
are... constructs. Theses 'functions' can access the global store. They can interact
with the internals of the language itself. I don't think they can be emulated in the
host language. The same can be said of defun in LISP, unless I know too little.

	(while boolean-test code)

	(label a)
	(doSomething)
	(if (predicate) (goto a))

How can while be written as an if and a goto? In this case, we'd like the while to
transform this code:

	(function [] while [x y] [
		(label x)
		# Code here
		(if y (goto x))
	])

Ideally, this would be what we call a macro. When called, it returns verbatim code.
Its arguments are not evaluated. This avoids a lot of trouble.

	(macro while condition code [
		(scope
			(label z)
			(x)
			(if y (goto z))
		)
	])

Nice. This is our code generation. Well.. It's not runnable code. As in, the macro
can't really run anything in a manner that's practical. To make that possible, I think
it may be necessary to add a block of code to any macro that is able to check the
input variables for validity. The good news about this stuff is that while does not
need to concern itself with the global scope or the internal store. Instead, it only
uses other macros and/or builtins. So what are the builtins?

	# TABLE 8.1 #
	# Operations
	## Types
	(bits number)                # Create a type of information of number bits.
	""                           # Create a string of characters.
	## Fundamental Constructs
	(address object)             # Take the address of an object.
	(cast type object)           # Cast an object into a new one.
	(code)                       # Scoping the code. This means that the end of the scope invokes destructors.
	(const name)                 # Returns a const pointer of name.
	(del pointer)                # Delete (deallocate) a pointer's contents.
	(deref pointer)              # Dereference a pointer.
	(expand function ...)        # Invoke a macro, returns code.
	(function out name in code)  # Create a function.
	(goto label-name)            # Jump to a label.
	(hack code)                  # Insert compiler-specific code.
	(if condition code else)     # Conditional evaluation of code.
	(label name)                 # Creates a label to jump to. Part of a scope, else nested while ambiguity.
	(new type [elems])           # Returns a ptr to type.
	(ptr type)                   # Returns a type of pointer to type.
	(procedure out name in code) # Create a procedure.
	(template name in code)      # Create a template.
	(val type name code)         # Declare a constant expression variable. code is run to initialize the type.
	(var type name ...)          # Declare a scope-local variable.
	[element ...]                # Array.
	{name reference}             # Single tuple declaration.

	# Unary Operators
	(++ object ...)              # Increment objects.
	(- object)                   # Negate an object.
	(-- object ...)              # Decrement an object.
	(! object)                   # NOT an object.
	(| object)                   # Absolute value of object.

	# Binary operators
	## Comparison
	(== left right)              # Check left and right for equality.
	(!= left right)              # Check left and right for equality.
	(>= left right)              # Check if left is >= right.
	(> left right)               # Check if left is > right.
	(<= left right)              # Check if left is <= right.
	(< left right)               # Check if left is < right.

	## Computation
	(= pair ...)                 # Copy an object to another.
	(+ left right ...)           # Add two or more objects.
	(+= object add ...)          # Add to an object.
	(- left right ...)           # Subtract two or more objects.
	(-= object subtract ...)     # Subtract from an object.
	(* left right ...)           # Multiply objects.
	(*= left right ...)          # Multiply and assign.
	(/ left right ...)           # Divide objects.
	(/= left right ...)          # Divide and assign.
	(\ left right ...)           # Integer divide objects.
	(\= left right ...)          # Integer divide and assign.
	(% left right ...)           # Remainder.
	(%= left right ...)          # Remainder and assign.
	(^ base exponent ...)        # Exponentiate.
	(^= base exponent ...)       # Exponentiate.
	(<< shift bits)              # Shift left.
	(>> shift bits)              # Shift right.
	(&& bits ...)                # AND operator.
	(|| bits ...)                # OR operator.

Does a unary + make sense? I'm not sure. C++ has it. What I like about LISP is that
you can have many items in the parameter list.

	(-- a b c d)  # Decrement a, b, c, and d by one.

That's incredibly helpful and shortens the code significantly:

	(-- a)(-- b)(-- c)(-- d)

I suppose operator \\ and // isn't very useful either. Dividing by oneself or by the
unit of division (be it one or something). The same goes for operator `**`. This operator
becomes ambiguous when contrasted with ++ and --. That's why I prefer the operator
to be avoided. What about exponentiation? I like the idea to be able to write

	(^ 2 3 2)

This will evaluate to 512 due to the order of operations.
Another difference from conventional languages is that the comparison operators >=
and <= are swapped out for => and =<. The reason for this is because "Op=" is already
used to assign a value. No let me put that back. We have == anyways. It's not a problem.

For any =, and compound assignment operator, we return a reference to the left hand
side:

	(var (bits 32) name)
	(= name 0)
	(-- (+= name 1 2 3))

This evaluates to (-- name), with name = 6, so we decrement name, and get 5. Sounds
good. What about invoking macros? I'd prefer to use a different notation than () like
is used for all functions and builtins. A question comes up: "What is code?". Can
we denote code as ((?

	(scope
		(var (bits 32) counter max sum)
		(= counter max sum 0 100 0)  # Assign 0 to counter, 100 to max
		(label begin)
		(+= sum counter)
		(++ counter)
		(if (< counter max) (goto begin))
	)

Scope can fill that role! That is really neat actually. What about verbatim strings?
I mean, how does this work with macros?

	(macro my-macro (a b c) (scope (if (> b c) (goto error)))
		(a b c)
	)

Maybe it's appropriate to say that the macro keyword affects the way stuff is evaluated
within. This makes sense, still, I'd like to see how I can make lists and all that.
Common Lisp just uses () without a problem... Maybe I need to do so as well...

	(macro my-macro (fragment code)
		(if (< (length fragment) 30)
			(fail "The length of the fragment should be bigger than 30")
		)
		(
			(fragment)
			(code)
		)
	)

	(my-macro (a > b is good) (derp herp))

No macros need to be more powerful and generate arbitrary code, not just be vapid
templates!

	(macro my-macro grammar (
		...
	))

Hold on... why not just use functions and invoke them specially?!

	(function (string out) createParser (Grammar grammar) (
		...  # Code to generate the code for a parser
		(= out generated)
	))

	(@ createParser my-grammar)

This also solves the issue of not being able to distinguish a function with a macro
call! That's absolutely awesome. Though, @ is too cryptic, let's change it to "expand".
I think I'm beginning to like this.

Perhaps even make the base language a little simpler, like only two elements for stuff
like + and * . Shouldn't really matter that much though. Anyhow, I like the idea of
just using "expand". It fits really well... anyways, are there any more things to
consider? Well of course! Strings. Let's look at strings. I currently like verbatim
strings. They're simple, small, and elegant. With "" as an escape for itself. A simple
mealy machine can take care of this. So what about strings anyways? And how are functions
invoked via expand in any way valid? Well, we know that their input parameters ought
to be some constant expression. It must not take anything from any surrounding context.
This is important, as it keeps the generators relatively pure. So what's the big idea?
Well, perhaps we need a way to declare constexpr data. Data that could be referenced
during macro expansion phase. That would be a clean way to keep the data organised.
The thing is, what if we have data that is used both in the macro expansion phase
as well as in the running code? A problem arises. If we embed the data into the specific
function, then the runtime can not access it. If we make the data a variable, the
expansion can not access it. A compromise has to be made. Perhaps a "variable" declaration
that is also considered by the expansion phase. (expand var ... comes to mind, but
I think that would clash with the current definition of var, which doesn't quite fit
the bill. `(val ...` could be used instead. Val is just that: a value. A constant
expression. To be used both during runtime and macro time. What about while, for,...?

	(expand while 1 10 1 it (...))

This seems rather verbose, ugly, and unnecessary. I'd prefer it to be just "while".
Perhaps it is thus useful to have a true macro,... a template. The template can be
the simplest of substitutions, merely substituting the code for something else.
Certainly, a template can use expand functions as well, but the template itself can
not do much except for putting stuff in. It can't compare. It just "puts stuff".

	(template while from to jump itname code (
		(expand While from to jump itname code)
	))

Just syntactic sugar for the expand of While. While can also check the incoming bounds
to see if these may be invalid and issue warnings, say, if the jump count is zero.
My programming senses always told me it's natural to evaluate the inner most parentheses
first. It feels as if the language violates that. I mean, what is even a list of runnable
code like?

	[
		(+= a b)
		(= a b)
		(while a 1000 b [
			(print (+ "I like this idea of using []" "It's nice"))
		])
	]

One can think of the `(print ...)` as an expression that gets evaluated into a runnable
object. Yes... yes,.. that seems to be a nice abstraction! If the runnable object
is at a local scope,.. or I should say at the top of the hierarchy and not nested,
then I suppose it can simply be executed. Otherwise it's not executed and instead
passed on into the function. This fails on addition or similar operations though.
Luckily, while is a template, it will expand in something different.

	[
		(+= a b)
		(= a b)
		(scope [
			(label x)
			(print (+ "I like this idea of using []" "It's nice"))
			(+= a b)
			(if (< a 1000) (goto x))
		])
	]

I can see the idea, but it doesn't feel quite natural. As if it should be different.
So how do we go about? Do we just state that any nested () is at the mercy of its
respective enclosure? That seems quite fair actually. This would do away with the
[ and ] in scope, as well as the while and other such functions. Though, it would
be nice to have a list over all arguments put in.

	(+= a b)
	(= a b)
	(scope
		(label x)
		(print (+ "I like this idea of using []" "It's nice"))
		(+= a b)
		(if (< a 1000) (goto x))
	)

Is now the equivalent code. I suppose scope is of the form (scope list). This seems
quite elegant. What about functions taking in functions? Maybe just let too many arguments
collapse into a single list automatically. That would be an elegant solution,.. I
think...

	(function out1 out2 out3 name in1 in2 in3 code)

In the case of a function, it's a little different, there are multiple lists here.

	(function (bits 32) out1 (bits 64) out2 (bits 100) out3 name (bits 10) que
		()
	)

Maybe the function declaration needs to use a delimiter of some kind. That's the problem
here. I can either use a delimiter or just put a list into the thing. Idealize "function"
as some hyper complex macro or template, what would it do? Well its arguments would
just be turned into some code right? That makes arrays pretty much perfect for the
job. What about scope then? Does scope need it? I'm not sure. What would be most clean?
I like the idea of the array though. It seems neat. So suppose we have the array,
as well as standard invocations via (), then what are we really dealing with?

	(scope
		(var (bits 10) cnt)
		(= cnt 0)
		...
	)

	(scope [
		(var (bits 10) cnt)
		(= cnt 0)
		...
	])

I like both ideas. I need something consistent. If scope were a macro, it would probably
just scan each element in the list and use it to determine destruction etc. On the
other hand, using an implicit list (via elipsis), the invocation looks much cleaner,
but it does make the language less consistent. The reason for being less consistent
is that the idea fails for the function keyword. What if we make the keyword scope
implicit? This means that () would be accepted as a scope. It also allows us to easily
create 'lists' of operations

	(while 1 10 1 (print "hey!"))
	(while 1 10 1 (
		(print "hey")
		(print "what's going on?")
	))
	(function [(bits 32) a (bits 64) b] name [] (
		(= a 301495)
		(= b 38175)
		# Do some cool stuff here.
	))

I really like this idea. It seems neat. So any ( followed by a non-word evaluates
to a scope. I really like this. This fits really well with the current models.

	(if (> 3 a) ((print "error") (exit 1)))
	(hack asm "mov eax, ebx")
	(function [TypeA a TypeB b] myFunction [TypeC c typeD d] (
		(= a (cast (type a) c))
		(= b (cast (type b) d))
	))

	(var TypeA mya)
	(var TypeB myb)
	(var TypeC myc)
	(var TypeD myd)
	(= {a mya} {b myb} (myFunction {c myc} {d myd}))

Seems like it's going somewhere. Let's have a small challenge: write a small program
that processes a number by taking the factorial of the Nth fibonacci number, squares
it, and then divides it by 100.

	(function [(bits 108) out] factorial [(bits 5) in] (
		(= out (cast (type out) in))
		(-- in)
		(while in 1 -1 (*= out (cast (type out) in)))
	))

	(function [(bits 5) out] recFib [(bits 5) in] (
		(if (>= 2 in)
			(= out (+ (recFib (- in 1)) (recFib (- in 2))))  # If true clause
			(= out 1)  # If false clause
		)
	))

	(function [] enter [] (
		(var (bits 5) n)  # Maximum size of 2^5-1, is about 31.
		(= n 4)  # Assign some number to it.
		(= n (recFib n))
		(var (bits 108) fact)
		(= fact (factorial n))
		(var (bits 216) square)
		(= square (^ (cast (type square) fact) 2))  # When you square, you can double the bit size (at most).
		(/= square 100)
		(sml-print square)
	))

That was easy enough. The fibonacci sum should however be taken care of, as it could
overflow the 5 bits. Five bits are chosen because the amount of bits for the biggest
factorial is only 108. How can the language be improved? Pointers are important. They
should somehow play a role. Already now, pointers are simply names. Names that refer
to a location in memory. Dynamic memory management should be possible as well, for
full control of the machine! `(new type)` comes to mind. It just returns a reference
to that new type. That seems too simple. Remember, a name always results in a reference.
Unless the type is a pointer:

	(var (ptr (bits 32)) x)
	(= x (new (bits 32))
	(= (deref x) 3000)
	(del x)  # A good practice to always deallocate it. Else memory leaks may occur.

The deref basically turns the pointer into the reference that the = operator can work
with. If not, we'd get a type-error because we're assigning a number to a ptr type.
I would need to consider the name for the deref function though. It needs to be clean
like most other elemental keywords. Deref sounds fine, actually. What about dynamic
arrays?

	# Put numbers from the command line into an array, then sort it. Inline.

	(function [] bubbleSort [(ptr (bits 32)) in size length] (
		(for 1 length 1 at (
			(var size local)
			(= local at)
			(while
				(and
					(< (deref (+ ptr (* 4 local))) (deref (+ ptr (- (* 4 local)))))
					(> local 0)) (
						(swap (deref (+ ptr (* 4 local))) (deref (+ ptr (- (* 4 local) 4))))
						(-- local)
			))
		))
	))

The above function sorts the integers of an contiguous array of length "length". The
way it does so is that it starts at location 1. Check if the previous element is bigger.
If it is bigger, swap the element. It looks kinda difficult, but that's nothing for
a template!

	(function [] bubbleSort [(ptr (bits 32)) in size length] (
		(for 1 length 1 at (
			(var size local)
			(= local at)
			(template current [] (deref (+ ptr (* 4 local))) )
			(template previous [] (deref (+ ptr (* 4 (- local 1)))) )

			(while
				(and
					(< (current) (previous))
					(> local 0))
				(
					(swap (current) (previous))
					(-- local)
				)
			)
		))
	))

Taking an address of an object is necessary when a function or method expects a ptr.
It is sufficient to state that the builtins can just take the address implicitly for
usability reasons. Otherwise, the above sort can be invoked only by the following:

	(var (Array 10) x)
	(= x [0 1 2 3 4 9 5 8 7 6])
	(bubbleSort x)  # Since an array essentially IS a pointer.

	(var (bits 100) y)
	(= y 10000)
	(doSomethingAndModifyMyData (address y))  # Will not work with just 'y'.

The latter also gives us important information about the call. We know for example
that the call may modify y in-place, so we must be cautious. If the 'address' was
not there, then it may still be modified in-place if y is a pointer type. If doSomething
is a function that takes a const ptr, then there will be no modification:

	(doSomething (address (const y)))

A template can be created for this:

	(template aco [name] ((address (const (name)))))
	(doSomething (aco y))

Which is much shorter to type and much more elegant. This reminds me of something
important. How should templates be evaluated? My current idea is to just take the
entire argument of name and paste it in its place. So if we have `(aco y)`, `(name)`
expands into `y`. If we have `(aco (y))`, `(name)` expands into `(y)`. That seems
fair enough. What about the rest of the template? The stuff in (), the code. Should
the () be added when the code is generated? I'm not sure actually. Does it make sense
to have those () everywhere a template is invoked? Ah! Yes?. The template invocation
itself already has (), these are removed when the template is evaluated. This means
that the contents of the template's () are put into that exact place. This seems like
it requires removal of the outermost () from the actual template's substitution. Imagine
the creation of a parameter list like so:

	(template accum [name] ((+ name 1) (+ name 2) (+ name 3)))
	(something (accum x))
	(something (+ x 1) (+ x 2) (+ x 3))  # Our desired result

Notice how the outermost layer is stripped. This is practical. If we really wanted
an outermost layer, we'd just define the template like so

	(template accum [name] (+ (+ name 1) (+ name 2) (+ name 3))))
	(something (accum x))
	(something (+ (+ x 1) (+ x 2) (+ x 3)))  # Our other desired result

So this gives us the flexibility we need to work with different styles of code generation.
Good. That's out of the way now. What else? Well, expand calls. Similarly, the code
generated by expand calls needs to strip the outermost () and just paste the code
there. It's that simple. The (name) from the arguments however is NOT similar. Remember
that the arguments are actual readable and useful input to the expand algorithm. This
means that we must just convert the input into a string and append it. We can however
not know the original name of the input parameter. That seems just.. done. Like, is
there anything I can say about templates or expansions? No? Let's move one.

Array syntax is important. I'm still a little wary of the [] notation, also, how can
we declare an array of size n? Restricting ourselves will probably give the best efficiency
results. Say (array (bits 100) 10) is the syntax for an array type. This means that
it's (array type length). These could just be allocated on the stack for maximum efficiency.

	(var (array (bits 64) 10) x)
	(= (x at 0) -100)  # Set the first index of x to -100
	(= (x at 2) 100)  # Set the third index of x to 100.
	(print (+ "Array size: " ((| x) toString)))  # Prints "Array size: 10", without quotation marks.

Where the absolute value function computes the length of the array. Any constant expression
array could be given by the [] notation. [1 8 9] evaluates as (array (bits 4) 3).
This makes us conjure up interesting datastructures. Imagine what one can do with
pointers, arrays, etc. It would be trivial to implement a map (dictionary).

	(class Map (
		(var (ptr Map) left right)
		(var (bits 32) value)

		(function [(ptr Map) out] find [(bits 32) in] (
			(if (> in value)
				(if (== right null) (= out this) (= out right))
				(if (< in value)
					(if (== left null) (= out this) (= out left))
					(= out this)
				)
			)
		))

		(function [] insert [(bits 32) in] (
			(var (ptr Map) near)
			(= near (this find in))
			(if (> in (near value)) (
				(= ((deref near) right) (new Map))
				(= (((deref near) right) value) in)
			) (if (< in (near value)) (
				(= ((deref near) left) (new Map))
				(= (((deref near) left) value) in)
			) ())
			)
		))
	))

Looking good. The language is quite expressive. Although I haven't really touched the
class syntax and semantics yet. I like that we can be imperative here. Really helps
speed things up (both conceptually and on the computer). Should a distinction between
procedures and functions be made? A function could be thought of as always pure, solely
relying on its input. Procedures can be thought of as potentially impure actions.
An impure action is to compare a pointer, or to cast a pointer to an integer, or to
call the environment's /dev/random device, or to interact with the filesystem. Or
to access a global variable. To avoid this, we can make a disctinction between procedures,
functions, and... well that's it right?

	(function [] name [] (
		...  # I shall never access anything not given to me. I am holy.
	))

	(procedure [] name [] (
		...  # The work horse of every program. Does all the dirty work and cracks gears.
	))

	(subroutine [] name [] (
		...  # I have no idea what can be done here.
	))

	(method [] name [] (
		...  # Specific to a class?
	))

So functions are truly pure. They can only access "val" items, but not "var"s. This
makes perfect sense. Procedures can access globals and talk to the environment. I'd
love to have some const-correctness like in C++. How do we get it here? We could say
that the function inside a class takes an implicit this pointer, which must therefore
not be modified. The method can then be the non-const function to a class. For subroutine,
I don't know. What about the semantics of accessing an object's field(s)? Currently
I have `(object field args...)` in mind. This basically asserts that all objects actually
are functions that take in a list, of which the first argument is to be the method
run. with the arguments args. So now about types.

Types are important. I've always been a fan of strong and static typing. They really
catch bugs and are especially useful in larger projects. However, how do we allow
different types to exist? Also, how do we define the base types? We already have the
'bits' types, which are,.. unsigned or signed integers? Who knows. This is important.
I suppose the bits are just that. Bits. Although it seems logical to just evaluate
them as unsigned integers. Essentially natural numbers. The most basic of numbers.
I like the idea. What about other types of integers? (signed 30)? (unsigned 64)? Looks
promising to be honest. `float` also looks good. Although I'd rather leave floats
and doubles to the implementation. Those types are scary. What about characters? My
idea is to just use `(bits 8)` as a character.

	(function [] copyInto [(ptr (bits 8)) into (array (bits 8)) from] (
		(for 0 (from length) 1 iterator (
			(= (into at iterator) (from at iterator))
		))
	))

	(procedure [] enter [] (
		(var (ptr (bits 8)) message)
		(= message (new (bits 8) 100))
		(copyInto message "Hello there!")
		(sml-print message)
	))

This means "" is just (array (bits 8)), which we can denote as (ptr (bits 8)) which
is the same as 'string'. So we can just start callin' em strings instead!

	(function [] setbit [(ptr (bits 256)) vec (bits 8) bitnum (bits 1) value)] (
		(template intype [name] ((type (deref vec))))

		(var (intype vec) razor)
		(= razor 1)
		(<<= razor bitnum)
		(if value (
			(|= (deref vec) razor)
		) (
			(var (intype vec) full)
			(= full (intype vec) max))
			(xor= razor full)
			(&= (deref vec) razor)
		)
	))

	(var (bits 256) mybits)
	(= mybits 4328472398479)
	(setbit (address mybits) 20 1)

How can the above bit setter be turned into a generic template? Well, type is definitely
a useful construct. Is it possible to make everything a template/macro using the hack
statement? Imagine

	(template if [condition code else] (
		(hack (condition))
		(hack (code))
		(hack (else))
	))

Of course, hack needs to be able to parse stuff like (> x y), so I'm not sure how
useful this is. I suppose new could be a hack. It doesn't really matter that much.
Stuff like the following can however easily be defined:

	(template print [item] (
		(hack (+ "std::cout << """ (item) """;"))
	))

	(var (bits 64) mybits)
	(= mybits 319231842378)

	# The following are equivalent:
	(hack (+ "std::cout << """ mybits """;"))
	(print mybits)

The problem with hack is knowing how the compiler works internally. To be able to
generate correct code everywhere, we need to know the name and version of the compiler.
This requires another primitive. (version) and (compiler). Version returning a x.y.z
version string, and version returning a simple name. This allows your template to
become a macro invocation:

	(function [string out] 'print [string version string compiler string to_print] (
		(if (&& (= version "1.2.7") (= compiler "CxyC")) (
			(= out
				(+ "(hack ""std::cout << """ to_print """.toString();""")"  # std::cout << to_print.toString();
			)
		))
	))

	(template print [item] (
		('print {version (version)} {compiler (compiler)} {to_print (item)})
	))

Note that (to_print) is evaluated as a string, such that the resuling C++ code may
be like `std::cout << my_variable.toString();`. This is only valid if the compiler
has specified all objects to be directly mapped into C++ with the same name. If the
code is interpreted, then the (compiler) variable will be "CxyCi", denoting that the
print function ought to use another hack (or compiler builtin). Do we need any more
primitives? It's like. I think we've gotten most of it out of the way. Templates,
macros, variables, turing completeness, pointers. Should be fine right? Oh wait! Something
important! Classes. These are always useful. How do we invoke a constructor and create
a class?

	(class MyClass [(bits 100) bitcount] (
		(var (bits (bitcount)) counter)

		(method [] addOne [] (
			(++ counter)
		))
	))

	(var (MyClass 20) myobj)
	(myobj addOne)
	(print (myobj counter))

This is a rough draft. I like the idea of templating classes with a []. This stuff
should be compile-time information that basically defines a subtype of a class. Any
class can have as many of these arguments as possible. Another thing is the potential
for a constructor. A constructor is really useful. I suppose the last arguments of
the class's var invocation can be the arguments to a constructor.

	(class MyClass [(bits 100) bitcount] (
		(var (bits (bitcount)) counter)

		(method [] MyClass [(type bitcount) start] (
			(= counter start)
		))

		(method [] addOne [] (
			(++ counter)
		))
	))

	(var (MyClass 20 {start 5}) myobj)

So we let's do something different for var.

	(var (MyClass 20) myobj {start 5})
	(MyClass 20 {start 5})

The first line is a constructor call during initialization. The second is an object
creation.
There's something about this declaration that I can't seem to like very much. Still,
we need an actual initializer (constructor). That's very useful.

So how do we make this consistent? One thing that bothers me somewhat is how the (())
stack up so much. What can be done to improve it? So far it's actually not that bad,
it's just a little thing that is bothersome. What about taking some hints from python?
An idea that has been lingering in my mind is to use indentation for the (). This makes
the code easier to read and work with.

	class Name [(bits 32) x]:
		method x:
			bits 32 a
			= a x
			++ a
			return a

That looks kinda weird... There's something elegant to it though.

	(class name [(bits 10) name] (
		(method [] cool [] (
			(for 1 10 1 (
				(print (name))
			))
		))
	))

Actually it looks fine. One thing I wish were more explicit is if we can distinguish
templates from function calls. Distinguishing them should be useful to the programmer.
The thing about this is that LISP like grammar is simple and easy to work with. It's
a language that's extremely easy to bootstrap. That's why it's so great. Maybe it
needs to be simplified somewhat. The semantics of the templates and macros seem somewhat
ambiguous. Currently, it's assumed that a macro invocation simply doesn't evaluate
its input at all. However, it looks like a single function call. I'm considering trying
to make the grammar actually  be so simple that only hack and template are in the
language. This way, the entire base language is extremely small. Although I'm not
sure how efficient the resulting opcode would be. Perhaps to compile to some lower
level form that takes care of it. A truly generic compiler compiler compiler ... recursive.

	(hack)
	(generator)

A simple language. The only two elements can be the generator and the hack statement.
This should be an interesting challenge. The only concern is that the language may
not be able to generate code for all other languages. The reason being that it loses
its coherence. That's kinda boring.

	(hack "code here is just inserted")
	(macro name code)

It's an interesting idea. So what's left for the original implementation to conform
to? Well, it has to lex, which is parenthesis-whitespace separated. Easy enough. What
about parsing? What about semantic rules? I think those would be a lot more difficult to
implement. Here's some sample code.

	(function ((bits 32) y) gcd ((bits 32) x (bits 32) y) (
		(while (!= y 0) ((= x y y (% x y))))
	))
	(gcd 312983 219319)

So implementing everything using hack and macro should result in not being able to
evaluate things correctly? Hmmm... I'm thinking about the information that the compiler
can use. If it's kept simple, there shouldn't be too big of a problem. So let's imagine.

	(macro if [a b c]
		(hack (+ "if (" (a) "){" (b) "}else{" (c)))
	)
	(if (> 1 2) (print "Bigger") (print "Smaller"))

So macro > evaluates to (and print)

	(macro > [a b]
		(hack (+ (a) ">" (b))
	)
	(macro print [a]
		(hack (+ "std::cout << " (a) ".toString();"))
	)

This mandates that the language structures the evaluations in a certain way. Some
languages may not be able to be translated into. Still, it is becoming interesting.
The grammar remains the same, it's all just translated to something else. Is that
really useful though? Well let's answer that later. Firstly, we see that + is required
to be a builtin. Also yes, it is really useful. All we need to evaluate is a complete
set of () at a time. This means that a macro invocation should take care of the requirements.
Anything else? Well... hmmm. Actually making running code from this shouldn't be too
difficult. The compiler is essentially written in itself. Hell, bootstrapping should
be a piece of cake then!
Alright, so what is required for the base language? We already have hack, which simply
outputs the string given. The grammar uses () evaluation and a simple lexing scheme.
We need string concatenation. What about checking function types? This is something
important. If a function call merely generates code, then it is not useful. However,
if a function call also checks the input types, then we can issue an error if the
wrong type is given. This is why a macro needs to be able to investigate the type.
How does this happen? How does the compiler get type information? What can be done?

	(var (bits 32) a)
	(var (bits 64) b)
	(+= a (* a b))

Suppose var and bits and var translate:

	(macro bits [num] (
		(hack (+ "BitSet<" (num) ">))
	))

	(macro var [type name] (
		(hack (+ (type) " " (name)))
	))

Works out right? So how can += know the types?

	(macro += [left right] (
		(hack (+ (left) " += " (right) ";"))
	))

Alright, so this translates to a C++ compound assignment. So how can we know if the
left is the same size as the right? That would be useful to know. If we look at normal
compilers, it can be seen that part of the algorthim contains the information about
a variable. So does this mandate a native variable declaration scheme? Or perhaps
the macro should be recursive?

	(var (bits 32) a
		(var (bits 64) b
			(
				(+= a b)
			)
		)
	)

But then, how does the += find out about the variables? We really need to have something
that can check. Perhaps the type checking should be delegated into the host language?
Of course not. The language itself should perform all the static checks. Hmmm....
It seems we need to be able to register data at the compiler in order to make it perform
checks.

	class MyClass
		function (::) myFunction
			Out.print("Hello world!");
			if (32 > 10)
				Out.print(math.sin(32.f));
			fi
		noitcnuf
	ssalc

	var [30, [2, sml.String]] choices =
	[
		['Rødvin', 'Rødvin'],
		[...]
	];

The thing is, most languages - when compiled - get information about surrounding instructions.
This is useful to the compiler. We can't perform the variable existence checks when
the compiler just compiles into simple machine instructions. (var (bits 32) x) can
be considered as two instructions, bits 32 returns a value, that's all. It can be

	mov r0, #4

And var does:

	add sp, sp, r0

This works, but there is no way to refer to a in the assembly language. So I can not
perform static checks using this :(. I wish it were different. The simplicity is elegant.
So we're back at the original grammar? Well not yet. We're back at having var in the
compiler. I should really make a branch for this. Alright, so let's just imagine a
little...

You know. I've been really contemplating all this, and I tend to get back to the original
idea for the language. It's just that it's practical. It's useful. It's ... simple.
The LISP version is also simple, but it makes the code more ugly and more difficult
to work with.

So I think this chapter can be closed. Let's move on with life.

*Conclusion*: LISP-like grammar was considered, but deemed too impractical.
