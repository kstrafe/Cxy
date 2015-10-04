# How UL Came To Be #

I was programming and it suddenly struck me: "It must be simple".
My view on life changed in an instant. Boom. It was over. It's happening.
It must be simple.

You take professional laziness and mix it together with my love of sleep and
you get total awesomeness. So awesome that it doesn't even have a name.


# Introduction #
To make a new programming language we need to start looking somewhere.
We start looking at history. Throughout the short history of programming
we've seen languages come and go. Ideas born, and then die.
There is a constant evolution: the evolution of the languages. As languages
evolve, so do computers. Nathan's first law says `software is a gas` (
as the computing power expands, so does the software's computing requirements).
Programming languages have followed Nathan's first law. A language that
mitigates tedious operations from the programmer to the computer is called
`higher leveled` than a language that does not.

Some languages, like C++, may use abstractions that do not incur a run-time
overhead. A good example of this are what we call "templates" in C++. A template
basically generates code (Not exactly, but this will be understandable
for the non-c++ crowd). Sometimes though, C++ compilers spend a long time
processing templates during compile-time.

Other languages like Python or JavaScript dynamically (during run-time) abstract
away types by embedding them into 'objects'. This allows objects to change
whilst still being evaluated correctly. Much like dynamic polymorphism in C++,
Java, C#,...

Code generation during compile-time is essentially 'free' during run-time. It
incurs no run-time overhead, and makes programming easier, safer, and more
comfortable. It would be rather inconvenient to have to resort to wasting time
on both processor cycles as well as liters of coffee for the programmer.
Wouldn't the world be a better place if we had zero-cost abstractions, whilst
also making life super-easy for the programmer?

Imagine the programmer identifying an interesting problem, with a tedious
solution. Surely he can write five-hundred switch-case statements with
stringify methods for each special case to solve the problem. That's no problem.
The problem is that we're wasting precious oxygen.

To remedy the problem, the programmer could create a code-generator in some
scripting language. This would be easy and fast - if he knew a scripting
language. The trouble with using another language is that it's non-standard.
It's a hack. And when requirements change, the entire code-generator may not
be able to recognize configuration.

So the programmer adds a tonne of functionality to the code-generator and it
becomes a grand mix of spaghetti and unrecognizable sauce with chunks so small
that they're difficult to study.

Having code-generation would be nice. Actual injected code, that is.

Code generation is not the only thing to consider. An interesting  programming
language is "Haskell". Haskell has this fantastic system where we
consider functions as "pure". Unfortunately, Haskell as a language is
remarkably impractical, and more on the forefront of theoretical language
research.
We're not looking for that.
We're looking to solve problems. Fast. Safely. Cleanly.

Functional purity is at the heart of cleanliness and safety. The code base
becomes a lot more reliable with pure semantics. The fact that you can
appoint an entire sub-tree of code as "pure" means that you can move that tree
pretty much anywhere, call its top-level function, and it'll work. Guaranteed.
It won't read or write mutable state, and it will not return different things
for the same input data.

That's nice to know. Now what's this "sub-tree" and "top-level" I'm talking
about?

Java is a programming language where we can take this from (as well as Python).
In Java, the natural thing to do is to list dependencies from the directory
that the compiler gets invoked from, also known as the "classpath". This means that
if we have two folders with one file each:


    Root/ | a/ -> A.java
          |
          | b/ -> B.java

Then if A uses B, it will need to import b/B (b.B in Java). Using such a
mechanism would keep dependencies organized and clean. If you've ever looked
at any C project you'll know that B.c could contain 15 different structs.
When a struct is referenced from another file that imports 20
other files, you have to search through all those files to find the struct.
This is a major weakness that I would like to address and enforce the solution
of.

There is good news about purity and graph-theory.
Trees and purity fit well together. In fact, they fit so well that purity can
be expanded into different domains. Not only are pure functions possible, but
so are pure trees.

    pure tree:
      Create an object 'a_' of a pure sub-tree.
      Perform a finite amount of operations on 'a_'.

      Create an object 'b_' of the same pure sub-tree as 'a_'.
      Perform the same operations as performed on 'a_', on 'b_'.

      The two objects should be in an exactly equal state.

This makes sense because we can only guarantee purity if the sub-tree
(from here on referred to as module) does not access any other data that
might make them unequal. we can not guarantee modular purity if some data is
read from an implicit variable, or from a socket.

Good, we got a few ideas written:
* Meta Programming
* Functional Purity
* Modular Purity
* Tree Structure

What about platform dependence? Ah yes, something Java has always boasted about.
Never forget: it's not "Code once, run everywhere", it's "Code once,
test everywhere". Still, it'd be nice to have _some_ guarantees. In any case,
having all our pure modules work on every platform is a requirement. After all
, it's a computation. It's logic. No Input/Output. No OpenGL rendering. No
printing. Pure parts should be fully portable.

One thing that has always annoyed me is the inability to define your own bit
requirements. I sometimes want a 15 bit integer because I just want it.
Just do it, I tell the compiler, but it doesn't want to. I'd like to have this.
Imagine you are absolutely certain you need 19 bits for your algorithm. On
many processors, the compiler can assign a 32 bit integer (which you never see).
On some exotic processor it may use 25 bits for your 19 bit integer.

Whatever happens, you as the programmer are guaranteed correct semantics for
those 19 bits. You give the compiler the ability to optimize for speed and size
on the platform you are compiling on. That's a good thing to have.

For the non-pure parts, it's probably fine having a wrapper library (not part
of the language) or just using a `static if`. A static if is evaluated during
compile time. This way we can check if we need to call system with "cls" or
"clear", depending on the platform. The nice thing about the static if is that
the program will compile even if the body uses module(s) that don't exist. The
reason for this is because the static if truly makes code invisible if its
condition is false.

Oh, and something I've always wanted: a "global" tied to a class object's scope.
This "glocal" is implicitly passed to all functions called from within the
object. This makes all objects that use the glocal impure, but if the module
that provides the glocal itself does not use any global/glocal, then the sub-tree
will be pure.

* Meta Programming
* Functional Purity
* Modular Purity
* Tree Structure
* User-Defined Bit-Length Integers
* Pure Modules Portable
* Glocal

Other than that, a simple imperative statement-based language with functions
and classes, without dynamic dispatch (a non-zero cost abstraction).

To enhance readability, maintainability and refactorability we'll also impose
strict requirements on the names of variables, packages, classes, and constants.
It would be useful if all these elements can be identified by their name alone.

Good. Anything else? I think we've got most of it covered. Note that the tree
structure should be enforced such that you have only access to two paths:
1. Modules in root
2. Direct children (relative to the current module)

This prevents ugly graphs of dependencies and promotes local growth of
necessities. Of course sometimes we need to import a library like OpenGL, or
a UNIX socket library. In that case, we can safely know that only our impure
parts may import this (since OpenGL and UNIX sockets are impure).

Another idea regarding this is the handling of globals. How are globals to be
handled? A global of a pure module should be fine. Anyone in the dependency tree
who uses this global is to be deemed impure. You can imagine a global as an
implicit parameter. This would not make pure functions impure but we say it does
simply because it is _implicit information_.

What about globals that are impure? How are they to be handled? There's a
solution that I've been toying around with. Suppose there are either two
cases of impure module usage:

1. Only a single module may use another impure module. It may instantiate any
amount of objects of that module for itself.
2. Only a single instance of a global may exist.

Why would we do this? Because this forces a single gateway. With single gateways
we can avoid having many impure functions. It also allows for easier debugging
because only a single module is responsible for the actions.

As an example we'll take the following directory structure:

    .
                            | B.tul
                 | b/       | Basename.tul
                 |
                 | c/       | Cool.tul
            | a/ | Aa.tul
    Root/   |
            | opengl/ | OpenGL.tul

Suppose not a single module is pure. If Aa.tul uses OpenGL.tul, then
Cool, B , and Basename {.tul} will not be able to also use OpenGL.tul. This forces
the programmer to set up a communications channel by handling functions well.
Aa will be a handler for data from-and-to OpenGL. It is the single gateway.
The same goes for anything that uses Aa.

This restriction on impure modules forces a very clean separation of concerns.
Unless we make the impure module global. Suppose Aa makes it global. This will
mean that Cool, B, and Basename may gain access to OpenGL. Luckily, the entire
OpenGL module will only be a single entity. If we want to make more entities, we
need to change the tree:

    .
                            | B.tul
                 | b/       | Basename.tul
                 |
                 | c/       | Cool.tul
            | a/ | Aa.tul
    Root/   |
            | opengl/  | OpenGL.tul
            | oglmany/ | OpenGLWrap.tul

Suppose OpenGLWrap is merely a module that uses OpenGL. It is automatically
impure. When Aa now declares OpenGLWrap global, one instance is created.
This single instance is however able to create many instances of OpenGL.

Again, we're forced to always go through OpenGLWrap as a gateway, as we're
unable to access the contained OpenGL object because they're impure.

Let's look at pure dependencies and how they are more friendly (this promotes
using pure modules.):

    .
                            | B.tul
                 | b/       | Basename.tul
                 |
                 | c/       | Cool.tul
            | a/ | Aa.tul
    Root/   |
            | array/  | Array.tul
            |         | GrowableArray.tul
            |
            | datastructures/  | Tree.tul
            |                  | List.tul
            |                  | Vector.tul
            |
            | customstrings/  | UTF8String.tul

Suppose all but the a/ sub-tree is pure. array, datastructures, and customstrings
are all pure sub-trees. In fact, all containing files have to be pure in this
case. Now suppose that Aa wishes to use UTF8String, but so does B. Is this
allowed? Yes! Because they're pure it's perfectly valid and even encouraged!
This way we move as much of the logic to the pure modules. This improves
cleanliness of the code.

Aa can also declare a UTF8String global, however, B, Basename, and Cool can
still create new instances of UTF8String as they see fit.

This hardcore practice enforced by the compiler can make designs better, more
reliable, easier to manage, and above all: easier to understand.

To summarize (TL;DR):

* Meta Programming (Code generation)
* Functional Purity
* Modular Purity
* Tree Structure
* Relative and classpath includes
  * Only allow direct child access. No transitive access. Deeper parts are to be
  considered internal - 'private' - modules.
* User-Defined Bit-Length Integers
* Pure Modules Portable
* Glocal
* Single entry-point impure global
* Single entry-point impure usage


## The rest of this document ##
For the remainder of this document, the language will be developed from its
entry point to its most precise detail.

## Version 1.2.0 ##
The standard GNU versioning scheme will be applied.

# General Note #
The sections are divided into different topics. Each topic starts with possible
cases/uses. Then we choose one and give a reason. The result is stated at the
end. The result is in a grammar specification. All characters are terminals
except for <:enclosed:> lexemes. These are non-terminals. Any point in the
topic may be interjected with the "tangent"s. Tangents refer to another topic in
order to clarify what is later used in the current topic.

