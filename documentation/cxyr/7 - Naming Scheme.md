## Naming Scheme ##
*Description*: How are names consistently given to different entities?

*Discussion*: Many languages use a very lax and liberal naming scheme. `abc` can
refer to a function, variable, package, namespace, class, etc. I thin this can
be improved by restricting the name (a subset of all possible strings) to
specific distinct entities.

*Conclusion*:
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



