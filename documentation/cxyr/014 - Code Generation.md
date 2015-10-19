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


