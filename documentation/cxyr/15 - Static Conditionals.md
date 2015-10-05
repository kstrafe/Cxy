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


