# String Types #
*Description*: Look at how the strings are escaped.
*Discussion*: Many languages have a method of escaping inside strings.
This is useful if you want to write non-verbatim elements such as newlines,
tabs, or similar characters. Unicode characters could also be escaped. This can be
seen in C++:

	"\u001F"

Is a potential unicode escaped character.

	'\''

Is an escaped single-quote, it should be similar to

	"'"

In C++, both types would be different, but I think that's unnecessary in Cxy. When
a constexpr string is received, it can automatically cast it, by checking if it has
size 1:

	var 8u a = 'a';
	var 8u b = "b";
	var sml.String c = "c";

Another kind of string is the verbatim string. It takes no escapes and can not escape
itself. This is useful, especially for regular expressions. Some languages like C++
use the R"()" syntax for verbatim strings. Others use a @"" syntax. Both work well,
but are still rather bulbous and ugly.

Instead, I'd like to use a different syntax:

	``

Simple, elegant, and minimalistic.

	`"'\ just works!~`

Note that `` does not allow \ to operate, it just takes it as a literal. To escape
a verbatim string we'd write a double ``. This is the only escape sequence that is
valid for verbatim strings.

## Under the Hood #

The Mealy machine of the lexer takes only " as input, ` and ' are considered "unknown symbols".
What happens right before the mealy machine fires is that each input character gets
to go through a 'string filter'. This filter turns ` into " and ' into " if they start
a string. Otherwise, it leaves them be. If the state of thes stringfilter is inside
a string, the escapes will be used (except for verbatim strings).

##

The escape sequences are \cXX \n, \t, \r, \v, \f.
X denote any hexadecimal digit.

*Conclusion*: Strings are given by '' (multichar), "" (multichar), and `` (verbatim)