The format of a rationale-entry is:
* Problem - A short description of the problem that needs to be resolved
* Discussion - A discussion of potential solutions
* Conclusion - A subset of the discussion is presented as the solution

An additional entry called the "tangent" may be added. This is placed such that
it resolves any unknown dependencies of the conclusion or discussion of the
problem. It can be peppered in anywhere. Let's start with the rationale!

# Index #
* Entry Point
* Function Declaration
* Variable/Type Declaration
* Plain Old Data (POD) Types
* Reference Types
* Naming Scheme
* Operators
* Classes and Data Structures
* Globals and Glocals
* Const Correctness and Purity
* Module Organization

## Entry Point ##

*Problem*: Where does the program officially _start_ execution?

*Discussion*:
Let's look at a few cases.

1. `label:`
2. Start from the beginning of a file (from the top to the bottom).
3. Use a standard function e.g.: `int main(int argc, char *argv[]){}`
4. Use a cyclic dependency graph.

Case three defines a function as the start of the program. The label
is too assembly-esque and resembles a jump. Neither is it 'callable', only
goto-able. The start of the file is not really useful as it does not allow us
exactly differentiate function declarations from actual running code. I think
this makes people understand the code less. A cyclic dependency graph is very
versatile (computations based on what is used); however, it does not look like
something that is simple to manage. If it is complicated for a computer to
parse: it is also complicated for humans to understand and reason about it.

We add the requirement that each file can have its own enter function and the
compiler can compile with respect to running a certain enter function.
Reason: Sometimes it is useful to have 2 different enter functions so either can
be run. They can be declared in the same project without copying a lot of the
code.

*Tangent*: Function Declaration.

*Conclusion*: Entry points are defined as

    (:) enterProgram
    {
      <:code:>
    }


## Function Declaration ##

*Description*: Decide function declaration syntax.

*Discussion*:
There are a wide variety of different methods for defining functions. Here are a
few.

1. C-decl: `<:return_type:> <:name:> ( <:arguments:> ) [ ] { <:statements:> }`
2. C-style: `<:return_type:> <:name:> ( <:arguments:> ) { }`
3. Java-style: `public class <:Name:> { public static <:return_type:> <:name:> ( <:arguments:> ) throws <:exception_list:> { <:statements:> } }`
4. Haskell-style: ` <:name:> :: <:type:> -> <:type:> \n <:name:> <:arguments:> = <:code:>  `
5. Lisp-style: `(<:name:> <:arguments:>)`
6. Matlab-style: `function [ <:return_names:> ] = <:function_name:> ( <:arguments:> ) <:code:> end`
7. Python-style: def <:name:> ( <:argument_list:> ) <:indent:> <:code:> <:unindent:>
8. Javascript-style: `function <:name:> ( <:arguments:> ) { <:code:> }`

*Tangent*: See `Variable/Type Declaration` for choice of type within the
function signature.

I am inclined to look at a modified Lisp-style, why?
Let's look at what appears practical

    () -> () functionName       // Quite a large signature

    () functionName             // Smaller signature, can this allow multiple
                                // returns?

    void() functionName         // Can only allow single returns, because there are
                                // no nested returns

    (32s a, char b)(64s c) functionName  // Note: '(' in the beginning is required to
    give the LL(1) parser information that it is a function.

    (32s q, 8u c):(64u e) functionName // Return args on the left.

    ((:) b):(30u a) functionName // Returning a b void function

    (ptr (:) b):(18u a) functionName // Returning a b, a pointer to a void function.

    (ptr (->) b)->(32s a) functionName // Returning a b, a pointer to a void function, using arrows

    (32u a_, 8u b_ : C c_) functionName


Why use arrows when typing is cheaper using ':'? The function declaration will
have to start with a '('. What if we want a variable of - say - a class to be
assigned functions? Use function pointers. This means declarations starting with
`(` are static functions that are nonassignable.
Having a collected signature of functions is very clean and useful. It makes the
language terser and is also easily parsable. Not to mention more easily
understandable due to the nature of nested functions:

    (ptr (:) function_match : 32u table_entry) getFunction

*Conclusion*: Function signatures are to be written in the following form.

    ( <:type:> <:result_name:>, <:arguments:> : <:type:> <:argument_name:>, <:arguments:> ) <:function_name:>
    { <:code:> }

An example of this.

    (64u factorial, 32s error_code : 32u to_calculate) computeRawFactorial
    {
      var 64u factorial(value: to_calculate);
      to_calculate -= 1;
      while (to_calculate > 1)
      {
        factorial *= to_calculate;
        to_calculate -= 1;
      }
      return factorial: factorial, error_code: 0;
    }


## Variable/Type Declaration ##
*Tangent*: See `Plain Old Data (POD) Types` for declaring PODs.
*Tangent*: See `Naming Scheme`.

*Description*: The style that is desired must be easy to parse. For consistency,
types must avoid `cdecl`. Type declarations declare an object of a type.

*Discussions*:

Type reads from left-to-right.

1. `const int *const variable_;`
2. `variable_ is const ptr const int`
3. `const ptr const int variable_`
4. `variable_: const ptr const int`
5. `variable_: int = 100`

Number four and five are a little more tricky to parse. Note that variable_ will
be an identifier. The problem with this statement is that we can allow arbitrary
expressions as statements (function calls mostly). We can not know until we
parse the ":" token that this is a declaration. This makes the grammar more
error-prone. The same goes for a human understanding. The human needs to read
until he gets to the ":". Only then will he understand that it is a declaration.

Number one and three are also interesting. I'd like to avoid the star in one
because it can be ambiguous with the multiplication operator. The last option is
number three (note that option two is removed along four and five).
This is LL(1) parsable (a predictive parser). The FIRST(statement) is different
for each production. Reads easily from left-to-right.

=== Kevin R. Stravers -- 15/07/2015

Due to the the parser being LL(1), and expressions accepting types as their
first token, this causes a conflict. An LR(1) parser easily fixes this, but it
is decided that an LL(1) parser is to be used. An example of the ambiguity.

    a.Ab a_;
    a.Ab.a_();

The former is a declaration, the latter is a function call (an expression) on a
function pointer a_. To avoid the ambiguity, we can use a tie-breaker in front
of this.

    var a.Ab a_;
    expr a.Ab.a_();

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

    var 32s x_ = 100, y_ = -400;
    var alpha.AlphaManager manager_;

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

*Conclusion*:
var <:type:> <:identifier:> <:optional_initialization:> ;

    var ptr 32u value_ = new 32u(100);
    var const ptr 32u value_(new 32u(100));
    var 82s value_(123456789123456789123456789);


## Plain Old Data (POD) Types ##

*Description*: How do we specify plain old data types? These are the fundamental
types every turing machine uses. These are bit sequences and often integers.
In some cases, floats are also considered PODs. Another POD is a pointer.
Simplifying this: anything that is not a compound data structure is a POD.

*Discussion*:
Let's look at a current proposal for POD types:
`<:denary:>`s:

    32s
    64s
    100s
    10s
    9s
    50s
    47s


Specifying an unsigned integer:
`<:denary:>u`:

    32u
    64u
    100u
    10u
    9u
    50u
    47u


Why not use u32 instead? The lexer will be able to pick this up faster? Suppose:

    2000000000s // 2 Signed Gigabit
    s2000000000 // 2 Signed Gigabit

What reads more easily? I have to say both are tempting candidates. There is
just something that irks me about the second one. We know a variable name must
start with either an underscore or an alpha character. If the token starts with
a number, we know that we only need to check the last character to see  if it is
a type. What this clashes with is that `s2000000000_` is a valid variable name.

Anyhow, utilizing massive bit integers is often not feasible. In most cases, the
highest values go into the thousands of bits. The length of the string that
describes the data type is so short that it should not be a problem for anyone
reading it to quickly find the "u" or "s".

Specifying a pointer type:
ptr <type>

    ptr 32u
    ptr ptr ptr 8u


Specifying a reference type:
ref <type>

    ref 32u
    ref ptr 9u


I'd like these references and pointer types to avoid the nastiness that cdecl
has given us. cdecl is difficult (and ugly) to parse whereas a keyword like ref
and ptr are easy and unambiguous.
A nice thing about ref is that it can be considered a true reference to the
compiler, and hence be optimized. ref can even be used to alias local variables
without any overhead during run-time (refs can not be reassigned).

Another very import pseudo-POD is the plain array. It would be highly beneficial
if the code can be read left-to-right. Let's look at my proposal:

    [32u, 10]
    var [32u, 10] var_name;

Notice how the number of elements follows the type that is going to be in each
slot. This can be read as: `Create an array containing 32u with 10 elements`.
How does this nest?

    [[32u, 5], 10]
    var [[32u, 5], 10] var_name;

Becomes `an array of an array containing 32u with 5 elements with 10 elements`.

That seems confusing. Maybe we should switch flip the order?

    [5, [10, 32u]]
    var [5, [10, 32u]] var_name;

`Five elements of an array of 10 elements of 32u`. Wow! That rolls off much more
nicely. This should be a valid type and degenerate to a pointer to the start of
the array. The declaration above makes it easy to visualize as well, as we can
read and draw from left-to-right:

          0-----1-----2-----3-----4
          |     |     |     |     |
    0    32u   32u   32u   32u   32u
    1    32u   32u   32u   32u   32u
    2    32u   32u   32u   32u   32u
    3    32u   32u   32u   32u   32u
    4    32u   32u   32u   32u   32u
    5    32u   32u   32u   32u   32u
    6    32u   32u   32u   32u   32u
    7    32u   32u   32u   32u   32u
    8    32u   32u   32u   32u   32u
    9    32u   32u   32u   32u   32u


How do we access an array? Well since [] haven't been used before in an
expression, we can add them to any variable as a suffix:

    var [10, [20, 32u]] values_;
    // Initialize the values...
    assert(values[3, 5] == values[3][5]);

Note that the comma-array-access-notation is syntactic sugar. If only that
notation is used then the compiler may be able to lay them in memory as a single
array. This often aids efficiency.

*Tangent*: See `Reference Types`.

*Conclusion*:

    <:type:> ::= <:number:><:type_of_int:>
    <ptr|ref><:type:>
    <:type:> ::= [<:type:>]

Note that the lexer will perform the operation of recognizing the tokens. The
parser never observes a separate "64" and "u"... but this is for the compiler
writer to decide.

An example of the above grammar:

    var ptr [5, 8u] a_;
    var ref ptr [5, 8u] b_ = $a_;


## Reference Types ##
*Description*: How are references passed around?

*Discussion*: The language ought to be quite verbose in the way that allows one
to infer what is going on without hopping around in different files. For example
, it would be comforting to know whether a function takes in a reference by
looking at its invocation.

    var 32u a_ = 1000;
    ClassName.doSomething(arg_: ref a_);

