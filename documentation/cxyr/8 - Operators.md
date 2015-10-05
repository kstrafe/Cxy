## Operators ##
*Description*: Should we allow operator overloading?

*Discussion*: Let's look at the three types that I can think of right now:

1. Liberal overloading
2. Conservative overloading
3. Restricted overloading

Liberal: can define any new operator. This is essentially a function declaration
with a group of symbols like `:?*=|` as a valid function name.
Conservative (C++): can overload any operator that is already well-defined on
plain-old-data types.
Restricted (Java): no operator overloading/creation at all.

So what shall we choose?
Let's look at some common operators.

    1 + 2;
    3 * 5;
    9 - 3;
    4 / 2;
    400 % 3;
    1 << 3;
    1000 >> 3;
    ==, !=, >=, <=, >, <
    ||, &&, !, ~, |, &
    , . ->

The conservative model is very useful when working with custom data types that
have well-defined semantics over addition, multiplication, etc.:

    var Vector vec1_(1, 3, -2);
    var Vector vec2_(10, -5, 4);
    var Vector vec3_ = vec1_ + vec2_;
    var Vector vec4_ = vec1_.add(vec2_);
    var Vector vec5_ = vec1_.+(vec2_);

The plus can be considered a member function of the Vector class.
Seems like a strong case for allowing at least conservative operator overloading
, right? I remember using the GLM library in C++. Its operator overloading made
code very understandable. Later I tried doing BigNum operations in Java and the
code was hard to read and quite unintelligible. I think it's a good idea to
allow at least conservative syntax overloading. A very liberal overloading style
(you can define any operator) is too much, I think.
Nobody is able to read code that says: "object_a ==¤(/#)-?+.. object_b;".
Function names should convey information about what the function does. The
common mathematical operators do that already.

How will the syntax be for overloading?

What about the different operator types and define their semantics:

    // Arithmetic operators
    +     // Add two elements together.
    -     // Subtract the left side by the right side.
    *     // Multiply two elements together.
    /     // Divide two elements.
    %     // Take the remainder of the left side with respect to the right side.

   	// Comparison operators
   	== 	  // Equal to
   	>= 	  // Greater than or equal
   	<= 	  // Less than or equal to
   	!!=   // Not equal to
   	>	    // Greater than
   	<	    // Less than

   	// Compound operators
   	+= 	  // Add to
   	-= 	  // Subtract from
   	*= 	  // Multiply bu
   	/= 	  // Divide by
   	%= 	  // Modulo by

   	&&=   // AND by
   	||=   // OR by
   	^^=   // XOR by

   	&=    // Bitwise AND by
   	|=    // Bitwise OR by
   	^=    // Bitwise XOR by

    // Boolean operators
    &&    // Logical AND
    ||    // Logical OR
    ^^    // Logical XOR
    !!    // Logical NOT

    &     // Bitwise AND
    |     // Bitwise OR
    ^     // Bitwise XOR
    !     // Bitwise NOT

    // Return argument operators
    ~     // Extracts the specified return parameter from a function call

    // Reference operators
    $      // Gets the address of a variable
    $$     // Gets the address to a const variable
    @      // Dereferences a pointer to a variable
    @@     // Dereferences a pointer to a const variable

This is interesting because this would deprecate C-like non-operations like `!!`
. In C, this operation would mean to NOT twice, which is nonsensical. I think
C might've had the idea that, since ! is a unary operator, that it would not be
useful to force groupings of symbols to mean something else. This needs to be
conveyed clearly to anyone using the language. Symbols are always grouper
together. Very important. Anyhow, I'm still thinking a bit about the reference
operators.


*Conclusion*:

    (Vec3 result_vec : cref Vec3 left_side, cref Vec3 right_side) +
    {
      var Vec3 temp_;
      temp_.set
      (
        x_coord: left_side.x_coord + right_side.x_coord,
        y_coord: left_side.y_coord + right_side.y_coord,
        z_coord: left_side.z_coord + right_side.z_coord
      );
      return result_vec: temp_;
    }

    As well as the operators
    1 + 2
    2 - 3
    3 * 7
    113 / 2
    8 % 3

    true && false
    true || false
    false ^^ true
    !! false

    0b0101101 & 0b1011
    0b0101101 | 0b1011
    0b0101101 ^ 0b1011
    ! 0b1011

    doCall()~x_val

    32u a_;
    ptr 32u a_ = $a_
    @a_ = 15;


