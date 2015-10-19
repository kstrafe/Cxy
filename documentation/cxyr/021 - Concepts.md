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

=== Kevin R. Stravers -- Thu 15 Oct 2015 01:02:13 PM CEST

A change was made. Now we use something called 'grant'. This is a way of specifying
a template. The nice thing about grants is that they are quite general:

	grant Aa {
		(:) doSomething;
		restricted var 32u a;
	}

Is valid for any class, this class needs to be substituted as:

	var ThatClass[Aa: AConcreteClass] thatclass;

Now, the conundrum lies in: shall we allow grant currying or not? Currently, it makes
no sense to put a non-concrete class in this place. It just doesn't fit very well.
I suspect it's best to keep the grants one dimensional like this. If more difficult
cases come to light, I suppose code generation can be used instead.

===

*Conclusion*: Concepts are to be forced for templates. They are of the form

	grant classname {
		grants
	}

Where grants are function and variable/static signatures.