ref could be both an operator, and used in declarations, but that seems rather
ambiguous. This also requires us to consider the dereferencing, address-of, and
reference-of operator. There are some symbols that we should consider...

    var 32u a_ = 1000;

    $a_       // Let $ return the address-of
    $$a_      // Let $$ return a const-address-of, because an address of an
              // address is nonsensical.

    @a_       // Let @ dereference a type.
    @@a_      // Let @@ return a const-reference, because a reference to a
              // reference is nonsensical.

    ^a_       // Let ^ return the actual object pointed to.
    ^^a_      // Let ^^ return a const-object that is pointed to.


Imagine calling a function that takes a pointer to a const variable, but your
local variable is not const. You need to be able to infer from the call that
the pointer sent into the function will point to memory which will not be edited
by the function.

  var 32u a_ = 1000;
  doSomething(arg_: $$a_);
  assert(a_ == 1000);

We can safely say that no matter how that `doSomething` operates, it will NEVER,
ever, change the value of a_ locally. I don't like the ^ notation though. That
seems superfluous. Let's try the following.

    (:) functionName
    {
      var 32u a_ = 1000;
      doSomething(value_: $$a_);
    }

    (: cref 32u value_) doSomething
    {
      if (@@value_ == 300)
      { doSomethingElse(); }
      else
      { doSomethingOther(); }
    }

Let `cref` be syntactic sugar for `const ref`. Here, taking a pointer is
automatically casted to a reference type. That seems to work for local
references as well.

    (:) functionName
    {
      var 32u a_ = 1000;
      var ref 32u b_ = $a_;
    }

A reference can be considered simple syntactic sugar for `ref X` ->
`const ptr X`. Maybe cref needs to change to refc, since that's more precise.
It is sugar for: `refc X` `ref const X`. Automatically casting pointers to
references seems like a good idea. In no case does it give us any useful
information about the function if we know if it holds a pointer or reference to
our variable. If it's a pointer it can potentially change the pointer value, and
that doesn't matter to us at all, since that's local inside the function. The
best thought to have is to just consider it a reference due to the semantics.

*Conclusion*:
  We use:
  * $ as the address-of operator.
  * $$ as the address-of-const operator.
  * @ as the dereferencing operator.
  * @@ as the dereferencing-to-const operator.
  * A pointer is automatically casted to a reference.


## Naming Scheme ##
*Description*: How are names consistently given to different entities?

*Discussion*: Many languages use a very lax and liberal naming scheme. `abc` can
refer to a function, variable, package, namespace, class, etc. I thin this can
be improved by restricting the name (a subset of all possible strings) to
specific distinct entities.

*Conclusion*:
package: `abc1`. Only alpha and numbers allowed, lowercase, no underscore.
variable: `abc1_`. Only alphanumerics allowed, lower/upper -case, underscore
mandatory.
function: `doSomething`. Only alphanumerics allowed, lower/upper -case.
Mandatory beginning and ending lowercase and at least one uppercase character.
class: `ClassName`. Only alphanumerics allowed, lower/upper -case. Mandatory
first upper case character. Mandatory ending lower case.
constant: `END_OF_LINE`. Only uppercase alphanumerics. Optional underscores.
keywords: same as package names. These are reserved. No packages can have these
names.


## Operators ##
*Description*: Should we allow operator overloading?

*Discussion*: Let's look at the three types that I can think of right now:

1. Liberal overloading
2. Conservative overloading
3. Restricted overloading

Liberal: can define any new operator. This is essentially a function declaration
with a group of symbols like `:?*=|` as a valid function name.
Conservative (C++): can overload any operator that is already well-defined on
plain-old-data types.
Restricted (Java): no operator overloading/creation at all.

So what shall we choose?
Let's look at some common operators.

    1 + 2;
    3 * 5;
    9 - 3;
    4 / 2;
    400 % 3;
    1 << 3;
    1000 >> 3;
    ==, !=, >=, <=, >, <
    ||, &&, !, ~, |, &
    , . ->

The conservative model is very useful when working with custom data types that
have well-defined semantics over addition, multiplication, etc.:

    var Vector vec1_(1, 3, -2);
    var Vector vec2_(10, -5, 4);
    var Vector vec3_ = vec1_ + vec2_;
    var Vector vec4_ = vec1_.add(vec2_);
    var Vector vec5_ = vec1_.+(vec2_);

The plus can be considered a member function of the Vector class.
Seems like a strong case for allowing at least conservative operator overloading
, right? I remember using the GLM library in C++. Its operator overloading made
code very understandable. Later I tried doing BigNum operations in Java and the
code was hard to read and quite unintelligible. I think it's a good idea to
allow at least conservative syntax overloading. A very liberal overloading style
(you can define any operator) is too much, I think.
Nobody is able to read code that says: "object_a ==¤(/#)-?+.. object_b;".
Function names should convey information about what the function does. The
common mathematical operators do that already.

How will the syntax be for overloading?

What about the different operator types and define their semantics:

    // Arithmetic operators
    +     // Add two elements together.
    -     // Subtract the left side by the right side.
    *     // Multiply two elements together.
    /     // Divide two elements.
    %     // Take the remainder of the left side with respect to the right side.

   	// Comparison operators
   	== 	  // Equal to
   	>= 	  // Greater than or equal
   	<= 	  // Less than or equal to
   	!!=   // Not equal to
   	>	    // Greater than
   	<	    // Less than

   	// Compound operators
   	+= 	  // Add to
   	-= 	  // Subtract from
   	*= 	  // Multiply bu
   	/= 	  // Divide by
   	%= 	  // Modulo by

   	&&=   // AND by
   	||=   // OR by
   	^^=   // XOR by

   	&=    // Bitwise AND by
   	|=    // Bitwise OR by
   	^=    // Bitwise XOR by

    // Boolean operators
    &&    // Logical AND
    ||    // Logical OR
    ^^    // Logical XOR
    !!    // Logical NOT

    &     // Bitwise AND
    |     // Bitwise OR
    ^     // Bitwise XOR
    !     // Bitwise NOT

    // Return argument operators
    ~     // Extracts the specified return parameter from a function call

    // Reference operators
    $      // Gets the address of a variable
    $$     // Gets the address to a const variable
    @      // Dereferences a pointer to a variable
    @@     // Dereferences a pointer to a const variable

This is interesting because this would deprecate C-like non-operations like `!!`
. In C, this operation would mean to NOT twice, which is nonsensical. I think
C might've had the idea that, since ! is a unary operator, that it would not be
useful to force groupings of symbols to mean something else. This needs to be
conveyed clearly to anyone using the language. Symbols are always grouper
together. Very important. Anyhow, I'm still thinking a bit about the reference
operators.


*Conclusion*:

    (Vec3 result_vec : cref Vec3 left_side, cref Vec3 right_side) +
    {
      var Vec3 temp_;
      temp_.set
      (
        x_coord: left_side.x_coord + right_side.x_coord,
        y_coord: left_side.y_coord + right_side.y_coord,
        z_coord: left_side.z_coord + right_side.z_coord
      );
      return result_vec: temp_;
    }

    As well as the operators
    1 + 2
    2 - 3
    3 * 7
    113 / 2
    8 % 3

    true && false
    true || false
    false ^^ true
    !! false

    0b0101101 & 0b1011
    0b0101101 | 0b1011
    0b0101101 ^ 0b1011
    ! 0b1011

    doCall()~x_val

    32u a_;
    ptr 32u a_ = $a_
    @a_ = 15;


## Classes and Data Structures ##
*Description*: How are classes defined, described, and implemented?

*Discussion*: I'll assume you're familiar with object-oriented programming. It
has been an incredibly useful tool to abstract away functionality in objects.
A thing to notice is how OOP (Object-Oriented Programming) is emulated in C.

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
    ClassName_init(&name, 3.f, -5.f); // Allocate
    printf("%f", ClassName_getLength(&name));
    ClassName_destroy(&name); // Deallocate

While being explicit is a good thing, it is sometimes best to have a duck-typing
pattern that C++ employs in its templates:

    ClassName name(3.f, -5.f); // Already automatically init'd
    printf("%f", name.getLength());
    // Automatically destroyed


Notice how much shorter, clearer, and safer this version is. The one thing it
forgoes is type declaration. You will not be certain what the type of an object
will be deeper inside the code:

    // Lots of code
    name.setSomething("X");
    // More code

Is this a problem? If we choose appropriate names, this problem generally
resolves itself. Personally, I tend to use plural for collection types (lists,
vectors, arrays, maps,..), and singular names for entities.

Our programming ought to be such that modules are of a medium size. At max
a thousand lines. Our functions ought to be no more than 100-200 lines verbatim.

This allows us to still understand and comprehend what the types are - and hence
- what the operations do. It also allows us to write sufficiently terse code
that does not become an annoyance. Also notice that when we have an instance,
its type is always defined from either: the entry point (function signature),
the class's fields, function-local data declarations, and last, a glo(b|c)al.
Only the glo(b|c)al is a little tricky to find out. The other make it easy to
find out which class you're working with. Even then, we can probably insert
static assertions of glo(b|c)als that do two things: They assert that their
dependency is of a specific name (a type). They document what they need.

Let's not forget that we wanted a directory structure for the namespaces or
packages and filenames for the classes. An idea I've been playing with is to
remove all class boilerplate code. Suppose that a file contains the following
code.

    private 32u counter_ = 0;

    public (:) countSomething(32u number_count)
    {
      counter_ += number_count;
    }

    public (32u counter_value:) getValue
    {
      return counter_value: counter_;
    }

As one can observe, there is no syntax for declaring a class. There are only
files, and there is one class per file. Can code generators create classes?
They certainly can, but they have to create the files in order to implement
the specific class.

*Conclusion*:
Classes and their objects are defined as in C++. Their methods accessed via
the instance, and not the name of the methods (like in C).


## Globals and Glocals ##
*Description*: How are glo(b|c)als to be implemented?

*Discussion*: Every language worth its salt needs at least some form of global
communication. Even though it would be great to just pass everything as a
message, we quickly run into problems...

First and foremost: the biggest problem with parameter message passing is that
function signature start becoming unwieldy. Especially if there's a chain of
functions where only the last function actually uses the variable as a decision.
This leads us to globals, but they are ugly. Globals are globally accessible by
everyone. This means that it is dangerous. You never know for sure how the
variable is affected, causing 2^globals - 1 potential unknown and unhandled
communication errors.

The benefit of not having to pass so many pass so many parameters fades quickly.
There has to be some kind of middle-ground that has the benefits of both keeping
the program easy to debug, whilst also preventing insane function signatures.

I shall present you to the "glocal". This is a global tied to a single object.

    glocal restricted 32u action_stage = 103;

    private Child child_node;

    public (:) doComputation
    {
      var 1u y_n = child_node.doSomething();
      if (y)
      {
        ++action_stage;
      }
    }

