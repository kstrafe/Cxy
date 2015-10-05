## Function Declaration ##

*Description*: Decide function declaration syntax.

*Discussion*:
There are a wide variety of different methods for defining functions. Here are a
few.

1. C-decl: `<:return_type:> <:name:> ( <:arguments:> ) [ ] { <:statements:> }`
2. C-style: `<:return_type:> <:name:> ( <:arguments:> ) { }`
3. Java-style: `public class <:Name:> { public static <:return_type:> <:name:> ( <:arguments:> ) throws <:exception_list:> { <:statements:> } }`
4. Haskell-style: ` <:name:> :: <:type:> -> <:type:> \n <:name:> <:arguments:> = <:code:>  `
5. Lisp-style: `(<:name:> <:arguments:>)`
6. Matlab-style: `function [ <:return_names:> ] = <:function_name:> ( <:arguments:> ) <:code:> end`
7. Python-style: def <:name:> ( <:argument_list:> ) <:indent:> <:code:> <:unindent:>
8. Javascript-style: `function <:name:> ( <:arguments:> ) { <:code:> }`

*Tangent*: See `Variable/Type Declaration` for choice of type within the
function signature.

I am inclined to look at a modified Lisp-style, why?
Let's look at what appears practical

    () -> () functionName       // Quite a large signature

    () functionName             // Smaller signature, can this allow multiple
                                // returns?

    void() functionName         // Can only allow single returns, because there are
                                // no nested returns

    (32s a, char b)(64s c) functionName  // Note: '(' in the beginning is required to
    give the LL(1) parser information that it is a function.

    (32s q, 8u c):(64u e) functionName // Return args on the left.

    ((:) b):(30u a) functionName // Returning a b void function

    (ptr (:) b):(18u a) functionName // Returning a b, a pointer to a void function.

    (ptr (->) b)->(32s a) functionName // Returning a b, a pointer to a void function, using arrows

    (32u a_, 8u b_ : C c_) functionName


Why use arrows when typing is cheaper using ':'? The function declaration will
have to start with a '('. What if we want a variable of - say - a class to be
assigned functions? Use function pointers. This means declarations starting with
`(` are static functions that are nonassignable.
Having a collected signature of functions is very clean and useful. It makes the
language terser and is also easily parsable. Not to mention more easily
understandable due to the nature of nested functions:

    (ptr (:) function_match : 32u table_entry) getFunction

*Conclusion*: Function signatures are to be written in the following form.

    ( <:type:> <:result_name:>, <:arguments:> : <:type:> <:argument_name:>, <:arguments:> ) <:function_name:>
    { <:code:> }

An example of this.

    (64u factorial, 32s error_code : 32u to_calculate) computeRawFactorial
    {
      var 64u factorial(value: to_calculate);
      to_calculate -= 1;
      while (to_calculate > 1)
      {
        factorial *= to_calculate;
        to_calculate -= 1;
      }
      return factorial: factorial, error_code: 0;
    }
