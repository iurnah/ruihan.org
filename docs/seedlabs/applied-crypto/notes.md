# Applied Cryptograph Notes

## GCD

### Definition

GCD of two integer is the largest integer that divide both given integers.

### Calculation

#### Factorization method

Find the integer factorization of the two integer first, find the common factors
and multiply together.

#### Euclidean Algorithm

The key idea of Euclidean algorithm is to use the smaller integer to "measure"
the larger integer, then use the reminder to "measure" the smaller integer, and
so on, untill the reminder is 0. Generally, gcd(a, b) can be obtained by apply
the following sequence of equations until $r_k = 0$.

$$a = q_0 b + r_0$$

$$b = q_1 r_0 + r_1$$

$$r_0 = q_2 r_1 + r_2$$

$$\vdots$$

$$r_{k - 3} = q_{k-1} r_{k - 2} + r_{k - 1}$$

$$r_{k - 2} = q_{k-1} r_{k - 1} + r_{k}$$

Because the qotient isn't needed in find the GCD. We can replace the iteration
$r_{k-2} = q_k r_{k-1} + r_k$ with modulo operations $r_k = r_{k-2} \mod r_{k-1}$.

## Modular multiplicative inverse (modular division)

### Definition

A modular multiplicative inverse of an integer $a$ is an integer $x$ such that
the product $ax$ is congruent to 1 with respect to the modulus $m$. The congruence
is written as $ax \equiv 1 \pmod m$. Two integers $a$ and $b$ are said to be
congruent modulo $m$ if $m$ divides their differences, denoted by  $a \equiv b \pmod m$.

### Solution

The modular multiplicative inverse $x$ of an integer $a$ may have zero, one or
several solutions. But with the condition $\gcd(a, m) = 1$ hold, there exists
exactly one solution. This is the basis of the theory used to calculate the
private key $d$ in RSA. Now we could use Extended Euclidean Algorithm to calculate $d$.

#### Extended Euclidean Algorithm

```python
def EEA(r0, r1):
    ''' Extended Euclidean algorithm'''
    # ensure r0 > r1
    if r0 < r1:
        r0 ^= r1
        r1 ^= r0
        r0 ^= r1

    # init value
    s0, s1, t0, t1 = 1, 0, 0, 1

    while True:
        ri = r0 % r1
        q = (r0 - ri) / r1
        si = s0 - q * s1
        ti = t0 - q * t1
        if ri == 0:
            break;

        r0, r1 = r1, ri
        s0, s1 = s1, si
        t0, t1 = t1, ti

    return r1, s1, t1
```

## Euler's $\Phi$ function

> Definition: The number of integers in $\mathbb{Z}_m$ relatively prime to m is denoted by $\Phi(m)$.
 $\mathbb{Z}_m$ is interger set. $\mathbb{Z}_6 = \{0,1,2,3,4,5\}$.

### How to calculate $\Phi(m)$

Let $m$ have the folllowing canonical factorization

$$
m = p^{e_1}_{1} \cdot p^{e_2}_{2} \cdot, \dots, \cdot p^{e_n}_{n}
$$

Where the $p_i$ are distinct prime numbers and $e_i$ are positive integers, then

$$
\Phi(m) = \prod_{i = 1}^{n}{(p^{e_i}_{i} - p^{e_i - 1}_{i})}.
$$

For example:

1. $m = 240 =  2^4 \cdot 3 \cdot 5$, $\Phi(240) = (2^4 - 2^3)\cdot (3^1 - 3^0) \cdot (5^1 - 5^0) = 8 \cdot 2 \cdot 4 = 64$
2. $m = 21 = 3 \cdot 7$, $\Phi(21) = (3^1 - 3^0)\cdot (7^1 - 7^0) = 2\cdot6 = 12$

> Remark 1: The security of RSA is based on the fact that Euler's function $\Phi(m)$ is hard to be calculated for large integers, as we know it is compuationally hard to do prime factorization for a large integer.

## Euler's Theorem

> Definition: Let $a$ and $m$ be integers with gcd(a, m) = 1, then: $a^{\Phi(m)} \equiv1\pmod m$.

## Fermat's Little Theorem

> Definition: Let $a$ be an integer and $p$ be a prime, then: $a^p \equiv a \pmod p$. It can also be stated in the form: $a^{p-1} \equiv 1 \pmod p$.

## RSA Algorithm

### Key generation

1. Choose large primes $p$ and $q$.
2. Calculate $n = p\cdot q$. Calculate $\Phi(n) = (p-1)(q-1)$.
3. Choose public key $K_{pub}=e \in \{1,2,\cdots,\Phi(n)-1\}$ such that $\gcd(e, \Phi(n)) = 1$ ($e$ and $\Phi(n)$ are co-prime).
4. Compute the private key $K_{priv}=d$ such that $d\cdot e \equiv 1\pmod {\Phi(n)}$ using the Extended Euclidean Algorithm.
EEA: $gcd(e, \Phi(n)) = 1 \Rightarrow gcd(e, \Phi(n)) = d\cdot e + t\cdot \Phi(n) \Rightarrow d\cdot e \equiv 1\pmod {\Phi(n)}$.
5. Output of the routine: 1. $K_{pub}=(n,e)$, 2. $K_{priv}=d$

!!! Note "Remark 1"
    The gcd condition is necessary to ensure the modular inverse of $e$, which is $d$, is unique.

!!! Note "Remark 2"
    Eve have $K_{pub} = (n, e)$, but not $\Phi(n)$. In order to compute the private key $d$.
    He have to first compute $\Phi(n)$. To compute $\Phi(n)$, he need to compute the primie
    factorization of $n$, which is very hard to compute when n is very large, such as a
    2048-bit integer. This ensure the security of RSA.

### Encryption

Given public key $K_{pub} = (n, e)$ and the plain text message $x \in Z_n = \{0, 1, \cdots, n-1\}$,
the encrypted $y$ is computed by

$$y = ENC_{k_{pub}}(x) \equiv x^e\bmod n$$

### Decryption

Given private key $K_{priv}=d$ and the cipher text $y \in Z_n = \{0, 1, \cdots, n-1\}$, the message can be decrypted as

$$x = DEC_{k_{priv}}(y) \equiv y^d\bmod n$$

### Fast exponentiation

!!! Note
RSA one-way function, 1. encryption and decryption, 2. use $n$ and $e$ to compute $d$.