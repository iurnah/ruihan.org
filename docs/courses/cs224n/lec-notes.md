# CS224N: Natural Language Processing with Deep Learning

## Lecture 1 Introduction to NLP and Deep Learning

* Representations of NLP levels: Semantics
  * Traditional V.S. DL (rules v.s. sophisticated algorithm)
* Applications:
    * Sentiment Analysis
    * Question Answering system
    * Dialogue agents / response generation

## Lecture 2 Word Vector Representations: word2vec

* "one-hot" representation, localist representation
* distributional similarity based representations
    * "You shall know a word by the company it keeps” (J. R. Firth 1957:11)"
    * dense vector for each word type, chosen so that it is good at predicting other words appearing in its context (gets a bit recursive)
* Learning neural network word embeddings
    * model $p(\text{context} | w_t) = ?$
    * loss function: $J = 1 - p(w_{-t}|w_{t})$, $w_{-t}$, context words that doesn't include word $w_t$.
* word2vec
    * Skip-grams (SG) - predict context words given target center words
    * Continuous Bag of Words (CBOW) - predict target center word from bag-of-words context words
* 2 training methods
    * hierarchical softmax
    * negative sampling: tain binary logistic regression for a true pair versus a couple of noice pairs.
* Core ideas of SG prediction
    * maximize the prediction of the model $p(\text{context} | w_t) = ?$ for all context words in the form of the cost function $J(\theta)$.
    * cost function:
    $$
    J'(\theta) = \prod_{t=1}^T\prod_{\substack{-m \le j \le m\\
    j \ne 0}} p(w_{t+j}|w_t; \theta)
    $$
    * Negative log likelihood
    $$
    J(\theta) = -\frac{1}{T} \sum_{t=1}^T\sum_{\substack{-m \le j \le m\\ j \ne 0}} \log p(w_{t+j}|w_t)
    $$
    * softmax
    $$
    p(o|c) = \frac{\exp(u_o^T v_c)}{\sum_{w=1}^{v}\exp(u_w^T v_c)}
    $$
* What's really mean when you say train word2vec model
    * optimize the parameter $\theta$, which is a $R^{2\cdot d \cdot V}$, $d$ is
    the word vector dimention, $V$ is the vacabular size, each word is represented by 2 vectors!
    * Compute all vector gradients!!!
* Gradient calculation (lecture slides)

## Lecture 3 Advanced Word Vector Representations

* Compare count	based and direct prediction
* count based: LSA, HAL (Lund & Burgess), COALS (Rohde et al), Hellinger-PCA (Lebret & Collobert)
    * __Fast training__
    * __Efficient usage of statistics__
    * Primarily used to capture word similarity
    * Disproportionate importance given to large counts
* direct prediction: NNLM, HLBL, RNN, Skip-gram/CBOW, (Bengio et al; Collobert & Weston; Huang et al; Mnih & Hinton; Mikolov et al;Mnih & Kavukcuoglu)
    * Scales with corpus size
    * Inefficient usage of statistics
    * __Can capture complex patterns beyond word similarity__
    * __Generate improved performance on other tasks__
* Combining the best of both worlds: GloVe
    * Fast training
    * Scalable to huge corpora
    * Good performance even with small corpus, and small vectors
* How to evaluate word2vec?
    * Intrinsic:
        * Evaluation on a specific/intermediate subtask
        * Fast to compute
        * Helps to understand that system
        * Not clear if really helpful unless correlation to real task is established
    * Extrinsic:
        * Evaluation on a real task
        * Can take a long time to compute accuracy
        * Unclear if the subsystem is the problem or its interaction or other subsystems
        * If replacing exactly one subsystem with another improves accuracy --> Winning!

## Assignment 1 (Spring 2019)

* Singular Value Decomposition (SVD) is a kind of generalized PCA (Principal Components Analysis).

### Review materials

* Gradient Descent (SGD)
* Singular Value Decomposition (SVD)
* cross entropy loss
* max-margin loss

## Lecture 4 Word Window Classification and Neural Networks

* Window classification: Train softmax classifier by assigning a label to a
  center word and concatenating all word vectors surrounding it.
* max-margin loss; $J(\theta) = \max(0, 1 - s + s_{corrupted})$. $s$ is the good
  part, $s_{corrupted}$ is the bad part, we would like the bad part is smaller than $s - 1$.
* backpropagation:
    * insight: reuse the derivative computed previously
    * Hadamard product ($\circ, \odot, \otimes$)

## Lecture 5 Backpropagation (Feb 24, 2019)

### Details of backpropagation

The backprop algorithm is essentially compute the gradient (partial derivative)
of the cost function with respect all the parameters, $U, W, b, x$

With the following setup:

* max-margin cost function: $J = \max(0, 1 - s + s_c)$
* Scores: $s = U^T f(Wx + b), s_c = U^T f(Wx_c + b)$
* input: $z = Wx + b$, hidden: $a = f(z)$, output: $s = U^T a$
* Derivatives:
    * $\frac{\partial s}{\partial U} = \frac{\partial}{\partial U} U^T a = a$
    * wrt one weight $W_{ij}$: $\frac{\partial s}{\partial W_{ij}} = \delta_i x_j$, $\delta_i = U_i f'(z_i) x_j$, where $f'(z) = f(z)(1 - f(z))$, $f(x)$ is logistic function or sigmoid function.
    * wrt all weights $W$: $\frac{\partial s}{\partial W} = \delta x^T$
    * wrt word vectors $x$: $\frac{\partial s}{\partial x} = W^T\delta$

### Iterpretations of backpropagation using simple function.

## Lecture 6 Dependency Parsing (Feb 27, 2019)

## Lecture 8

## Lecture 9 Machine Translation and Advanced Recurrent LSTMs and GRUs

## Reference

* [Natural Language Processing with Python](http://www.nltk.org/book/)