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


