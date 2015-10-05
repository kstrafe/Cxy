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


