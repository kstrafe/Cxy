## Variable and Type Declaration ##
*Tangent*: See `Plain Old Data (POD) Types` for declaring PODs.
*Tangent*: See `Naming Scheme`.

*Description*: The style that is desired must be easy to parse. For consistency,
types must avoid `cdecl`. Type declarations declare an object of a type.

*Discussions*: Type reads from left-to-right.

1. `const int *const variable;`
2. `variable is const ptr const int`
3. `const ptr const int variable`
4. `variable: const ptr const int`
5. `variable: int = 100`

Number four and five are a little more tricky to parse. Note that `variable_` will
be an identifier. The problem with this statement is that we can allow arbitrary expressions
as statements (function calls mostly). We can not know until we parse the ":" token
that this is a declaration. This makes the grammar more error-prone. The same goes
for a human understanding. The human needs to read until he gets to the ":". Only then
will he understand that it is a declaration.

Number one and three are also interesting. I'd like to avoid the star in one because
it can be ambiguous with the multiplication operator. The last option is number three
(note that option two is removed along four and five).  This is LL(1) parsable (a predictive
parser). The FIRST(statement) is different for each production. Reads easily from left
-to-right.

=== Kevin R. Stravers -- 15/07/2015

Due to the the parser being LL(1), and expressions accepting types as their first token
, this causes a conflict. An LR(1) parser easily fixes this, but it is decided that
an LL(1) parser is to be used. An example of the ambiguity.

	a.Ab a;
	a.Ab.a();

The former is a declaration, the latter is a function call (an expression) on a function
pointer `a`. To avoid the ambiguity, we can use a tie-breaker in front
of this.

	var a.Ab a;
	expr a.Ab.a();

Either one of them, or both. Either way we want expressions to be executable in
within the function scope. It is thus rather inconvenient to have to resort to
writing `expr` all the time. In addition, the `var` syntax allows us to add the
following to our basic types.

	[3, 32u].element_count;
	(const 32u).max_value;
	ptr.max_value;
	ptr.bit_count; // Platform-specific.

This allows using these expressions as statements. Our declarations will look
like.

    var 32s x = 100, y = -400;
    var alpha.AlphaManager manager;

The nice thing about this is that we (humans) can quickly scout the code for
declarations. Especially when var is highlighted with some semi-obnoxious color.

===

=== Kevin R. Stravers -- 02/08/2015

The newly added concepts conflict somewhat with field declarations in that the first
token is the ClassName. To rememdy this, var will also be used for fields.

A simplifcation can be made. Instead of redundantly specifying var, we can allow

	var {
		MyType my_type(arg_: 10);
		32u a_number;
		sml.String uri_string;
	}

===

=== Kevin R. Stravers -- Mon 05 Oct 2015 09:19:14 AM CEST

A second simplification seems feasible:

	var 32u {
		a = 345;
		b = a * 2;
		c = b * a;
	};

The form of this statement is `var Type { assignments };`

	var sml.String {
		a = "a";
		b = a + "b";
		c = a + b * 30 + "cccc";
	};

This also works for arrays. There are no clashes with other datatypes as none contain
curly braces.

===

*Conclusion*:
	var Type Identifier OptionalInitialization;

examples:

	var ptr 32u value = new 32u(100);
	var const ptr 32u value(new 32u(100));
	var 82s value(123456789123456789123456789);
