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


