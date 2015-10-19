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



