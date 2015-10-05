# How Cxy Came To Be #

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


