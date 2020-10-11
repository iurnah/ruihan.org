# Convolutional Neural Networks

* no pad (valid): n, f -> n - f + 1. p is zero
* after pad (Same): n + p, f -> n + 2p - f + 1. $p = \frac{f - 1}{2}$ is the layer
  of the pad
* The value `f` is always odd.
* stride $s$. output dimention: floor of $\frac{n + 2p - f}{s} + 1$
* 2D image
    * num_channel must match


input: 1 x 60 x 60,  filter: 1 x 1 x 60,