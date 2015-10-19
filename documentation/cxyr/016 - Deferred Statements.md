## Deferred Statements ##
*Description*: Allowing any collection of statements to be executed at all exit
points.
*Discussion*: It is rather useful when objects have destructors. Whenever a
scope is exited, the destructors from that object in that scope need to run.
These deallocate data and ensure that all field destructors are all destructed.

This is incredibly useful, however, sometimes, we need some custom code to run
at the end of all possible exit points. Meet the defer statement.

    {
      mutex_.lock();
      defer mutex_.unlock();
      // Do something, may throw an error, may prematurely return
    } // Always run the unlock().

Some languages use a sentry object that has a destructor that performs the
unlock as shown above. There are some problems with that. First of all, it's
not intuitive and not made to do that. Secondly, it requires an actual named
object to be instantiated. It's also not practical to have to create a new
class for your special action. The defer statement works better.

*Conclusion*: The defer statement guarantees to run its contents at the scope's
exit.


