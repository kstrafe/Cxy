## Access Specifiers ##
*Description*: How is data in a class object hidden?
*Discussion*: Many languages use the public/private/protected idiom. This document
has been using them for a while, but I think it can now be replaced by a lexer rule.
The rule can be simple:

	any_name // is public
	_any_name // is restricted
	__any_name // is private

	anyName // is public
	_anyName // is restricted
	__anyName // is private

The same lexing rules apply for the rest of the lexeme. What is considered special
are the starting underscores. This is in line with common conventions in languages
without data hiding methods. Instead of introducting verbose keyword, we can store
the data along the name. The following was previously utilized

	public any_name
	restricted any_name
	private any_name

What's interesting is that we can question whether restricted is really needed...
Imagine the following class.

	32u __my_number;
	const ptr const 32u my_num;

	(: this ) ClassName
	{
		this.my_num = $$this.__my_number;
	}

The above forced my_num to be public, referring to __my_number by pointer. The weakness
of that is that we need to assign a pointer. Pointers aren't nice to work with here.
It's probably okay to allow restricted variables to avoid the horrible boilerplate.

*Conclusion*: Use leading underscores to declare access mode.