Note that the above example does not require us to pass action_stage into
child_node's `doSomething` method, yet it was still given read access. This
allows sub-trees of dependencies to share data in a simple manner. It also
localizes data to the top node that declares the glocal.

Looking back at Classes and Data Structures reminds me that we need to know -
preferably without searching other classes - what type the glocal `action_stage`
is. We can try the following:

    // Child.tul
    extern restricted 32u action_stage;

The extern keyword should be familiar to C/C++ programmers. This keyword
declares an external dependency, not given through function parameters. It also
allows us to easily see what type `action_stage` is by searching the current
file we're in for that name.

Should we have `extern global` and `extern global`? Should the distinction not
be there? If our module declares just an `extern` variable, how will that module
be observed by us? Will we think: "Oh, look, a module. It has a dependency. If I
tie it to a custom class that provides that dependency then that'll be a pure
sub-tree. No worries."? In any case, what's the difference between a glocal and
a global on a conceptual level?

We know the global to be independent of any object's instances, yet a glocal is
completely tied to that. Do we need a global at all? Of course, one important
need is for threads to be able to communicate. Even though I do not like global
mutable threaded state, it's something that may sometimes be necessary. It's
rare, but it's potentially useful.

So must our declaration be of `extern global` and `extern glocal` instead of
just `extern`? What information do we get from this? Let's look at a module that
utilizes both types of externs:

    extern global 32u global_object_count;
    extern glocal 32u object_count;

What's actually quite useful is that it can read and/or write its own object
count used within a pure sub-tree. It can also read/write the global object
count. Is it useful to know this? I suppose it's a little more comforting
from the programmer's standpoint to see "glocal" instead of nothing. That means
it's probably not a terribly dangerous object to read or write from.

What if we want a kind of "extern polymorphism"? With this I mean that it's up
to where the module is used whether its extern is global or glocal.

Suppose we have a logging utility that is global. Would it not be useful for
a submodule to declare: `extern Logger logger;`? If a module is used by a module
that declares it glocal, then there is no problem. Neither is there a problem
when it is declare global. There is however one catch. There is essentially no
dynamic polymorphism. Luckily, this can be solved by using a code generator that
generates code with the appropriate extern.

Suppose this submodule is extracted. Now we want to use it in a location that's
per-instance of the parent module. Would it not be great to say that we could
declare a glocal without having to change the submodule we just copied into a
new project? That seems quite useful.

*Conclusion*: Globals are declared per-module, either as glocals or globals.
Modules that use these glo(b|c)als have to declare externs.

    // Inside the parent
    glocal 32u total_sum;
    global 32u error_number;

    // Inside a dependency
    extern 32u total_sum;
    extern 32u error_number;


## Const Correctness and Purity ##
*Description*: How are functional and modular purity to be defined?

*Discussion*: Because it would be ideal to put this inside the function
signature, we try the following.

    (: : const) myConstFunction
    {}

    (: : pure) myPureFunction
    {}

    (: : const pure) myConstPureFunction
    {}

Const methods can only call other const methods. All fields of the class
instance become constant when a constant function is called.
Pure functions/methods can only call other pure functions or methods on objects.

Pure modules are also possible. How will these be specified? The definition of a
pure module is as follows.

    var ClassName a_;
    a_.doOperation1(argument_1: argument_1);
    a_.doOperation2(argument_2: argument_2);
    .
    .
    .
    a_.doOperationN(argument_N: argument_N);

    ClassName b_;
    b_.doOperation1(argument_1: argument_1);
    b_.doOperation2(argument_2: argument_2);
    .
    .
    .
    b_.doOperationN(argument_N: argument_N);

    b_ is a_;

`b_` and `a_` must be conceptually identical. This means that the `==` operator
must return the same result every time we run the above algorithm, with any
number of operations in any order, so long those operations and orders are equal
for both objects. The operations can also be interleaved, as can object creation
.

Note that purity gives some trouble with `new` as an operator. This is similar
to `malloc` in C. What this does is that it returns a pointer to a newly
allocated address. The program has been given permission to read and write to
this location. It is useful to be able to allocate and deallocate memory even in
pure functions or methods. In order to do this, I think we need to make the new
function pure in itself. This stems from the definition of purity.

Now we have another problem, new can not be pure. The pointer it returns has no
guarantee of being the same. To solve this problem, we mitigate the problem into
the domain of conversion. We state that pointers returned are essentially pure;
however, converting them into anything other than a pointer is impure.

This is like quantum mechanics, where the state is in a superposition until
observed. Imagine the following:

    (32u array_: : pure) cleverAlgorithm
    {
      var ptr 32u int_arr = new [100, 32u]; // Create 100 objects of 32u
      var ptr 32u int_arr2 = new [100, 32u]; // Create 100 objects of 32u
      if (int_arr == int_arr2)
      {
        // This if is allowed. It only checks if two pointers point to the same
        // data. We know new will never return two pointers pointing to the same
        // memory address.
      }

      if (int_arr > int_arr2)
      {
        // This requires a conversion into something that allows ordering.
        // This is not allowed here: converting pointers to integers is impure.
      }

      return array_: int_arr;
    }

Notice that using the new operator on a type creates an object of the entirety
of that type. This means that `new [5, [3, 8u]]` will instantiate the entire
table five pointers to three 8u variables. If you want to prevent this, then one
can do so by degenerating to a pointer: `new [5, ptr 8u]`. This allocates five
pointers to 8u. The pointers are of unknown size and hence not initialized.
One can now loop over them and assign different sizes.

    var 32u amount_ = 5;
    var ptr ptr 8u a_ = new [amount_, ptr 8u];
    for (32u i_ = 0; i_ < amount_; ++i;)
    {
    	a_[i_] = new [i_ + 1, 8u];
    }

The above code should create the following two-dimensional array.

    O----O----O----O----O
    |    |    |    |    |
    8u   8u   8u   8u   8u
         8u   8u   8u   8u
              8u   8u   8u
                   8u   8u
                        8u

*Conclusion*: To declare const and/or pure methods, and pure modules, we write
this property in the signature.

    (: : const) myConstFunction
    {}

    (: : pure) myPureFunction
    {}

    (: : const pure) myConstPureFunction
    {}

The `new` allocation function is pure. Casting a pointer is impure.


## Module Organization ##
*Description*: How are modules and their dependencies organized?

*Discussion*: We want to be able to always have a good overview over a project.
Already stating that classes are specific to a single file forces quite a bit of
organization already. It is important to consider the cases that are going to be
most practical.

Since namespaces/packages are directories and classes are files, we can look at
their trees. I will formalize a tree format using indentation.

    root/
        a/
            aa/
                AdvancedAlgorithm.tul
                Mergesort.tul
                HeapSort.tul
            Alpha.tul
        b/
            boson/
                ControlReactor.tul
            Beta.tul
        c/
            codegenerator/
                supergenerator/
                    CodeGen.tul
                Without.tul

An idea is to state the following:
  A dependency is only located from:

  * Sub-tree directories - without transitivity
  * Root directory - without transitivity

This means that you can only access direct subdirectory modules, but you can not
enter their subdirectories again. The same goes for what is accessible from root
. This forces isolation and dependency management.

The problem with the above proposal is that it's impossible to define sub-tree
local protocols. Suppose my `Alpha.tul` as shown in the above graph would like to
communicate to the different algorithms using a protocols that is intrinsic to
those algorithms. This forces that protocol (a module basically) to reside
in the root folder to be accessible to all. But, you don't want it to be
accessible to everyone! It's only there to make working with aa/ more easy
for `Alpha.tul`. How can this be solved?

We should probably dive a little into graph theory. The common ancestor bound is
where all the dependencies will meet. The common ancestor of `Alpha.tul`, and
the modules in aa/ have a common ancestor which is the folder a/. Can this
information be used and can it be applied such that the structure of projects
becomes easy to manage as well as clean?

I think I have a solution. With inspiration from how the internet works. We
include a dependency by walking up the tree. First we try the descendants, then
the current folder, the parent, that parent's parent, etc... root.

Is this a good solution? It somehow manage itself under the dynamic discovery
problem. It also allows us to promote certain modules to a higher level if they
are requested there. The problem this creates is that all classes ought to have
different names. Is it really that much of a problem though..? We could say that
all classes can have the same name so long they are not in the same folder. I
strongly dislike the idea entirely because it makes us - the programmers - look
for dependencies manually. That's something to be avoided.

At least the programmer will know that the module resides either in the children
or the sub-tree above, but not all the branches. This makes it a tonne easier to
locate. I still don't like the idea because it's too tempting to state that a
module only has access to its children and root. That would make projects flat
and clean.

We could just conclude by saying: "Well, if they use that, just put it in root".
It's tempting, but there has to be a better solution to this.

A possibility is to look at the glo(b|c)als. These are declared "extern". That's
rather interesting. What if we did the same for dependencies? Suppose that a
modules states that it requires a dependency upon `ProtocolEx` for some reason.

    extern ProtocolEx;

  	public (:) doStuff
    {
      ProtocolEx.doSomething();
    }

This will allow the class that uses the dependency to have it be provided for
it. Provided, that the dependency has the methods/fields that this module uses.
In the case where a extern module is absent, or the extern module fails to
provide the requested methods and fields: compilation fails. Can this method be
used to find out about where the dependency is located more easily than
conventional methods? I certainly do think so. Consider that we essentially only
have to go up in the directory structure. I still don't like the idea though.
It's somehow dirty. It's not what I intended with the directory-tree structure.

The directory-tree structure guarantees that children are in a subdirectory
specified by the one being dependent. I'd rather not have this the other way
around. Then again... what if we use this to our advantage?

    root/
        ds/
            Array.tul
            List.tul
            Heap.tul
        pr/
            TypeEnum.tul
            String.tul
        MyProgram.tul

Suppose MyProgram wants a List of TypeEnum. Certainly it could create a copy of
List with an extern dependency on TypeEnum and move TypeEnum into root, but that
is what is ugly. We can solve this by creating templatable code. MyProgram
injects the dependency when it declares usage of anything in ds/. Much like how
C++ templates or Java Generics work.

    // MyProgram.tul

    public (:) enterProgram
    {
      var ds.Array<TypeEnum> e_arr;
      var TypeEnum x_ = TypeEnum.BIG_DATA;
      e_arr.appendItem(x_);
    }

Notice how this keeps the tree separated, yet also allows their interoparability
. Utilizing these templates allows us to force a lot of parts to be truly
generic. These parts then become reusable very easily. In addition, the clean
dependency graph is kept. Making MyProgram purely dependent on only its
children. Of course, for practical reasons we can also allow root/ to be
directly accessible by all. We do not want the following structure:

    root/
        gl/
            OpenGL.tul
        pr/
            MyTrueProgram.tul
        MyProgram.tul

