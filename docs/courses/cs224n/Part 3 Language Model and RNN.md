# Part 3 Language Model and RNN

# n-gram

* assumption: $\boldsymbol{x}^{(t+1)}$ depends only on previous $n-1$ words.
* Sparsity problem, --> backoff (n-1)-gram
* practically, n has to be less than 5.

# Neural language model

* fixed-window natural language model
* Recurrent Neural Net