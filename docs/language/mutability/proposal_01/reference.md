A quick overview of icarus' planned reference and assignment semantics

This is all theoretical at this point and still requires much discussion.

quick summary of rules
======================

    `=` always creates a new value and binds the variable on the left to it

Some examples:

1. `let a = 5` creates a new variable `a` and a new value `5` and binds them
2. `a = 15` creates a new value `15` and rebinds the existing `a` to this
3. `a += 3` is really `a = a + 3`, this creates a new value containing the result of `a + 3` and rebinds the existing `a` to this new value. No mutation occurs here.
4. `f.a = 4` will change the field `a` of the instance `f` to the new value `4`. This will not otherwise mutate `f`.
5. `&a = 5` is rebinding the variable referenced by `a` to the new value `5`
6. `&f.a = 14` is rebinding the field `a` within the instance references by `f` to the new value `14`. This will not otherwise mutate `f`.

local mutation
==============

Local variables are mutable and they can be reassigned new values

    let a = 5

this allocated an integer value `5` and created a mutable variable `a` referencing this

      +-var-------+
      | a         |
      | ::Signed  |
      | mutable   |
      +-----------+
           \
            \
             \
              \
               \
                v
               +-value-----+
               |  5        |
               |  ::Signed |
               +-----------+


we can rebind the variable to new values

    a = 14

      +-var-------+
      | a         |
      | ::Signed  |
      | mutable   |
      +-----------+
           \
            \
             \
              \
               \
                v
               +-value-----+
               |  14       |
               |  ::Signed |
               +-----------+



local aliasing
==============

we can then create aliases

    let a = 5
    let b = a

we now have

      +-var-------+   +-var-------+
      | a         |   | b         |
      | ::Signed  |   | ::Signed  |
      | mutable   |   | mutable   |
      +-----------+   +-----------+
           \           |
            \          |
             \         |
              \        |
               \       |
                v      v
               +-value-----+
               |  5        |
               |  ::Signed |
               +-----------+

however any use of `=` creates a new value

    b += 3

yields:

      +-var-------+   +-var-------+
      | a         |   | b         |
      | ::Signed  |   | ::Signed  |
      | mutable   |   | mutable   |
      +-----------+   +-----------+
          |               |
          |               |
          |               |
          |               |
          |               |
          v               v
     +-value-----+      +-value-----+
     |  5        |      |  8        |
     |  ::Signed |      |  ::Signed |
     +-----------+      +-----------+



function arguments
==================

Signedro
-----------

Arguments passed to function by default pass an immutable reference

    function foo(a::Signed) ...

    ....

    let x = 5
    foo(x)

is a function taking an immutable reference to an int,
any attempts to mutate a will result in a compile time error,
any attempts to upgrade a to a mutable reference will also result in a compile time error.


A function can also ask for a mutable refernece

    function bar(&a::Signed) ...

this function is now free to mutate a,
this function can also pass on this reference to another function as either a mutable or immutable reference,
that is the function can 'downgrade' to an immutable reference.

Call sites must also 'agree' to this mutable contract by also using the & operator on the variable being passed

The following is a compile time error as the caller is not agreeing to the mutable contract

    let x = 5
    bar(x) # compile time error, bar wanted a mutable reference

To correctly call this function the caller must acknowledge the mutable contract

    let x = 5
    bar(&x)

Conversely trying to call pass a mutable argument when a function is expecting an immutable reference is also an error

    let x = 5
    foo(&x) # compile time error, foo wanted an immutable reference



Simple example
--------------

Let's look at an example of how this looks:

    let a = 15
    baz(&a a)

    function baz(&m::Signed i::Signed) ...


The view of the world during the function call to baz:


      +-var-----+
      | m       |
      | ::int   |
      | mutable |
      +---------+
           |
           v
      +-var-----+          +-var-----+
      | a       |          | i       |
      | ::int   |          | ::int   |
      | mutable |          |immutable|
      +---------+          +---------+
           \                   /
            \                 /
             \               /
              \             /
               \           /
                v         v
                +-value---+
                |  15     |
                |  ::int  |
                +---------+



If inside our function baz we mutate through m

    let a = 15
    baz(&a a)

    function baz(&m::Signed i::Signed)
        &m = 123
    end

we then get this view of the world

      +-var-------+
      | m         |
      | ::Signed  |
      | mutable   |
      +-----------+
           |
           V
      +-var-------+       +-var-------+
      | a         |       | i         |
      | ::Signed  |       | ::Signed  |
      | mutable   |       |immutable  |
      +-----------+       +-----------+
           |                 |
           |                 |
           |                 |
           v                 v
      +-value-----+       +-value-----+
      |  123      |       |  15       |
      |  ::Signed |       |  ::Signed |
      +-----------+       +-----------+

More complex example
--------------------

Within a function body any assignment across `functional boundaries` has to use the `&` reference operator

Here we consider how this works when we involve instances of types containing fields:

    type Foo
        a::Signed
    end

    fn bar(&x::Foo y::Foo) ...

    let f = Foo(1)
    bar(&f f)

we now have

      +-var-----+
      | x       |
      | ::Foo   |
      | mutable |
      +---------+
           |
           v
      +-var-----+          +-var-----+
      | f       |          | y       |
      | ::Foo   |          | ::Foo   |
      | mutable |          |immutable|
      +---------+          +---------+
           \                   /
            \                 /
             \               /
              \             /
               \           /
                v         v
                +-value---+      +-value-----+
                |  a ----------->|  1        |
                |  ::Foo  |      |  ::Signed |
                +---------+      +-----------+

we can then assign across functional boundary using the `&` operator

    &x.a = 5

we now have

      +-var-----+
      | x       |
      | ::Foo   |
      | mutable |
      +---------+
           |
           v
      +-var-----+          +-var-----+
      | f       |          | y       |
      | ::Foo   |          | ::Foo   |
      | mutable |          |immutable|
      +---------+          +---------+
           \                   /
            \                 /
             \               /
              \             /
               \           /
                v         v
                +-value---+      +-value-----+
                |  a ----------->|  5        |
                |  ::Foo  |      |  ::Signed |
                +---------+      +-----------+

if we create a new Foo

    &x = Foo(14)

we would now have

      +-var-----+         +-var-----+
      | x       |         | y       |
      | ::Foo   |         | ::Foo   |
      | mutable |         |immutable|
      +---------+         +---------+
           |                  |
           v                  v
      +-var-----+         +-value---+      +-value-----+
      | f       |         |  a ----------->|  5        |
      | ::Foo   |         |  ::Foo  |      |  ::Signed |
      | mutable |         +---------+      +-----------+
      +---------+
          |
          v
      +-value---+      +-value-----+
      |  a ----------->|  14       |
      |  ::Foo  |      |  ::Signed |
      +---------+      +-----------+


Further considerations
======================

Some food for further thought

* I quite like the idea of of the mutability contract where both caller and function have to explicitly agree on mutability of arguments


One possible consideration is to make it illegal to pass a mutable and immutable reference
of the same object to the same function

    # potential: ILLEGAL cannot pass both mutable nad immutable reference of same value
    bar(&a a)