Where `MyProgram` invokes the templated `MyTrueProgram`. This seems rather
inconvenient... But actually. It doesn't look that bad. Can this method be
utilized everywhere though? Is it useful with generic data types as lists,
arrays, vectors, maps, trees, etc.? Let's look at such a graph.


    root/
    	ds/
    		Array.tul
    		List.tul
    		Map.tul
    		Vector.tul
        gl/
            OpenGL.tul
        pr/
        	dpr/
        		Deep.tul
        		Very.tul
        		Internal.tul
            MyTrueProgram.tul
        MyProgram.tul

Any usage of the ds/ package inside gl/, pr/ will need to be provided by the
parent of ds/, which is MyProgram.tul. The OpenGL.tul and the MyProgram.tul
will need to be templates for this to work. What if only Deep.tul utilizes a
data structure dependency (stuff from ds/)? This is remarkably similar to
the glo(b|c)al problem. We define a glo(b|c)al in order to not having to pass
the data downwards. Similarly we don't want to define every module before the
deepest module as a template merely because the deepest module uses some module.

We return to the basic assumption made in the introduction, in addition to
having recognized that templates (well, dependency-templates) are to be used in
order to pass appropriate dependencies down.

A consequence of this is that the grammar can not allow transitive member
expressions:

    a.Bb.Cc

can not be allowed, yet

    a.a_

is completely fine. You're accessing a variable inside a package. Wait,... is
that even possible? I suppose it is with globals. Anyhow, wait. No. a package is
merely a namespace. It has no associated variables. The above is wrong.

    a.Aa

This is the correct usage. A namespace is _always_ followed by a class name. No
exceptions. The nice thing about this is that it makes a lot of things very easy
and understandable. By forcing this rule in the grammar, we exit as soon as we
spot an error of transitive packages. This saves compilation time, as lexing and
parsing are quite cheap compared to semantic analysis and optimization. But
let's not get into premature optimization here. This keeps the work clean, and
the types sufficiently short.

    sys.Dir dir_(start_: "/");

A directory iterator can be as simple as that. Now we iterate over the
different folders.

    var sml.String paths_;
    while (dir_.hasChild()~yes_)
      paths_ += dir_.getCurrent()~child_;

That looks so awesome! Named returns! Named parameters! Operator overloading!
Flat structure! Awesome. This is going to be great.

=== Kevin R. Stravers -- 18/07/2015

Ideally, we'd have all these dependencies under the same 'name'. As such, when a
module is moved from a subdirectory into root, it will not break a program.
Likewise, when a dependency is substituted, it must also be reffered to by that
same entry. Will this create collisions? As long as a strict order is specified,
this should be fine.

*tangent*: Dependency Order

*Conclusion*:
  * Dependencies searched for in children
  * Dependencies searched for from root
  * Dependencies granted by invoking callee


## Dependency Order ##
*Description*: In which order should dependencies be searched for?
*Discussion*: The order in which dependencies are fetched need to be listed
according to their usefulness. Let's review the three types of dependencies:

* The subdirectories of the current module's directory
* The modules that are granted via the callee
* Modules that reside at the root level

Obviously, modules at the root level tend to be core libraries. Here's an
exhaustive list of modules I can imagine residing there (note that these won't
actually be in the root folder, but rather built-in to the compiler, still, they
act as if they are at the root level).

* sml.Deque
* sml.Flist
* sml.List
* sml.Map
* sml.Queue
* sml.Regex
* sml.Set
* sml.Stack
* sml.String
* sml.Vector

* sys.Dir
* sys.Err
* sys.In
* sys.Time
* sys.Out

One thing that is noticeable is how we can not descend the sys package into
subpackages. This is due to the direct-child restriction. This keeps the
libraries flat, but it may collide with other global namespaces. Generally, this
hasn't been shown to be much of a problem in C++ or C, considering that everyone
uses a prefix.

Consider that we define a structure as follows.

    root/
      |--- a/
           |--- sml/
           |    |--- Vector.tul
           |--- Aa.tul

What happens in this scenario when `Aa` references `sml.Vector`? This can be
chosen. Is it beneficial to be able to overwrite default behaviour? "Hey, the
standard vector isn't really optimal, can you overwrite it at X?". This means
that any class can easily be overwritten with a child of the same name. The real
question is whether this is desirable.

What about modules that are granted, having the same name as either global or
child modules? Would a granted module overwrite a child, or vice-versa? Let's
attempt to quantify the usefulness of each case.

First we have child overwrites global for a specific module: does the language
still hit the goals that it is supposed to achieve? Self-documenting code is
extremely important. The property of self-documentation is retained when a child
overwrites a global node. If we construct a list of locations to search, we
quickly find ourselves looking at directories in the following order:

* Children directory
* Global directory
* Compiler (as global, but for sml, sys...) directory

Does this provide a guarantee of safety and assurance? What about granted
modules? Let's look at a somewhat more advanced structure.

    root/
      |--- a/
           |--- sml/
           |    |--- Vector.tul
           |--- b/
           |    |--- sml/
           |    |    |--- Vector.tul
           |    |--- Tools.tul
           |--- Aa.tul

In the above case, `Aa` can utilize both `b.Tools` and `sml.Vector`. It can also
grant one to the other given they accept grants.

    ...
    var b.Tools<x.Storage: sml.Vector> my_tools;
    ...

The problem here is that tools already has a child called `sml.Vector`. The
names would collide. Instead, during grants, I think it's better to have the
qualified name of the grant not collide with any child. Non collision with the
global qualified names can be tricky, so that can be ignored. What if the
granted names can only be non-qualified names? With that I mean that there is no
package descriptor. It would make sense, right?

    ...
    var b.Tools(Storage: sml.Vector) my_tools;
    ...

(Note that the <> or () syntax is not yet decided.). This forces `b.Tools` to
refer to the granted storage as purely `Storage`. Nothing more. Maybe this makes
the intent of the code more clear.

    ...
    var ptr Storage store_ = new Storage();
    store_->performOperation(...);
    ...

Whenever we read that code, we immediately know that Storage must be granted, as
we can not access the modules in our own folder. Is this a good idea? After all,
wouldn't it be practical to have them be qualified names in order to overwrite
functionality? Well... here's where I see a parallel with the inheritance idea.
It's hard to follow. Nobody wants to sift through spaghetti to find what was
actually meant.

This will not allow easy refactorization. I think we can therefore make every
access be through a namespace/package first. If a granted collides with a child,
and error ought to be emitted.

*Conclusion*: Prioritize children/granted over global module access (override).
Do not allow children and granted of the same name.



## Code Generation ##
*Description*: How will syntax for code generation be defined and what will the
semantics be?
*Discussion*: Code generation is a desired ability of this language. Not in the
C/C++/LISP macro sense, but as <?PHP sense, where code is injected into a
specific point in the program. To do this cleanly I think it is important to
force every code generator to its own file, as the code generation has to
interrupt during lexing to take over the lexing stack temporarily with generated
code.

    public (:) enterProgram
    {
      var [4, 8u] my_array = ['a', 'c', '.', '?'];
      #GenerateSwitches(sw_arr: my_array);
    }

The problem with the above code is that we actually need to store contextual
information in the variables. When the lexer spots a "#" outside a string, bells
ring, machinery crackles, and the lexer notifies the callee that a generator is
coming up.

The callee then processes the generator (using a simple lexer and parser). It
calls the generator, and pushes the resulting string on a stack such that it
feeds all characters from the top into the previous lexer so that the generated
code actually gets processed.

This is probably the reason why we can't have C++-like constexpr expressions
around, being put as arguments to the generator. Instead, any generator must
fetch data not from where it is called from, but from a file or direct input.

constexpr in C++ are guaranteed to be evaluated at compile-time. An example is
a constexpr function, that will always return a value at compile-time, thus
eliminating the call. A constexpr data declaration may use constexpr functions
to initialize its data. All of this is done during compile time and makes the
program (in many cases) faster.

This means that, the codegenerator can open a config file, read it, and decide
what code to generate based on that configuration. It can also use direct
arguments.

    #GenerateSwitches(sw_arr: "['a', 'c', '.', '?']");

Allowing constexprs as shown before does not seem to be easy to do. The compiler
would need to actually store the expression somewhere, in case the generator
requests it. I think it may be better if the generator takes in strings only. A
single string. It won't need any named parameters for this, but to be consistent
we can force it here as well. I bet we can even have more strings. Anyways,
using this method, we'll be able to generate code from string input.

Is this good? Won't it be more useful to allow the already-existing compiler
parse and process an array instead of doing it manually inside the codegenerator
? Well, that is indeed easier. I suppose an idea can be taken from Scala here.

We can allow arguments that are non-direct constexprs, but they need to be `val`
types. This means that instead of `var`, we have `val`. The nice thing about
this is that it fits well with the current lexing scheme where `SOMETHING` in
all-caps is considered a constant.

Could `var` be used? Perhaps, but what I dislike about using `var` with ALL_CAPS
and `val` is that they look so alike.

    var ...
    var ...
    val ...
    var ...
    var ...
    val ...
    var ...

You kinda need to be somewhat careful when looking at these. In fact, should
constexprs be allowed, the parser is forced to report these by reporting a small
tree of the expression. So any syntax for this? Constexpr functions will also be
interesting to have. `calculateFactorial(value_: 10)` could be useful, but why
not `#CalculateFactorial(value_: 10)`? I suppose that it's just insanely
practical to be able to use locally defined variables.

Then again, having a language-agnostic code generator sounds very tempting. This
means that the cg will work outside the language as well, and is not forced to
interpret or parse the surrounding text, definitions, or dependencies. Actually
yeah I think that's the best way to go. No constexpr. This means generating a
factorial guaranteed to be at compile-time will just go through a code
generator. This means that a file can be preprocessed easily by search and
replace. Luckily we'll do something more efficient (using the lexer stack).

Anyhow, it would be useful if a code generator did not require "". Unfortunately
, this is almost a requirement, as we're unable to find out the closing
parenthesis without it.

    switch (character_)
    {
      #CreateTrueCharCasesReturn("a,b,c,d,e,f")
      #CreateFalseCharCasesReturn("x,y,z,w")
    }

The above cases look like they take up only two lines. This allows us to easily
generate huge blocks of code without trouble.

I like the idea of the codegenerator being agnostic to its context. This
guarantees that it will never generate different code when it's moved from one
location to the other. With that I mean that the invocation with a string, when
moved from one module (copy-pasted) to another. Of course, this excludes the
codegenerator being able to read files.

What about the ability for a codegenerator to read files like configuration?
Should it be able to read files? After all, it's not truly related to the flow
of code. It's probably fine. The only thing it breaks is that moving the code
may now alter the output from a codegenerator, as paths may be relative.

The same can be said about allowing code generation within code generation. When
the invocation is moved, another codegenerator may be invoked. This should not
be a problem as it mimics how our dependency structure is put into place already
. The same occurs when you move modules.

