## Abstract Syntax Tree Form ##
*Description*: Describe a formal intermediate representation (IR).
*Discussion*: The parser and lexer are in themselves done. This will be a discussion
regarding the processing of the IR representation. It's not strictly grammar/syntax
or semantics related, but it may be important for future compilers.

In its current state, it's rather difficult to process the parse tree. I have very
informally specified the nodes to prune and how to prune them. I think it's important
to define a formal 'grammar' of the tree. Normally, the grammar itself acts as the
grammar of the tree; however, the abstract tree needs to cut away a lot of unnecessary
nodes.

Examples of such nodes are separation symbols (comma, colon, semicolon,...) and certain
keywords. The idea is to define an AST 'form' that can be processed independently.
An example:

	variable-declaration/static-declaration
		access-specifier
		globality-specifier
		type
		variable-name
		assignment-expression

The above needs to be read by using indentation as children. The example forgoes the
constructor initialization expression. An s-expression could also be used:

	(var access globality type varname (operator ...))

But I think it's nicer to have it in the former form. So let's handle variable declarations
first. What needs to be done to make this work? Well obviously, we need to add in
the support for constructor calls. I suppose we can just swap out the entire assignment-expression
with a constructor-call expression if needed, giving us two possible trees. Even then,
there isn't much of a difference as we can process the parts independently. For example,
suppose we add the entire declaration to a table of variables, we just store each
child of the declaration. Then, the assignment expression is also stored, and everything
is wonderful.

	type
		modifier
		type

The linearity of types is really paying off here, a type is simply a recursive modifier
plus another type. If the modifier is epsilonable, then the type needs to be a basic
type. If the modifier is a constant number (or expression) then we get:

	type
		array-size
			expression
		type

Where type is just the type that the array contains. I'm really starting to like all
this. For functions we can make a recursive list of functions:

	runnable
		statement
		runnable

What if the statement is a 'while' statement?

	runnable
		while-statement
			condition
			runnable
		runnable

What about jumps?

	runnable
		label-statement
			name
		runnable
			goto-statement
				name
			runnable
				...

This creates trees of code. All that's needed is to formalize this language into something
that can be created by the tree builder and checked by the semantic analyzer without
needing to use eachother. This way, independent trees can be built for the semantic
analyzer. The semantic analyzer can be built completely independently from the rest
of the compiler. That's a good thing. Modularity wins. So what's the gimmick? Well,
all that's needed is to define this language and build a semantic analyzer for it.
A translation scheme needs to be put in place in the tree builder, such that the parse
tree can be converted into an AST. I like the idea of having this IR representation,
it really gives me the ability to code directly. The question is whether the whole
language should be recursive or not. Surely each class can be made its own little
tree, but is that really desired? Why not make each class a vector representing the
different constraints? I feel as if the recusive definition is cleaner. It allows
the usage of recursive functions more easily. Anyhow, I can probably create a small
algorithm that takes in a tree representation, constructs a tree quickly, and then
I send that tree onwards to semantic analysis. What's practical in C++?

	var(
		PUBLIC,
		null,
		Unsigned(32),
		data("name"),
		null);

Can be used to indicate a variable declaration. Classes need to be made for all possible
distinctive entities of the language. An idea is to use the already existing token
type. This would allow seamless integration from the treebuilder to the semantic analyzer.
Ideally, macros can be used for declarations as 'var' to create a new ConcreteSyntaxTree.

*Conclusion*: We build the semantic analyzer (and code generator) independently from
the rest of the compiler.
