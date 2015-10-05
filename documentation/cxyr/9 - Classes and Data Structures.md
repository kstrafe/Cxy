## Classes and Data Structures ##
*Description*: How are classes defined, described, and implemented?
*Discussion*: I'll assume you're familiar with object-oriented programming. It has been
an incredibly useful tool to abstract away functionality in objects.  A thing to notice
is how OOP (Object-Oriented Programming) is emulated in C.

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
    ClassName_init(&name, 3.f, -5.f);  // Allocate
    printf("%f", ClassName_getLength(&name));
    ClassName_destroy(&name);  // Deallocate

While being explicit is a good thing, it is sometimes best to have a duck-typing pattern
that C++ employs in its templates:

    ClassName name(3.f, -5.f);  // Already automatically init'd
    printf("%f", name.getLength());
    // Automatically destroyed


Notice how much shorter, clearer, and safer this version is. The one thing it forgoes
is type declaration. You will not be certain what the type of an object will be deeper
inside the code:

    // Lots of code
    name.setSomething("X");
    // More code

Is this a problem? If we choose appropriate names, this problem generally resolves itself.
Personally, I tend to use plural for collection types (lists, vectors, arrays, maps,..),
and singular names for entities.

Our programming ought to be such that modules are of a medium size. At max a thousand
lines. Our functions ought to be no more than 100-200 lines verbatim.

This allows us to still understand and comprehend what the types are - and hence -
what the operations do. It also allows us to write sufficiently terse code that does
not become an annoyance. Also notice that when we have an instance, its type is always
defined from either: the entry point (function signature), the class's fields, function
-local data declarations, and last, a glo(b|c)al.  Only the glo(b|c)al is a little
tricky to find out. The other make it easy to find out which class you're working with.
Even then, we can probably insert static assertions of glo(b|c)als that do two things:
They assert that their dependency is of a specific name (a type). They document what
they need.

Let's not forget that we wanted a directory structure for the namespaces or packages
and filenames for the classes. An idea I've been playing with is to remove all class
boilerplate code. Suppose that a file contains the following code.

    private 32u counter = 0;

    public (:) countSomething(32u number_count)
    {
      counter += number_count;
    }

    public (32u counter_value:) getValue
    {
      return counter_value: counter;
    }

As one can observe, there is no syntax for declaring a class. There are only files,
and there is one class per file. Can code generators create classes?  They certainly
can, but they have to create the files in order to implement the specific class.

*Conclusion*:
Classes and their objects are defined as in C++. Their methods accessed via the instance,
and not the name of the methods (like in C).