*Conclusion*: Codegenerators use information given as a string, and can never
refer to information outside of the `()`. To allow multiple arguments is more
consistent and useful when trying to avoid splitting in the code.

## Static Conditionals ##
*Description*: Conditions that are evaluated at compile time.
*Discussion*: Static conditionals are statements with the same
grammar as the if conditional. This conditional is evaluated during
compile-time. This means that it incurs no overhead of if-testing
during run-time.

    static if (a_conditional_value)
    {
      // do something
    }

The statements contained in the static if are not semantically
evaluated when the conditional value evaluates to false.
This allows the use of code that refers to non-existing
modules or libraries when these do not exist.

This can be useful for different reasons. In addition, it's
cleaner than a macro expression:

    #IfConditional(arg_: /*do something*/);

It conveys more information about the actual code. As it is
visible directly when using the static if statement.

Does this justify adding the keyword static? Why not add
a keyword like the "when" keyword instead? I think static
conveys the meaning more. In addition, static is not used
inside a function anyhow. What about outside? Can static
ifs be used there? What about the collision with static
variables?

Suppose there's another statement like `staticif`. That
looks incredibly ugly. I'd rather not. Any other ideas? What's left? Suppose we
use another word. I already use "when" for expressions to denote that this
counts for just an expression and not a statement. The same is needed here.
Although static conforms with the LL(1) requirement, it will clash with
static declarations of data. Essentially, the entire statement is actually

    #PasteAssemblyIf(arg_: "Put your code here");

The problem with this is that it just doesn't look nice at all. You put
the code inside a string. In a way, it's very clean as a string is never
parsed nor semantically analyzed if it's just not inserted. Doing
else-if-else... chains with these is going to be somewhat more difficult.
It will require running them inside another code generator that made sure
that only one returned. It's just incredibly powerful and ensures that
the expression is to be evaluated during compile time...

How does it mix with codegenerators though? Suppose we have the following

    static if (expression_)
    {
      #GenerateSomeCode();
    }
    static else
    {
      #GenerateOtherCode();
    }

Note that this is counter-intuitive. static ifs allow a single statement to be
listed (without the {}). In this case, if a # statement returns multiple
statements, this would erroneously look like:

    static if (something_)
      #Generate();

This also counts for normal ifs. I suppose we can force the code generators to
always encapsulate code with { } blocks. Will this damage anything? Yes, as it
will be impossible to put code in function signatures or generate anything
else than statement lists. Well, it could be a simple warning to any developer.

*Conclusion*: Use static if.


## Deferred Statements ##
*Description*: Allowing any collection of statements to be executed at all exit
points.
*Discussion*: It is rather useful when objects have destructors. Whenever a
scope is exited, the destructors from that object in that scope need to run.
These deallocate data and ensure that all field destructors are all destructed.

This is incredibly useful, however, sometimes, we need some custom code to run
at the end of all possible exit points. Meet the defer statement.

    {
      mutex_.lock();
      defer mutex_.unlock();
      // Do something, may throw an error, may prematurely return
    } // Always run the unlock().

Some languages use a sentry object that has a destructor that performs the
unlock as shown above. There are some problems with that. First of all, it's
not intuitive and not made to do that. Secondly, it requires an actual named
object to be instantiated. It's also not practical to have to create a new
class for your special action. The defer statement works better.

*Conclusion*: The defer statement guarantees to run its contents at the scope's
exit.


## Hack ##
*Description*: How do we allow platform-specific and low level access in a
general manner?

*Discussion*: GCC's C and C++ compiler supports the asm statement. This statement
allows you to put assembly in the C/C++ code. The syntax supports getting pointers
and other data by setting registers.

This is very useful for low level control. Sometimes the compiler for a machine
has no support for generating specific CPU assembly code. It can be useful to
allow the user to write this code himself.

The current compiler reference implementation (ULCRI) compiles to C++ (that's the
plan). It can be tremendously useful to implement a print function by redirecting
to std::cout. There are multiple options. One is to make the standard library
special. The compiler generates special code for that library module.
I think it is better to avoid that, and instead use the following:

	public (: sml.String to_print) printString
	{
		hack
		(
			var_: "to_print",
			cpp_: "std::cout << {0};"
		);
	}

Note how this allows to bind to platform-specific C++ elements very easily. This
requires some hacking on the object level of sml.String. The string is luckily not
garbage collected and the data model is as simple as the C++ version.

The hack statement combines well with the static if, allowing the compiler to
maybe generate C code, or even assembly for a specific processor. Maybe a compiler
compiles to Python and uses hack to generate python code. Some compilers may
generate Java, or ARM, or x86, or,... The list is virtually endless, with hack,
we allow endless possibilities.


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


So how is UL going to solve this specific problem? Should types be enforced? Let's
look at the possibilities

	sml.String a_ = "Type assertions are ok";
	a_.sml.String.replaceSubstring(to_replace: "ok", with_string: "probable");

The version that does not use type assertions:

	sml.String a_ = "Type assertions are ok";
	a_.replaceSubstring(to_replace: "ok", with_string: "probable");

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
	var x a_;
	a_.x.replaceSubstring(to_replace: "ok", with_string: "probable");

The problem is that "x" does not give the coder any information. The coder will
have to find the using declaration, which is just as useful as finding the actual
var declaration! So using is out of the window.

What about going one step further? What about type-asserting method arguments and
returns?

	var sml.String a_;
	a_ = __myFunction(in_.sml.String: a_)~a_.sml.String;

Actually, that gives us a tonne of information. We know the return type immediately.
Imagine the following scenario

	getBuffer()~buf_

vs

	getBuffer()~buf_.usr.Buffer

Holy shit! I immediately know what is returned! I don't even need to go to the getBuffer
function. This also allows one to write two cases:

	getBuffer()~buf_.usr.Buffer
	getBuffer()~buf_.sml.String

Providing return type overloading using the same name, instead of what the context
expects. I think this may be tremendously useful. Don't forget that the different
classes are unambiguously available in subdirectories, root, or granted. This gives
us an immediate place to look for that class. So far the idea is nice. I really like
it. It's just that duck-typing part that I kinda miss. Given a constexpr-like can
be given, it feels hacky and not clean.

First, let's write some code in UL that we port from the C++ SFML library.

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

	var x_ = y_.y.Obj.getString()~str_.sml.String;

You can infer from the right side what the type of x_ will be just by looking at
the type assertion.

Okay so, there is a distinction to be made. Variables declared locally can easily
be found and haven't shown that much of a pain in other languages. What's not easy
to find out are the input or return parameter types.

You know, it's 7 AM and I've been sitting here all night thinking about this bullshit.
I've gotta tell ya, honestly, from my own experience, this isn't actually a problem.
The core of any problem is in the dependencies. In C++ there may be typedefs,
shadowing, and define shenanigans at work. That causes ambiguous names to pop up.
The reason types are hard to track is just that. They're hard to even find.
In UL they're easy to find. Check the function scope, is it defined somewhere? No?
Check the functions' arguments. No? Check the class declaration or the class that
is referred to.

	sml.String // Okay, inside sml/String.tul
	a_ // Must be locally defined somewhere in the method
	this.a_ // A field of the current class
	Class.a_ // Defined as a global/glocal inside Class (granted)

Yeah, so finding the type is going to be a non-issue. Even if it is an issue you're
probably having a function that's too big. Break it up.

*Conclusion*: This entry is rejected for being detrimental and obfuscating.


## Access Specifiers ##
*Description*: How is data in a class object hidden?
*Discussion*: Many languages use the public/private/protected idiom. This document
has been using them for a while, but I think it can now be replaced by a lexer rule.
The rule can be simple:

	any_name // is public
	_any_name // is restricted
	__any_name // is private

	anyName // is public
	_anyName // is restricted
	__anyName // is private

The same lexing rules apply for the rest of the lexeme. What is considered special
are the starting underscores. This is in line with common conventions in languages
without data hiding methods. Instead of introducting verbose keyword, we can store
the data along the name. The following was previously utilized

	public any_name
	restricted any_name
	private any_name

What's interesting is that we can question whether restricted is really needed...
Imagine the following class.

	32u __my_number;
	const ptr const 32u my_num;

	(: this ) ClassName
	{
		this.my_num = $$this.__my_number;
	}

The above forced my_num to be public, referring to __my_number by pointer. The weakness
of that is that we need to assign a pointer. Pointers aren't nice to work with here.
It's probably okay to allow restricted variables to avoid the horrible boilerplate.

*Conclusion*: Use leading underscores to declare access mode.


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


## Concepts ##
*Description*: How can templates ensure that correct types are given?
*Discussion*: An initial idea is to let the compiler catch any errors. The requirements
are not immediately apparent to the programmer using a class. Imagine a vector class
The programmer can see the top of it, but it will be difficult to infer what is required.

Suppose we allow concepts to exist. A proposal is very simple. Given we have granted
classes, we can simply add concepts to the top of the class file.

	Type has (1u is_less : this, ref Type cmp_to) <;

There is something very appealing about this. It essentially forces var in the outer
scope as well (it used to be inside the function only). In addition, when a programmer
sees this, he will know the requirements of Type. Don't forget that the compiler
can do some important type checking as well. That is really useful. Finally, the
information given in the concepts give us all the required template parameters.

*Conclusion*: Concepts are to be forced for templates. They are of the form
	<type> has <signature> <name>;


## Namespaces and Variables ##
*Description*: How are namespaces (packages) and variables separated?
*Discussion*: This has always been an irk to me. Forcing the _ on variables
just seems so scary. I've never really user that a lot. Although I think
being clear in code, I think that maybe that is going too far.

In any case, how can we distinguish between a package and a variable without
distinguishing between their lexemes?

lexeme.Lexeme will always make lexeme a package, since we can not access
a class of a variable.
lexeme.something will always make lexeme a variable, since we can not access
a field of a package.
lexeme.doSomething(), will also make lexeme a variable.

Is there an ambiguous case? Function arguments will always be variables, and
so we're left with variable declarations. In that case, it will always be a
package name. There is, however, one ambiguity..

	var sml.String a = sml.getString()~string;

The question is: is "sml" after the '=' a package or a variable? Oh I guess I
tricked myself! that sml is obviously a variable since the subsequent token is
a method! Hmm... this seems to be a good case for allowing non _ variable names.

Can we dynamically assign stuff or in any way or shape _use_ a package? As far
as I know, a package can only be used as a prefix to a class. So in essense,
we can make x.Yname a complete lexeme, instead of having three different lexemes...

This change makes functions a LOT cleaner imho:

	(:) enterProgram
	{
		sys.Out.printLine(string: "Hello world!");
	}

instead of:

	(:) enterProgram
	{
		sys.Out.printLine(input_string: "Hello world!");
	}

