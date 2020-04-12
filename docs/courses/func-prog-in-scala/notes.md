# Functional Programming Principles in Scala

## Week 1

* Theory wise, pure imperative programming languages is limited by "Von Neumann" bottleneck

* Theory:
    1. one or more data types
    2. operations on these types
    3. laws that describe the relationship between values and operations
    4. Normally, a theory does not describe mutations (change something but keep the identity)

*  Theories without Mutation
    1. Pylonomial 
    2. String

* consequence in programming
    * avoid mutations
    * have a power way to abstract operations and functions.

* The substitution model
    * lambda calculus
    * only to expression don't have a side effect
    * termination
    ```scala
    def loop: Int = loop
    ```
    * unreduced arguments

* Call-by-name and call-by-value
```scala
def test(x: Int, y: Int) = x * x
test(2, 3)
test(3 + 4, 8)
test(7, 2 * 4)
test(3 + 4, 2 * 4)
```
* CBV, CBN, and termination
    * CBV termination --> CBN termination
    * Not vice versa
    * example
    ```scala
    def first(x: Int, y: Int) = x
    first(7, loop)
    ```
    * enforce call by name `=>`

* Conditional expressions
    * if-else not a statement, just a expression.
* Value and definitions
```scala
scala> def loop: Boolean = loop && loop
loop: Boolean
scala> def x = loop
x: Boolean
scala> val x = loop
java.lang.StackOverflowError
at .loop(<console>:12)
```
* Square root with Newton's Method
```scala
def abs(x: Double) = if (x < 0) -x else x

def sqrtIter(guess: Double, x: Double): Double =
if (isGoodEnough(guess, x)) guess
else sqrtIter(improve(guess,x), x)

def isGoodEnough(guess: Double, x: Double) =
abs(guess * guess - x) < 0.001

def improve(guess: Double, x: Double) =
(guess + x / guess) / 2

def sqrt(x: Double) = sqrtIter(1.0, x)
```

* Tail recursion: Implementation consideration. If a function calls itself as
  its last action, the function's stack frame can be reused. This is called tail
  recursion. Tail recursive function are iterative processes. (anotation: @tailrec)
* `gcd` is tail recursion
```scala
def gcd(a: Int, b: Int): Int = {
  if (b == 0) a else gcd(b, a % b)
}
```

* `factorial` is not
```scala
def factorial(n: Int) =
  if (n == 0) 1 else n * factorial(n - 1)
```

* but we can rewrite `factorial` in tail recursion form.
```scala
def factorial(n: Int): Int = {
  def loop(acc: Int, n: Int): Int =
    if (n == 0) acc
    else loop(acc * n, n - 1)
  loop(1, n)
}
```
