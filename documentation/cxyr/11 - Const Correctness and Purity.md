## Const Correctness and Purity ##
*Description*: How are functional and modular purity to be defined?

*Discussion*: Because it would be ideal to put this inside the function
signature, we try the following.

    (: : const) myConstFunction
    {}

    (: : pure) myPureFunction
    {}

    (: : const pure) myConstPureFunction
    {}

Const methods can only call other const methods. All fields of the class
instance become constant when a constant function is called.
Pure functions/methods can only call other pure functions or methods on objects.

Pure modules are also possible. How will these be specified? The definition of a
pure module is as follows.

    var ClassName a_;
    a_.doOperation1(argument_1: argument_1);
    a_.doOperation2(argument_2: argument_2);
    .
    .
    .
    a_.doOperationN(argument_N: argument_N);

    ClassName b_;
    b_.doOperation1(argument_1: argument_1);
    b_.doOperation2(argument_2: argument_2);
    .
    .
    .
    b_.doOperationN(argument_N: argument_N);

    b_ is a_;

`b_` and `a_` must be conceptually identical. This means that the `==` operator
must return the same result every time we run the above algorithm, with any
number of operations in any order, so long those operations and orders are equal
for both objects. The operations can also be interleaved, as can object creation
.

Note that purity gives some trouble with `new` as an operator. This is similar
to `malloc` in C. What this does is that it returns a pointer to a newly
allocated address. The program has been given permission to read and write to
this location. It is useful to be able to allocate and deallocate memory even in
pure functions or methods. In order to do this, I think we need to make the new
function pure in itself. This stems from the definition of purity.

Now we have another problem, new can not be pure. The pointer it returns has no
guarantee of being the same. To solve this problem, we mitigate the problem into
the domain of conversion. We state that pointers returned are essentially pure;
however, converting them into anything other than a pointer is impure.

This is like quantum mechanics, where the state is in a superposition until
observed. Imagine the following:

    (32u array_: : pure) cleverAlgorithm
    {
      var ptr 32u int_arr = new [100, 32u]; // Create 100 objects of 32u
      var ptr 32u int_arr2 = new [100, 32u]; // Create 100 objects of 32u
      if (int_arr == int_arr2)
      {
        // This if is allowed. It only checks if two pointers point to the same
        // data. We know new will never return two pointers pointing to the same
        // memory address.
      }

      if (int_arr > int_arr2)
      {
        // This requires a conversion into something that allows ordering.
        // This is not allowed here: converting pointers to integers is impure.
      }

      return array_: int_arr;
    }

Notice that using the new operator on a type creates an object of the entirety
of that type. This means that `new [5, [3, 8u]]` will instantiate the entire
table five pointers to three 8u variables. If you want to prevent this, then one
can do so by degenerating to a pointer: `new [5, ptr 8u]`. This allocates five
pointers to 8u. The pointers are of unknown size and hence not initialized.
One can now loop over them and assign different sizes.

    var 32u amount_ = 5;
    var ptr ptr 8u a_ = new [amount_, ptr 8u];
    for (32u i_ = 0; i_ < amount_; ++i;)
    {
    	a_[i_] = new [i_ + 1, 8u];
    }

The above code should create the following two-dimensional array.

    O----O----O----O----O
    |    |    |    |    |
    8u   8u   8u   8u   8u
         8u   8u   8u   8u
              8u   8u   8u
                   8u   8u
                        8u

*Conclusion*: To declare const and/or pure methods, and pure modules, we write
this property in the signature.

    (: : const) myConstFunction
    {}

    (: : pure) myPureFunction
    {}

    (: : const pure) myConstPureFunction
    {}

The `new` allocation function is pure. Casting a pointer is impure.