In any case, the name is unambiguous. I also suspect that any input arguments can
easily be looked up or remembered, such that it is up to the API provider to
name the variables correctly, without forcing underscores. In fact, enough information
comes from "printLine" and "string" to deduce that "We print this string as a line".

Let's look at a few examples of the code getting cleaner:

	(32u out : 32u in : pure) computeFibonacci
	{
		return out: if (in <= 1) in else computeFibonacci(in: in - 1)~out + computeFibonacci(in: in - 2)~out;
	}

	(300u out : 5u in : pure) computeFactorial
	{
		out = in;
		--in;
		while (in > 1)
			out *= in;
	}

	(sml.String canonical_path : sml.String path : pure) foldRelativePaths
	{
		// Imagine we get the path "../abc/../qwerty/xyz"
		// The second comment contains the state of the special-case path "../../"
		var ref sml.String cap = $canonical_path;
		var 64u start_at = 1;
		loop
		{
			// Let '|' denote from, ".|./abc/../qwerty/xyz"
			// ".|./../"
			var 64u index: index, 1u found: found = cap.findString(from: start_at, substring: "../");

			// We have the index as follows, "../abc/|../qwerty/xyz"
			// "../|../"
			if (found)
			{
				// We search backwards from, "../abc|/../qwerty/xyz"
				// "..|/../"
				var 64u parent_index, 1u parent_found = cap.findStringReverse(from: index - 1, character: '/');

				if (parent_found == false)
					// "|../../", we haven't found a '/', so we a set the start_at to "../.|./"
					start_at = index + 1;
				else
					// Now the parent_index is given as "..|/abc|/qwerty/xyz", together with index - 1
					cap.eraseIndex(from: parent_index, to: index - 1)
			}
			else
				break;
		}
	}

	// Without comments
	(sml.String canonical_path : sml.String path : pure) foldRelativePaths
	{
		var ref sml.String cap = $canonical_path;
		var 64u start_at = 1;
		loop
		{
			var 64u index: index, 1u found: found = cap.findString(from: start_at, substring: "../");
			if (found)
			{
				var 64u parent_index, 1u parent_found = cap.findStringReverse(from: index - 1, character: '/');
				if (parent_found == false)
					start_at = index + 1;
				else
					cap.eraseIndex(from: parent_index, to: index - 1)
			}
			else
				break;
		}
	}

Let's also allow automatic up-casting (from less information to more). Any non-existing information
can be 0 bits.

I really like the factorial code above. Notice there is no return variable? Yeah... that's planned.

So whilst writing some code, I also got the idea of adding the "ifs" statement. It's really simple,
but allows any expression to be added. It's just like a case, but does not use constant
expressions. Hmm, why not then just allow a case with any expression?

	switch (something)
	{
		case a + b * c: statements;
		case u - d: statements;
		...
	}

It's an interesting idea. It's shorthand for

	if (something == a + b * c)
		statements;
	else if (something == u - d)
		statements;

That's a lot less typing and redundancy. I like it. In addition, if there are only constant
values in the switch, we can optimize it to be a true switch... I like the idea. It makes
coding a lot less tedious.

I'm digressing.

== Fri 14 Aug 2015 03:47:34 AM CEST - Kevin Robert Stravers
One thing that may be important to distinguish are enumeration constants. We know
that the compiler changes these into numbers during compile time; however, are they
fundamentally different from variables at all?

For now, the following rules are established:

	Class names: [A-Z][A-Za-z0-9]*
	Variables: [a-z][a-z0-9]
	Enumerations: [A-Z]+

What if enumerations are implicit inside the class? I'll provide an example.

	// MyClass.ul
	var 32u SOME_NUMBER, ANOTHER, NUMBER, DECLARED, HERE;
	var sml.String my_string;
	var 8u MY, NICE, ENUMS;

Now the `my_string` and other declarations should not affect the enumeration in the
slightest. In fact, the enumeration will continue... or should the enumeration be
limited to a single number that limits the enumerations? That's probably better...

	// MyClass.ul
	var 32u SOME_NUMBER, ANOTHER, NUMBER, DECLARED, HERE, MY, NICE, ENUMS;
	var sml.String my_string;

We can then align the enums to look like an enum declaration in other languages:

	// MyClass.ul
	var 32u
		SOME_NUMBER,
		ANOTHER,
		NUMBER,
		DECLARED,
		HERE,
		MY,
		NICE,
		ENUMS;

The type of the enum is dependent on class here. How can we assure that an enum is
correctly given to methods in their signatures? We could use 32u, but that would
forgo type checking. What can be done is the following:

	// MyClass.ul
	var 32u
		SOME_NUMBER;

	(: MyClass.32u enum_variable) doSomething
	{
		doSomething(enum_variable: SOME_NUMBER);
	}

This could work, but only if we allow single enums per class... What about constants?
It would be nice to have constants be recognizable by some upper case or special
characterization, but it's probably not that important.

