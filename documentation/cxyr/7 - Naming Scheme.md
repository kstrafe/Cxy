## Naming Scheme ##
*Description*: How are names consistently given to different entities?

*Discussion*: Many languages use a very lax and liberal naming scheme. `abc` can
refer to a function, variable, package, namespace, class, etc. I think this can
be improved by restricting the name (a subset of all possible strings) to
specific distinct entities.

Here is an initial proposal:
package: `abc1`. Only alpha and numbers allowed, lowercase, no underscore.
variable: `abc1_`. Only alphanumerics allowed, lower/upper -case, underscore
mandatory.
function: `doSomething`. Only alphanumerics allowed, lower/upper -case.
Mandatory beginning and ending lowercase and at least one uppercase character.
class: `ClassName`. Only alphanumerics allowed, lower/upper -case. Mandatory
first upper case character. Mandatory ending lower case.
constant: `END_OF_LINE`. Only uppercase alphanumerics. Optional underscores.
keywords: same as package names. These are reserved. No packages can have these
names.

The issue with this is that it just looks ugly. Trailing underscores are not nice to
have peppered all over.

package: [a-z]+                                 // sml, math, gl,..
variable: [a-z][0-9a-z]+                        // name, numb01, x1y1, x2y1,..
function: [a-z][0-9a-zA-Z]+[0-9a-z]             // doSomething, sin, enter, createNewUnit, kill5
class: [A-Z][0-9a-zA-Z]+[a-z]                   // Name, MyClass09, Queue, Vector2mapNice
enum: `[A-Z]+_[A-Z_]+`                          // `ENUM_NUM, IS_DAMAGED, CONTROL_THE_LEAK`
constant: [A-Z]+                                // THIS, IS, A, CONSTANT

The nice thing is that the above still allows slick variable names, but separates
classes, constants, and enums from them.

*Conclusion*: Enforce names as given by the last table.
