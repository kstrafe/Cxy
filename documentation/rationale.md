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

## Version 1.1.0 ##
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
      64u factorial(value: to_calculate);
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

*Conclusion*:
<:type:> <:identifier:> <:optional_initialization:> ;

    ptr 32u value_ = new 32u(100);
    const ptr 32u value_(new 32u(100));
    82s value_(123456789123456789123456789);


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

    [32u, 10] variable_name;

Notice how the number of elements follows the type that is going to be in each
slot. This can be read as: `Create an array containing 32u with 10 elements`.
How does this nest?

    [[32u, 5], 10]

Becomes `an array of an array containing 32u with 5 elements with 10 elements`.

That seems confusing. Maybe we should switch flip the order?

    [5, [10, 32u]]

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

    [10, [20, 32u]] values_;
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

    ptr [5, 8u] a_;
    ref ptr [5, 8u] b_ = $a_;


## Reference Types ##
*Description*: How are references passed around?

*Discussion*: The language ought to be quite verbose in the way that allows one
to infer what is going on without hopping around in different files. For example
, it would be comforting to know whether a function takes in a reference by
looking at its invocation.

    32u a_ = 1000;
    ClassName.doSomething(arg_: ref a_);

ref could be both an operator, and used in declarations, but that seems rather
ambiguous. This also requires us to consider the dereferencing, address-of, and
reference-of operator. There are some symbols that we should consider...

    32u a_ = 1000;

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

  32u a_ = 1000;
  doSomething(arg_: $$a_);
  assert(a_ == 1000);

We can safely say that no matter how that `doSomething` operates, it will NEVER,
ever, change the value of a_ locally. I don't like the ^ notation though. That
seems superfluous. Let's try the following.

    (:) functionName
    {
      32u a_ = 1000;
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
      32u a_ = 1000;
      ref 32u b_ = $a_;
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

    Vector vec1_(1, 3, -2);
    Vector vec2_(10, -5, 4);
    Vector vec3_ = vec1_ + vec2_;
    Vector vec4_ = vec1_.add(vec2_);
    Vector vec5_ = vec1_.+(vec2_);

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
      Vec3 temp_;
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
      1u y_n = child_node.doSomething();
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

    ClassName a_;
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
      ptr 32u int_arr = new [100, 32u]; // Create 100 objects of 32u
      ptr 32u int_arr2 = new [100, 32u]; // Create 100 objects of 32u
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

    32u amount_ = 5;
    ptr ptr 8u a_ = new [amount_, ptr 8u];
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
      ds.Array<TypeEnum> e_arr;
      TypeEnum x_ = TypeEnum.BIG_DATA;
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

    sml.String paths_;
    while (dir_.hasChild()~yes_)
      paths_ += dir_.getCurrent()~child_;

That looks so awesome! Named returns! Named parameters! Operator overloading!
Flat structure! Awesome. This is going to be great.

*Conclusion*:
  * Dependencies searched for in children
  * Dependencies searched for from root
  * Dependencies granted by invoking callee
