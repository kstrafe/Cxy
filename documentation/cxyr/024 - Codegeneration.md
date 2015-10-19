## Codegeneration ##
*Description*: What level of code generation is useful?
*Discussion*: I've recently started to grok different macro and text-editing
languages. Examples of these are awk and m4. These languages are perfect for
generating code or text. They are both very simple and easy to understand. Other
languages such as PHP also exist, even though PHP is more aimed at the net.

The thing about these languages is that they exist, they are specifically for
generating code, and they are really good at it. I think it may be better to
avoid implementing the code generator in Cxy itself. Instead, think about allowing
compiler hooks of some sort. It calls the hook on the file upon seeing diffs.

The biggest problem is that the language (Cxy) is itself not very practical for
editing text. There is operator overloading for strings, but it is still cumbersome.

	var sml.String my_string = "Hi!";

versus awk's

	my_string = "Hi!";

Code generators should not have terribly complex logic anyways. Well... maybe.
You know. I think it's best to postpone code generators. Instead, I'll make sure
that the code that is generated can easily be checked for correctness. An important
factor in this is going to be "concepts".

*Conclusion*: Code generators will be implemented later.