One problem with this enum is that it uses 32 bits,... Maybe this is a good thing.
It allows the user to tweak the type manually. What if we want to use this type,
but we want to make it a different size on different platforms? Suppose some platforms
just are more efficient processing a 16 bit value, and others more efficient with
32 bit values? What about signedness? For this, a code generator can be used to
substitute the type...

	// MyClass.ul
	var #EnumSize~
		// Many more enums here
		SOME_NUMBER;

	(: MyClass.#EnumSize~ enum_variable) doSomething
	{
		doSomething(enum_variable: SOME_NUMBER);
	}

I really seem to enjoy this. For what reason? Well it's flexible, you know where
EnumSize is located. In addition, wherever you pepper it in, it can easily be changed.

	// EnumSize.ul
	( sml.String ~ :)
	{
		return ~: "32u";
	}

That reminds me, the 'standard' return parameter ~, without a name. In fact, that
is the name. It's a standard name. I like the idea. It also requires a standard input
though. Why? Because it's cleaner. We don't need to guess simple functions' argument
names. Functions such as computeFactorial, or computeFibonacci, or cos, sin, tan,..

Imagine:

	cos(0.3)~

Should this extract the standard argument, or just

	cos(0.3)

?
The latter looks cleaner. But it may confuse us about void returning functions. The
latter is more consistent when it comes to "not naming stuff". We can allow a single
unnamed parameter and return that we have to call ~. What if we don't call it anything?

	( sml.String : sml.String ) normalizeString
	{
		// ... do some string operations
		return :

Ah, see the problem? There's no way to reference the input parameter! We can use a
standard name instead, but would not that muddy the language? Calling it a symbol
like ~ seems to be more productive. This way, the function implementation can actually
access the in and out variables.

Just had a shower thought.. the issue with ~ is that the current lexer doesn't really
enjoy it, neither does it allow a clear separation between input and output. Instead,
we can opt for something that's easier to recognize as well as more consistent:

	(128u out : 32u in) fib
	{
		//...
	}

	(:) enter
	{
		sys.Out.print(fib(50));
	}

This solves the previous conundrum of allowing a single unnamed parameter! It still
forces all parameters to be named, but allows a single unnamed parameter for convenience.
This unnamed parameter binds to in as an argument, and out on the output argument!

==

*Conclusion*: Allow variables to have the same name form as packages.


## Function Naming Scheme ##
*Description*: How are functions and methods to be named?
*Discussion*: Currently, functions are named with a forced middle big case character.
An example: `printLine`.

I find this to be a little ugly, actually. Considering that we can have pointers to
functions being called as well:

	var ptr (:) my_func = $someFunction;
	my_func();  // Should work

The problem is that there are now two functions, one with a variable name, that acts
exactly like a function! I'd say that it's probably not that useful to force function
names within this context.

This also causes another question to come up: how do we overload? Well, we can use the
C++ method of overloading function pointer fetching:

	var ptr (32u something : ) f = cast(ptr (32u something : )) { $someFunction };

This causes another question to arise: what to do about this immediate redundancy?
Certainly, the double declarations are quite useless here... What I'm referring to
are the double `ptr (32u something :)` parts.

In fact, that reminds me of a potentially useful shortening.

	// expression
	f~string == f()~string

This avoids having to write () if the function returns at least something...
Can it also be used if the function returns nothing? That would be interesting...
Maybe g~ can be used to indicate that the function does not return anything, since
we're not getting a variable. The problem with this is that `g~ a` can be seen
as if a is the return parameter. Even if there is a space.

	g~;
	g~a;
	g()~a;

Where can void functions occur? Well, they can certainly not be in expressions.
The question is whether there is any ambiguity at all. There does not appear to
be one. In C and C++, the ~ is the bitwise NOT operator. That is a useful operator,
so it need a replacement operator in UL.

*Conclusion*: Use any function name, as long as it does not collide with class
names.


## Codegeneration ##
*Description*: What level of code generation is useful?
*Discussion*: I've recently started to grok different macro and text-editing
languages. Examples of these are awk and m4. These languages are perfect for
generating code or text. They are both very simple and easy to understand. Other
languages such as PHP also exist, even though PHP is more aimed at the net.

The thing about these languages is that they exist, they are specifically for
generating code, and they are really good at it. I think it may be better to
avoid implementing the code generator in UL itself. Instead, think about allowing
compiler hooks of some sort. It calls the hook on the file upon seeing diffs.

The biggest problem is that the language (UL) is itself not very practical for
editing text. There is operator overloading for strings, but it is still cumbersome.

	var sml.String my_string = "Hi!";

versus awk's

	my_string = "Hi!";

Code generators should not have terribly complex logic anyways. Well... maybe.
You know. I think it's best to postpone code generators. Instead, I'll make sure
that the code that is generated can easily be checked for correctness. An important
factor in this is going to be "concepts".

*Conclusion*: Code generators will be implemented later.

## Code Generators Revisited ##
*Description*: Can we use code generators to implement a simple language?
*Discussion*: There is an idea that lingered on my mind since I started thinking more about code generation. After all, that's what compilers and interpreters basically do.
They take our simple-to-read/edit code and turn it into a tonne of non-simple-to-read/edit code.
This really is at the core of what a programming language is. I wonder if we can
make the entire language a code generator by making all actions actual code generators.
My first idea is to use the # operator to designate a code generator. Here is an
example:

	#var `32u a; 64u b; sml.String c;'

Note how we use m4's string notation, starting with a backtick and ending in a normal '.
The `var` code generator is a builtin one. This allows us to avoid all future variable/keyword collisions.
Especially if we state that code generators created by the user must start with a capital letter.

Let's continue looking at some code:

	#var `32u a; 64u b; sml.String c'
	#inc `a'
	#call `c = `Hello World''
	#call `System.out.print(c)'

This becomes interesting as we expand the code to a function:

	#function `(32u upper, 32u lower : 64u in) enter
		#expression `a = #upper `in''
		#expression `b = #lower `in''
	'

or even a #class! I wonder how that will work.

	#class `name=MyClass
		#function `(32u upper, 32u lower : 64u in) enter
			#expression `a = #upper `in''
			#expression `b = #lower `in''
		'
	'

One thing to like about this is the simplicity and how everything is implemented per-codegenerator.
The thing to dislike about this is that there's too much noise from code generation.

Too much of anything is per definition bad. I really like the idea of code generation,
but this is probably going too far. What can be done to make it good? To make it beautiful.

*Conclusion*: Rejected

## Solidifying Grants and other Grammatical entities ##
*Description*: We need to set an example for the full grammar in its current form.
*Discussion*: The granted syntax is interesting. I wish to make the syntax generally
very easy and intuitive. Let's give it a shot...

	granted {
		A has {
			public static var 32u public_named_variable;

			(32u squared_value : this) computeSquare;
			(: this, value) setValue;
		}
	}

Ok, so granted contains a list of granted classes. This is similar to a template
in C++. How do we grant classes? Suppose the above class is named B.

	var B[A: String] object;

This seems unambiguous. '[' and ']' are grouper characters. Having a ClassName[X: Y,...]
is something that can never be confused with an array... or can it?

	var [10, B[A: sml.String]] objects;

That looks acceptable. B[A: sml.String] is a type in itself, and hence [10, B[A: String]]
is also a type. It's almost as if a non-prefixed '[]' means "Array". I'm a little uncertain
actually. It seems that it's mixing too much. The main question is how to grant classes.

	var B<A: String>

is possible, and resembles C++ templates, but then <> need to be reinstated as potential
groupers. Unambiguously, B() is a constructor call on a class. B[] is not anything.

	var [10, 32u] x;
	x[3] = 30102;

	var sml.String[Atom: 8u] y = "derp";
	y[3] = 'c'; // "derc"

In both cases, we can easily discern what is a granted type and what's an array or
index type. Another method is to create a builtin Array type, and write:

	Array(10, Type: Array(5, Type: 32u)) x;

But that appears too verbose. Regarding that, I can't seem to remember what __ meant
public, restricted, and private. Maybe it's better to spell it out...

	(32u : private static) name;
	(32u name : : private static) computeId { return 5; }

Egh, always trying to generalize too much. It's probably fine the way the grammar
is.

	granted

		A has { var { 32u a; } };
		B has {};
		C has {};
		...

	variables

		var {
			32u a;
			64s b;
			...
		};

	methods

		(32u stdout : 32u stdin : pure static) computeFactorial
		{
			stdout = stdin;
			--stdin;
			whilte (stdin > 1)
				stdout *= stdin;
		}

My problem with this is that it doesn't look very clean. For example, the variables
part has the same signature kind as the methods part, but looks vastly different.

	granted

		A has {
			(32u a : public static);
		};

	variables

		(32u a : public static);
		(64s b : public static);

	methods

		(32u stdout : 32u stdin : pure static) computeFactorial
		{
			(32u local_var:) = 100;
			stdout = stdin;
			--stdin;
			whilte (stdin > 1)
				stdout *= stdin;
		}

There certainly is an ugliness to that local variable definition in the factorial
function. Let's try and remove it...

	granted

		A has {
			(32u a : public static);
		};

	variables

		(32u a : public static);
		(64s b : public static);

	methods

		(32u stdout : 32u stdin : pure static) computeFactorial
		{
			var 32u local_variable = 100;
			stdout = stdin;
			--stdin;
			whilte (stdin > 1)
				stdout *= stdin;
		}

The idea with var is to be an LL(1) token in a local declaration. Instead of using var,
we can use an access specifier to the variable. Should this work on functions as well?
Suppose you have the following class.

	local private static Aa a;
	local private static Ba b;
	local private static Ca c;

	(34s weird_return : 1u stdin) yesOrNo
	{
		local 34s a = 3129;

		switch (stdin)
		{
			case true: weird_return = 31293;
			case false: weird_return = -312837;
		}
	}

There's an intricate difference between a method and a variable here. Does a simple
variable have any property of public, private, or static? A static is essentially a
global. Can a function be global as well? Going back to the original meaning of var
seems to be the safest way.

	(290u out : 5u in) computeFactorial
	{
		var 290u temporary  = cast[type(out)](in);
		--in;
		while (in > 1) {
			temporary *= cast[type(out)](in);
			--in;
		}
		out = temporary;
	}

A question comes up with this. It basically assigns () within a var statement as the
signature of a function. This is perfectly acceptable, however; should access specifiers
be added to that signature? Honestly I think it's rather irrelevant. This is my initial
reason for using the __ and _ suffix/prefix notation. What can be done is to set the
visibility of the name right before the actual name.

	glocal private 32u x;

	local private (290u out : 5u in) computeFactorial
	{
		var 290u temporary  = cast[type(out)](in);
		--in;
		while (in > 1) {
			temporary *= cast[type(out)](in);
			--in;
		}
		out = temporary;
	}

This also reminds me of the ability to avoid putting variables and methods in a single
particular order. The thing with codegenerators is that if they generate both code
for variables and methods, and you have multiple CG invocations of those in one file,
then it will be impossible to compile correctly. So that has to be corrected.

I like the idea of prefixing. It keeps the function signature clean. Of course stuff
like purity and const correctness are part of the signature.

Does this imply some lexer changes? Also, what about the casting operator or type
operator. Should they not be in uppercase to avoid the ambiguity of the array access
operator? Should an entirely new operator be applied? From C++ we learn about decltype:

	int main(int argc, char *argv[])
	{ decltype(argc) copy = argc; }

The following is a translation to UL:

	(8u return : 32u argc, ptr ptr 8u argv) enter
	{ var type(argc) copy = argc; }

That really seems to seal the deal. By allowing only a single name inside the type
expression, we can easily make this a grammar rule. Anywhere a type can be placed,
a `type(x)` could be placed. What about expressions? type(x.getString()~out) could
be interesting, but we get to what is known as type deduction. I think this is dangerous.
To quote the java docs:

	Humans benefit from the redundancy of the type declaration in two ways. First, the redundant type serves as valuable documentation - readers do not have to search for the declaration of getMap() to find out what type it returns. Second, the redundancy allows the programmer to declare the intended type, and thereby benefit from a cross check performed by the compiler.

This forces the programmer to always assign the returned variable to somethnig locally:

	sml.String x = y.getString();
	var type(x) z = x + " hello";

I like this idea. It allows types to be somewhat transparent, but not fully. It also
keeps everything readable. Not allowing expressions, but only simple names seems to
be the way to go!

Good, that's in order. What's next? Ah, casting! Let's give it a try. It'd rather
be consistent...

	var 43u a = 8796093022207;
	var 32u b = cast[type(b)](a);
	var 32u b = cast(type(b), a);

	var type[a] copy = a;

Hmm,... type[] seems better than (). Considering that appending it to a keyword is
something that is evaluated during compile-time, whereas () is during run-time.

	(::) enter
	{
		var 32u x = -1;
		var 64u y = cast[type[x]](x);
	}


## Lambda ##
*Description*: Introduce the popular lambda functionality
*Discussion*:
Lambdas are incredibly useful to the programmer, hence their popularity.
A lambda is a simple, inline function that can be assigned to a variable or
used as an argument to a function. The current grammar does not appear to support
this feature. Let's explore this.

	(: (32u anything:) in :) fun
	{
	}

	(::) enter
	{
		fun((32u anything:) { return 329; });
	}

The problem here is that it's impossible to discern the () from an expression or a
function definition in the call to function. This requires a small change to the
grammar.

	def (: (32u anything:) in :) fun {
		sys.Out.print(in());
	}

	def (::) enter {
		fun(def (32u anything:) { return 329; });
	}

I admit it to not be the prettiest sight in programming. Especially the argument
list not containing a def. Why is that? The idea for the def keyword is to accept
a subtree of the form:

	RunnableCode ::= def ( Returns : Arguments [: Modifiers] ) \{ Code \}

In that case, def acts as a head for the runnable code definition. Whenever a ( is
found after a "var", the ( will act as function type. Maybe var can be generalised
in this specific case. I think this needs some more work...
Maybe [] as in C++ will work. I like it. It's short and to the point, and even allows
variable capturing. That's neato.

Still, capturing stuff should not be required. Keeping the language somewhat simple
is nice. Let's decide on a how to parse a type. For now, all "types" are parsed in
the classical manner, where a ( denotes a function. A [ denotes an array. The grammar
becomes

	def Type name \{ Code \}

What about "defining" types that are not functions?

	def 32u name \{ 32553 \};

That just seems retarded, we already have `var 32u name = 32553;`. Why not just use
`var` instead?

	var (::) myFunction = \{ Code \}

I think this has been discussed previously. var is for variables. Names that change
what they contain. It's equally valid though,.. except for one part. Code can't be
assigned like this.

	def (::) myFunction = \{ Code \}

Could be the standard way of defining a piece of code. Even so, def without a function
name can be used in an expression to signify lambdas. Anything better than def? Python
uses it to define methods and free functions. `fun` is also possible. fnc fcn, dec,..
I'm not sure what should be appropriate.

	private {
		var {
			32u my_var = 3219;
			() doNothing = def () {};
		}
	}

	public {
		def {
			() enter {
				sys.Out.print("Hello World");
				doNothing = def () { sys.Out.print("Cool stuff!"); };
				doNothing();
			}
		}
	}

Matlab with its "end x" comes to mind. This is something I've actually been thinking
about due to the nature of C++. In C++, I often end up not really knowing which }
ends which corresponding opening {. I might be tempted to try and use the "end" syntax.
Not only does this make the program easier to read, but it prevents potentially nasty
bugs as well.

	private
		var
			32u my_var = 3219;
			() doNothing = def () {};
		end var
	end private

	public
		def
			() enter {
				sys.Out.print("Hello World");
				doNothing = def () { sys.Out.print("Cool stuff!"); };
				doNothing();
			}
		end def
	end public

Ugh, this clashes with the niceties of the {}, because we can deduce whether a statement
is a raw declaration or definition, or a set thereof:

	var 32u x = 353489;
	var { 32u y = 499049; 32s z = -42384; } // Ending semicolon superfluous

So,.. ideally, the following is made:

	granted String has {
		def (1u out : String left, String right) > { return ...; }
		var {
			64s wow;
		}
	}

	private { var {
		32u control_number = 100300;
	} }

	public { def {
		(::) enter {
			sys.Out.print("Heey!");
		}
	} }

The java rationale is against this, as the public/private... specifiers are hard to
follow when there are wades of code between them. Then again, var is often short and
very compact

	var {
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
		32u my_name_is = 942;
	}

This is manageable. So let's drop the def {} as well as the access specifiers. Seems
like it would not be productive to use them.

	public var type(name) some = case[type(name)](othername);

	def

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
		(if (&& (= version "1.2.7") (= compiler "ULC")) (
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
code is interpreted, then the (compiler) variable will be "ULCi", denoting that the
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
So we're back at the original grammar? Well not yet. We're back at having var in the compiler. I should really make a branch for this.
