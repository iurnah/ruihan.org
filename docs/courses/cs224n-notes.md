# CS224N NLP with Deep Learning

## Part 1 Word Embeddings Based on Distributional Semantic

Date: April 20th 2019
If you have no experience in the Natrual Language Processing (NLP) field and asked
to name a few practical NLP applications, what would you list? Google Translation
or Amazon Echo voice assistant may first come into your mind. How did those products
understand a sentence or human conversation? Engineers built computational models
that understand our language as our brain does. To build such a computational model,
the following components are necessary.

1. a large corpus of text (input training data)
2. a method to represent each word from the corpus (feature representation)
3. a starting model that barely understands English at the beginning but can be improved by "reading" more words from the corpus (parametric function).
4. an algorithm for the model to correct itself if it makes a mistake in understanding (learning algorithm/optimization method)
5. a measurement that can qualify the mistake the model made (loss function)

### Introduction

Starting with this post, I will write materials from my understanding of the Stanford CS224N. The plan is to anchor all the applications I learned in the above 5 components so as to systematically understand the gist of each of those applications and corresponding solutions. Part 1 is about language models and word embeddings. Part 2 will discuss neural networks and backprop. Part 3 revisits the language model and introduces recurrent neural networks. Part 4 study advanced RNN, Long short-term memory (LSTM), and gated recurrent networks (GRN). Future parts will depend on my availability and the schedule of CS224N. Let's get started!

### Word representations

How do we represent word with meaning on a computer? Before 2013, wordNet and one-hot vector are most popular in word meaning representations. WordNet is a manually compiled thesaurus containing lists of synonym sets and hypernyms. Like most of the manual stuff, it is subjective, unscalable, inaccurate in computing word similarity, and impossible to maintain and keep up-to-data. One-hot vectors represent word meaning using discrete symbolic $1$s in a long stream of $0$ of vector elements. It suffers from sparcity issues and many other drawbacks. We will not spend time on that outdated method Instead, We will focus on the embedding method using the idea of word vector.

The core idea of this embedding method is based on the remarkable insight on word meaning called __distributional semantics__. It conjectures that a word’s meaning is given
by the words that frequently appear close-by. It is proposed by J. R. Firth. Here is the famous quote:

!!! quote
    "You shall know a word by the company it keeps" -- J. R. Firth 1957: 11

In this method, we will build a dense vector for each word, chosen so that it is similar to vectors of words that appear in similar contexts. word vector representation also called distributed representation, or word embeddings.

$$
\mathrm{banking} =
\begin{pmatrix}
0.286\\
0.792\\
−0.177\\
−0.107\\
0.109\\
−0.542\\
0.349\\
0.271
\end{pmatrix}
$$

### word2vec

Word2vec (Mikolov et al. 2013) is a framework for learning word vectors. Word2vec go through windows of words and calculate the probability of the context word given the center word (or vice versa) using the similarity of the word vectors. It keeps adjusting the word vectors to maximize this probability. Vividly, these two pictures show the general idea of how word2vec works.

![word2vec1.png-48.3kB][1]

![word2vec2.png-49.1kB][2]

Formally, for a single prediction, the probability is $P(o|c)$, interpreted as the probability of the outer word $o$ given the center word $c$. For the large corpus including $T$ words and each position $t = 1, 2, \cdots, T$, we predict context words within a window of fixed size $m$, given center word $w_t$. The model likelihood can be written as the following

\begin{equation*}
\mathrm{likelihood} = L(\theta) = \prod_{t=1}^T\prod_{\substack{-m \le j \le m\\
j \ne 0}} p(w_{t+j}|w_t; \theta)
\end{equation*}

The objective function $J(\theta)$ is the average negative log likelihood:

\begin{equation*}
J(\theta) = -\frac{1}{T}\log{L(\theta)} = -\frac{1}{T} \sum_{t=1}^T\sum_{\substack{-m \le j \le m\\
j \ne 0}} \log p(w_{t+j}|w_t; \theta)
\end{equation*}

We've defined the cost function by now. In order to minimize the loss, we need to know how $p(w_{t+j}|w_t; \theta)$ can be calculated. One function we can use to calculate the probability value is the $\mathrm{softmax}$ function.

\begin{equation*}
\mathrm{softmax}(x)_i = \frac{\exp(x_i)}{\sum_{j=1}^{n}\exp(x_j)}
\end{equation*}

Particularly we will write the probability as the following format.

\begin{equation*}
p(o|c) = \frac{\exp(u_o^T v_c)}{\sum_{w=1}^{V}\exp(u_w^T v_c)}
\end{equation*}

There are several points need to be emphasized.

1. Two vectors will be obtained for each individual word, one as center word $v_w$, and the other context word $u_w$. $d$ is the dimension of the word vector. $V$ is the vocabulary size.

\begin{equation*}
\theta =
\begin{bmatrix}
v_{\mathrm{aardvark}}\\
v_{\mathrm{a}}\\
\cdots \\
v_{\mathrm{zebra}}\\
u_{\mathrm{aardvark}}\\
u_{\mathrm{a}}\\
\cdots \\
u_{\mathrm{zebra}}\\
\end{bmatrix} \in \mathbb{R}^{2dV}
\end{equation*}

2. The dot product in the exponet compares similarity of $o$ and $c$. Larger dot product indicates larger probability.
3. The denorminator sum over entire vocabulary to give normalized probability distribution.
4. The $\mathrm{softmax}$ function maps aribitrary values $x_i$ to a probability distribution $p_i$. “$\mathrm{max}$” because it amplifies the probability of the largest $x_i$, “$\mathrm{soft}$” because it still assigns some probabilities to smaller $x_i$. It is very commonly used in deep learning.

### Gradient descent to optimize log likelihood loss function

This section I will purely focus on how to derive the gradient of the log likelihood loss function with respect to center word using the chain rule. Once we have the computed gradients, we are ready to implement it in matrix form and train the word vectors. This model is called skip-gram.


| loss function in $p(w_{t+j}|w_t)$ | $p(w_{t+j}|w_t)$ in softmax form |
|:---:|:---:|
| \begin{equation*}J(\theta) = -\frac{1}{T} \sum_{t=1}^T\sum_{\substack{-m \le j \le m\\ 
j \ne 0}} \log p(w_{t+j}|w_t; \theta)\end{equation*} | \begin{equation*}p(o|c) = \frac{\exp(u_o^T v_c)}{\sum_{w=1}^{V}\exp(u_w^T v_c)}\end{equation*} |


We would like to find the following derivatives:

\begin{equation*}
1. \frac{\partial}{\partial v_c} \log p(o|c) \\
2. \frac{\partial}{\partial u_o} \log p(o|c)
\end{equation*}

Let's now start working with the first one, the derivative wrt. $v_c$.

\begin{align*}
\frac{\partial}{\partial v_c} \log p(o|c)
& = \frac{\partial}{\partial v_c} \log{\frac{\exp(u_o^T v_c)}{\sum_{w=1}^{V}\exp(u_w^T v_c)}} \\
&= \frac{\partial}{\partial v_c} \log{\exp(u_o^T v_c)} - \frac{\partial}{\partial v_c} \log{\sum_{w=1}^{V}\exp(u_w^T v_c)} \;\;\;\cdots (\log\frac{a}{b} = \log a - \log b) \\
& = \frac{\partial}{\partial v_c} (u_o^T v_c) - \frac{1}{\sum_{w=1}^{V}\exp(u_w^T v_c)} \sum_{x=1}^{V} \frac{\partial}{\partial v_c} \exp(u_x^T v_c) \;\;\;\cdots \mathrm{chain\ rule}\\
& = u_o - \frac{1}{\sum_{w=1}^{V}\exp(u_w^T v_c)} \sum_{x=1}^{V} \exp(u_x^{T} v_c) \frac{\partial}{\partial v_c} (u_x^T v_c) \;\;\;\cdots \mathrm{chain\ rule}\\
& = u_o - \frac{1}{\sum_{w=1}^{V}\exp(u_w^T v_c)} \sum_{x=1}^{V} \exp(u_x^{T} v_c) \cdot u_x \\
& = u_o - \sum_{x=1}^{V} \frac{\exp(u_x^{T} v_c)}{\sum_{w=1}^{V}\exp(u_w^T v_c)} \cdot u_x \\
& = \underbrace{u_o}_{\mathrm{current}} - \underbrace{\sum_{x=1}^{V} p(x|c) \cdot u_x}_{\mathrm{expectation}}
\end{align*}

The result is remarkable. It have a great intuition in it. The gradient represent the slop in the multidimentional space that we should walk along to reach the optima. The result gradient we got tell us that the slop actully equals to the difference of current context vector $\boldsymbol{u}_o$ and the expected context vector (the weighted average over all context vectors). It has nothing todo with the center word $c$.

To compute the gradient of $J(\theta)$ with respect to the center word $c$, you have to sum up all the gradients obtained from word windows when $c$ is the center word. The gradient with respect to the context word will be very similar, chain rule is also handy in that case.

Once all the gradient with resepct to center words and context words are calculated. We can use gradient descent to update the model parameters, which in this case is all the word vectors. Because we have two vectors for each word, when update the parameters we will use the average of the two vectors to update.

### Gradient descent to optimize cross entropy loss function

Alternatively, we could also use cross entrypy loss function. CS224N 2017 assignment 1 requires to derive the gradient of cross entropy loss function. This section, we will go step by step to derive the gradient when using cross entropy loss function and $\mathrm{softmax}$ activation function in the out put layer. the cross entropy function is defined as follows

\begin{equation*}
\mathrm{CE}(\boldsymbol{y}, \boldsymbol{\hat y}) = - \sum_i y_i \log (\hat y_i)
\end{equation*}

Notice the $\boldsymbol{y}$ is the one-hot label vector, and $\boldsymbol{\hat y}$ is the predicted probability for all classes. The index $i$ is the index of the one-hot element in individual label vectors. Therefore the definition of cross entropy loss function is defined as followinng if we use $\mathrm{softmax}$ to predict $\hat y_i$,

\begin{equation*}
J_{\mathrm{CE}}(\theta) = - \sum_i^N y_i \log (\hat y_i) = -\sum_i^N y_i \log (\mathrm{softmax}(\theta)_i)
\end{equation*}

The gradient of $\frac{\partial}{\partial \theta}J_{\mathrm{CE}}(\theta)$ can be derived using chain rule. Because we will use the gradient of the $\mathrm{softmax}$ for the derivation, Let's derive $\mathrm{softmax}$ gradient first.

\begin{equation*}
\frac{\partial \hat y_i}{\theta_j} =
\frac{\partial }{\theta_j}\mathrm{softmax}(\theta)_i = \frac{\partial }{\theta_j} \frac{e^{\theta_i}}{\sum_{k=1}^{n}e^{\theta_k}}
\end{equation*}

Here we should use two tricks to derive this gradient.

1. Quotient rule
2. separate into 2 cases: when $i = j$ and $i \ne j$.

If $i = j$, we have 

\begin{align*}
\frac{\partial }{\theta_j} \frac{e^{\theta_i}}{\sum_{k=1}^{n}e^{\theta_k}} & = \frac{e^{\theta_i}\sum_{k=1}^{n}e^{\theta_k} - e^{\theta_i}e^{\theta_j} }{\Big(\sum_{k=1}^{n}e^{\theta_k}\Big)^2} \\
& = \frac{e^{\theta_i}\Big(\sum_{k=1}^{n}e^{\theta_k} - e^{\theta_j}\Big)}{\Big(\sum_{k=1}^{n}e^{\theta_k}\Big)^2} \\
& = \frac{e^{\theta_i}}{\sum_{k=1}^{n}e^{\theta_k}} \frac{\sum_{k=1}^{n}e^{\theta_k} - e^{\theta_j}}{\sum_{k=1}^{n}e^{\theta_k}} \\
& = \hat y_i(1 - \hat y_j)
\end{align*}

if $i \ne j$, we have

\begin{align*}
\frac{\partial }{\theta_j} \frac{e^{\theta_i}}{\sum_{k=1}^{n}e^{\theta_k}} & = \frac{0 - e^{\theta_i}e^{\theta_j} }{\Big(\sum_{k=1}^{n}e^{\theta_k}\Big)^2} \\
& = \frac{e^{\theta_i}}{\sum_{k=1}^{n}e^{\theta_k}} \frac{ - e^{\theta_j}}{\sum_{k=1}^{n}e^{\theta_k}} \\
& = -\hat y_i  \hat y_j
\end{align*}

Now let calculate the gradient of the cross-entropy loss function. Notice the gradient is concerning the $i$th parameter. This is because we can use the gradient of the $\mathrm{softmax}$ (the $\frac{\partial  \hat y_k}{\partial \theta_i}$ term) conveniently. 

\begin{align*}
\frac{\partial J_{\mathrm{CE}}(\theta)}{\partial \theta_i} & = - \sum_k^N y_k \frac{\partial \log (\hat y_k)}{\partial \theta_i} \\
& = - \sum_k^N y_k \frac{\partial \log (\hat y_k)}{\partial \hat y_k} \frac{\partial  \hat y_k}{\partial \theta_i} \\
& = -  \sum_k^N y_k \frac{1}{\hat y_k} \frac{\partial \hat y_k}{\partial \theta_i} \\
& = - \Big(\underbrace{y_k \frac{1}{\hat y_k} \hat y_k(1 - \hat y_i)}_{k = i} + \underbrace{\sum_{k\ne i}^N y_k \frac{1}{\hat y_k} (-\hat y_k  \hat y_i) \Big)}_{k \ne i} \\
& = - \Big(y_i (1 - \hat y_i) - \sum_{k\ne i}^N y_k \hat y_i \Big) \\
& = - y_i + y_i \hat y_i + \sum_{k\ne i}^N y_k \hat y_i \\
& = \hat y_i\Big(y_i + \sum_{k\ne i}^N y_k\Big) - \hat y_i \\
& = \hat y_i \cdot 1 - y_i = \hat y_i - y_i
\end{align*}

Write in vector form, we will have $\frac{\partial J_{\mathrm{CE}}(\boldsymbol{y}, \boldsymbol{\hat y})}{\partial \boldsymbol{\theta}} = \boldsymbol{\hat y} - \boldsymbol{y}$. With this gradient, we can update the model parameters, namely the word vectors.  In the next post, I will discuss neural networks, which have more layers than the word2vec. In neural networks, there are more parameters need to be trained and the gradients with respect all the parameters need to be derived.

### Skip-gram models

Skip-gram model uses the center word to predict the surrounding.

We have derived the gradient for the skip-gram model using log-likelihood loss function and $\mathrm{softmax}$. This section will derive the gradient for cross-entropy loss function. The probability output function will keep using the $\mathrm{softmax}$.

#### Cross entropy loss function

Since we have derived above that $\frac{\partial J_{\mathrm{CE}}(\boldsymbol{y}, \boldsymbol{\hat y})}{\partial \boldsymbol{\theta}} = \boldsymbol{\hat y} - \boldsymbol{y}$. In this case, the vector $\boldsymbol{\theta} = [\boldsymbol{u_1^{\mathsf{T}}}\boldsymbol{v_c}, \boldsymbol{u_2^{\mathsf{T}}}\boldsymbol{v_c}, \cdots, \boldsymbol{u_W^{\mathsf{T}}}\boldsymbol{v_c}]$.

#### Gradient for center word

Borrow the above steps to derive the gradient with respect to $\boldsymbol{\theta}$, we have

\begin{align*}
\frac{\partial }{\partial \boldsymbol{v_c}}J_{\mathrm{softmax-CE}}(\boldsymbol{o}, \boldsymbol{v_c}, \boldsymbol{U}) & = \frac{\partial J_{\mathrm{CE}}(\theta)}{\partial \theta_i} \frac{\partial \theta_i}{\boldsymbol{v_c}} \\ & = (\hat y_i - y_i) \boldsymbol{u_i^{\mathsf{T}}}
\end{align*}

Notice here the $\boldsymbol{u_i}$ is a column vector. And the derivative of $\boldsymbol{y} = \boldsymbol{a}^{\mathsf{T}}\boldsymbol{x}$ with respect to $\boldsymbol{x}$ is $\boldsymbol{a}^{\mathsf{T}}$. Written in vector form, we will get

\begin{equation*}
\frac{\partial J}{\partial \boldsymbol{v_c}} = \boldsymbol{U}(\boldsymbol{\hat y} - \boldsymbol{y})
\end{equation*}

In the above gradient, $U = [\boldsymbol{u_1}, \boldsymbol{u_1}, \cdots, \boldsymbol{u_W}]$ is the matrix of all the output vectors. $\boldsymbol{u_1}$ is a column word vector. The component $\boldsymbol{\hat y} - \boldsymbol{y}$ is a also a column vector with length of $W$. The above gradient can be viewed as scaling each output vector $\boldsymbol{u_i}$ by the scaler $\hat y_i - y.$ Alternatively, the gradient can also be wrote as distributive form

\begin{equation*}
\frac{\partial J}{\partial \boldsymbol{v_c}} = \boldsymbol{U}\boldsymbol{\hat y} - \boldsymbol{U}\boldsymbol{y} = -\boldsymbol{u_i} + \sum_{w=1}^{W}\hat y_w \boldsymbol{u_w}
\end{equation*}

The index $i$ in the above equation is corresponding to the index of the none zero element in the one-hot vector $\boldsymbol{y}$. Here we can see $\boldsymbol{y}$ as a the true label of the output word. 

#### Gradient for output word

We can also calculate the gradient with respect to the output word.

\begin{align*}
\frac{\partial }{\partial \boldsymbol{u_w}}J_{\mathrm{softmax-CE}}(\boldsymbol{o}, \boldsymbol{v_c}, \boldsymbol{U}) & = \frac{\partial J_{\mathrm{CE}}(\theta)}{\partial \theta_i} \frac{\partial \theta_i}{\boldsymbol{u_w}} \\ & = (\hat y_i - y_i) \boldsymbol{v_c}
\end{align*}

Notice here we apply $\frac{\partial \boldsymbol{a}^{\mathsf{T}} \boldsymbol{x}}{\partial \boldsymbol{a}} = x$. Writen the gradient in matrix format, we have

\begin{equation*}
\frac{\partial J}{\partial \boldsymbol{U}} =  \boldsymbol{v_c}(\boldsymbol{\hat y} - \boldsymbol{y})^{\mathsf{T}}
\end{equation*}

Notice the shape of the gradient. It determines how the notation looks like. In the above notation notice the shape of the output word gradient $\frac{\partial J}{\partial \boldsymbol{U}}$ is $d \times W$. As we will discussion in the next post that it is a convention to make the shape of gradient as the shape of the input vectors. in this case the shape of $U$ and $\frac{\partial J}{\partial \boldsymbol{U}}$ are the same.

### Negative sampling

The cost function for a single word prediction using nagative sampling is the following

\begin{align*}
J_{\mathrm{neg-sample}}(\boldsymbol{o},\boldsymbol{v_c},\boldsymbol{U}) & = -\log(\sigma(\boldsymbol{u_o}^{\mathsf{T}} \boldsymbol{v_c})) - \sum_{k=1}^{K} \log(\sigma(-\boldsymbol{u_k}^{\mathsf{T}} \boldsymbol{v_c}))
\end{align*}

It comes from the original paper by Mikolov et al. $\sigma$ is the sigmoid function $\sigma(x) = \frac{1}{1+e^{-x}}$. The ideas is to reduce the optimization computation by only sampling a small part of the vocabulary that have a lower probability of being context words of one another. The first lecture notes from CS224N discussed briefly the origin and intuition of the negative sampling loss funciton. Here we will focus on deriving the gradient and implementation ideas.

With the fact that $\frac{\mathrm{d}\sigma(x)}{\mathrm{d} x} = \sigma(x)(1-\sigma(x))$ and the chain rule, it is not hard to derive the gradients result as following

\begin{align*}
\frac{\partial J}{\partial \boldsymbol{v_c}} & = (\sigma(\boldsymbol{u_o}^{\mathsf{T}}\boldsymbol{v_c}) - 1)\boldsymbol{u_o} - \sum_{k=1}^{K}(\sigma(-\boldsymbol{u_k}^{\mathsf{T}}\boldsymbol{v_c}) - 1)\boldsymbol{u_k} \\
\frac{\partial J}{\partial \boldsymbol{u_o}} & = (\sigma(\boldsymbol{u_o}^{\mathsf{T}}\boldsymbol{v_c}) - 1)\boldsymbol{v_c} \\
\frac{\partial J}{\partial \boldsymbol{u_k}} & = -\sigma(-\boldsymbol{u_k}^{\mathsf{T}}\boldsymbol{v_c}) - 1)\boldsymbol{v_c},  \mathrm{for\ all\ } k = 1, 2, \cdots, K 
\end{align*}

How to sample the $\boldsymbol{u_k}$ in practice? The best known sampling method is based on the Unigram Model raise to the power of 3/4. Unigram Model is the counts of each word in the particular corpus (not the vocabulary).

#### Gradient for all of word vectors

Since skip-gram model is using one center word to predict all its context words. Given a word context size of $m$, we obtain a set of context words $[\mathrm{word}_{c-m}, \cdots, \mathrm{word}_{c-1}, \mathrm{word}_{c}, \mathrm{word}_{c+1}, \cdots, \mathrm{word}_{c+m}]$. For each window, We need to predict $2m$ context word given the center word. Denote the "input" and "output" word vectors for $\mathrm{word}_k$ as $\boldsymbol{v}_k$ and $\boldsymbol{u}_k$ respectively. The cost for the entire context window with size $m$ centered around $\mathrm{word}_c$would be 

\begin{equation*}
J_{\mathrm{skip-gram}}(\mathrm{word}_{c-m \;\cdots\; c+m}) = \sum_{\substack{-m \le j \le m\\ 
j \ne 0}} F(\boldsymbol{u}_{c+j}, \boldsymbol{v}_c).
\end{equation*}

$F$ is a placeholder notation to represent the cost function given the center word for different model. Therefore for skip-gram, the gradients for the cost of one context window are

\begin{align*}
\frac{J_{\mathrm{skip-gram}}(\mathrm{word}_{c-m \;\cdots\; c+m})}{\partial \boldsymbol{U}} & = \sum_{\substack{-m \le j \le m\\ j \ne 0}} \frac{\partial F(\boldsymbol{u}_{c+j}, \boldsymbol{v}_c)}{\partial \boldsymbol{v}_c},\\
\frac{J_{\mathrm{skip-gram}}(\mathrm{word}_{c-m \;\cdots\; c+m})}{\partial \boldsymbol{v}_c} & = \sum_{\substack{-m \le j \le m\\ j \ne 0}} \frac{\partial F(\boldsymbol{u}_{c+j}, \boldsymbol{v}_c)}{\partial \boldsymbol{v}_c}, \\
\frac{J_{\mathrm{skip-gram}}(\mathrm{word}_{c-m \;\cdots\; c+m})}{\partial \boldsymbol{v}_j} & = 0, \forall j \ne c
\end{align*}

### CBOW model

Continuous bag-of-words (CBOW) is using the context words to predect the center words. Different from skip-gram model, in CBOW, we will use $2m$ context word vectors as we predict probability of a word is the center word. For a simple variant of CBOW, we could sum up all the $2m$ word vectors in one context vector $\hat{\boldsymbol{v}}$ and use the similar cost function with $\mathrm{softmax}$ as we did in skip-gram model.

\begin{equation*}
\hat{\boldsymbol{v}} = \sum_{\substack{-m \le j \le m\\ 
j \ne 0}} \boldsymbol{v}_{c+j}.
\end{equation*}

Similar to skip-gram, we have

\begin{align*}
\frac{J_{\mathrm{CBOW}}(\mathrm{word}_{c-m \;\cdots\; c+m})}{\partial \boldsymbol{U}} & = \frac{\partial F(\boldsymbol{u}_{c}, \hat{\boldsymbol{v}})}{\partial \boldsymbol{U}},\\
\frac{J_{\mathrm{CBOW}}(\mathrm{word}_{c-m \;\cdots\; c+m})}{\partial \boldsymbol{v}_j} & = \frac{\partial F(\boldsymbol{u}_{c}, \hat{\boldsymbol{v}})}{\partial \boldsymbol{v}_j}, \forall j \in \{c-m, \cdots, c-1, c+1, \cdots, c+m\}\\
\frac{J_{\mathrm{CBOW}}(\mathrm{word}_{c-m \;\cdots\; c+m})}{\partial \boldsymbol{v}_j} & = 0, \forall j \notin \{c-m, \cdots, c-1, c+1, \cdots, c+m\}.
\end{align*}

### GloVe model
todo, the paper and lecture.

### Word vector evaluation

### Summary

This post focused on gradient derivation for various word embedding models. The bootstrap model is based on the distributional semantic, by which to predict the probability of a word given some other words from the fixed corpus. We use the $\mathrm{softmax}$ function to computer the probability. To update the word vectors, we introduce the likelihood function and derived its gradient. In that derivation, the parameter $\boldsymbol{\theta}$ is the hyper parameters related to word vector that can be used to compute the probability. We continue to derive the gradient of the cross entropy loss function for a single word prediction. The result gradient of the cross entropy loss function is the key for all our later gradient derivation. After that we introduce the word2vec family of word embedding, skip-gram and CBOW models. In word2vec we use the dot product of the center word and its context words to compute the probability. We derive the gradient with respect both center word and context word when using cross entropy loss funciton. Negative sampling is also discussed as it improves the computation cost by many factors. With the discussion of skip-gram model, CBOW model is easier to present. One thing you need to distinguish is that whether the gradient is for a particualr word prediction, for the whole window of the words, or the over all objective for the corpus. For skip-gram, we first compute the gradient for each word prediction in the given context window, then sum up all the gradient to update the cost function of that window. We move the window over the corpus until finish all the word updates. This whole process is only one update. While using negative sampling, the process becomes more efficient, we don't have to go through all the window, only sampling $K$ windows with Unigram Model raise to the $3/4$ power. In the simple CBOW model discussed, we add up all the context vectors first, then only update a single gradient for that window corresponding to the center word. We repeat this process for different windows to complete one update.

  [1]: http://static.zybuluo.com/iurnah/pr0lsuygcp9a2fxusnpdmcyi/word2vec1.png
  [2]: http://static.zybuluo.com/iurnah/csk9axyf2xvsse0zew8sztc6/word2vec2.png

## Part 2 Neural Networks and Backpropagation

Date: April 15th 2019
We discussed the softmax classifier in Part 1 and its major drawback that the classifier only gives linear decision boundaries. In Part 2, Neural Networks will be introduced to demonstrate that it can learn much more complex functions and nonlinear decision boundaries.

### Intro to Neural Networks

| Biological Neuron | Mathematical Model | Simplified Neuron |
|:---:|:---:|:---:|
| ![Biological Neuron][1] | ![Mathematical model][2] | ![simplified neuron][3] |

The neuron can be modeled as a binary logistic regression unit as in the right figure above. It can be further simplified as following functions,

\begin{equation*} 
h_{w, b} (\boldsymbol{x})=f(\boldsymbol{w}^\mathsf{T} \boldsymbol{x} + b) \\
f(z) = \frac{1}{1 + e^{-z}}.
\end{equation*}

* $\boldsymbol{x}$ is the inputs
* $\boldsymbol{w}$ is the weights
* $b$ is a bias term
* $h$ is a hidden layer function
* $f$ is a nonlinear activation function (sigmoid, tanh, etc.)
 
If we feed a vector of inputs through a bunch of logistic regression (sigmoid) functions, then we get a vector of outputs. The [CS224N lecture note 3 secion 1.2][4] have a complete derivation on multiple sigmoid units. But we don’t have to decide ahead of time what variables these logistic regressions are trying to predict! It is the loss function that will direct what the intermediate hidden variables should be, so as to do a good job at predicting the targets for the next layer, etc.

### Forward Propagation in Matrix Notation

In a multilayer neural network, not only did we have multiple sigmoid units, but we also have more than one layer. Let's explicitly write down the signal transformation (aka forward propagation) from one layer to another referring to this network from Andrew's ML course.

![Neural_network.png][5]

We will use the following notation convention

* $a^{(j)}_i$ to represent the "activation" of unit $i$ in layer $j$.
* $W^{(j)}$ to represent the matrix of weights controlling function mapping from layer $j$ to layer $j + 1$.

The value at each node can be calculated as

\begin{equation*}
a^{(2)}_1 = f(w^{(1)}_{10} x_0 + w^{(1)}_{11} x_1 + w^{(1)}_{12} x_2 + w^{(1)}_{13} x_3) \\
a^{(2)}_2 = f(w^{(1)}_{20} x_0 + w^{(1)}_{21} x_1 + w^{(1)}_{22} x_2 + w^{(1)}_{23} x_3) \\
a^{(2)}_3 = f(w^{(1)}_{30} x_0 + w^{(1)}_{31} x_1 + w^{(1)}_{32} x_2 + w^{(1)}_{33} x_3) \\
\text{and} \\
h_{w} (\boldsymbol{x}) = a^{(3)}_1 = f(w^{(2)}_{10} a^{(2)}_0 + w^{(2)}_{11} a^{(2)}_1 + w^{(2)}_{12} a^{(2)}_2 + w^{(2)}_{13} a^{(2)}_3)
\end{equation*}

write the matrix $W^{(j)}$ explicity,
\begin{equation*}
W^{(1)} = 
\begin{bmatrix} 
w^{(1)}_{10} & w^{(1)}_{11} & w^{(1)}_{12} & w^{(1)}_{13} \\
w^{(1)}_{20} & w^{(1)}_{21} & w^{(1)}_{22} & w^{(1)}_{23} \\
w^{(1)}_{30} & w^{(1)}_{31} & w^{(1)}_{32} & w^{(1)}_{33}
\end{bmatrix}
\end{equation*}

\begin{equation*}
W^{(2)} = 
\begin{bmatrix} 
w^{(2)}_{10} & w^{(2)}_{11} & w^{(2)}_{12} & w^{(2)}_{13}
\end{bmatrix}
\end{equation*}

With the above form, we can use matrix notation as

\begin{align*}
& \boldsymbol{a}^{(2)} = f(\boldsymbol{W}^{(1)} \boldsymbol{x}) \\
h_{w} (\boldsymbol{x})  = & \boldsymbol{a}^{(3)} = f(\boldsymbol{W}^{(2)} \boldsymbol{a}^{(2)})
\end{align*}

We can see from above notations that if the network has $s_j$ units in layer $j$ and $s_{j+1}$ in layer $j + 1$, the matrix $W^{(j)}$ will be of dimention $s_{j+1} \times s_{j}+1$. It could be interpreted as "the dimention of $W^{(j)}$ is the number of nodes in the next layer (layer $j + 1$) $\times$ the number of nodes in the current layer $+$ 1.

> Note that in cs224n the matrix notation is slightly different,
> \begin{align*}
& \boldsymbol{a}^{(2)} = f(\boldsymbol{W}^{(1)} \boldsymbol{x} + \boldsymbol{b}^{(1)}) \\
h_{w} (\boldsymbol{x})  = & \boldsymbol{a}^{(3)} = f(\boldsymbol{W}^{(2)} \boldsymbol{a}^{(2)} + \boldsymbol{b}^{(2)})
\end{align*}
the different is from how we denote the bias. The two are enssentially the same, but be cautions that the matrix dimentions are different. 

### Word Window Classification Using Neural Networks

From now on, let's switch the notation to cs224n so that we can derive the backpropagation algorithm for word window classification and get more intuition about the backprop. The drawing of the neural nets in cs224n for word window classification is less dynamic and slitly different from the drawing from Andrew's ML class. The figure from cs224n may be slightly confusing at first, but it is good to understand it from this particular application, word window classification.

#### Forward propagation

Firstly, the goal of this classification task is to classify whether the center word is a location. Similar to word2vec, we will go over all positions in a corpus. But this time, it will be supervised and only some positions should get a high score.

![Screen Shot 2019-04-18 at 10.50.59 PM.png-57.6kB][6]

The figure above illustrate the feed-forward process. We use the method by Collobert & Weston (2008, 2011). An unnormalized score will be calculated from the activation $\boldsymbol{a} = [a_1, a_2, a_3, \cdots]^\mathsf{T}$.

\begin{equation*}
\text{scroe}(\boldsymbol{x}) = U^\mathsf{T} \boldsymbol{a} \in \mathbb{R}
\end{equation*}

We will use max-margin loss as our loss function. The training is essentially to find the optimal weights $W$ by minimize the max-margin loss 

\begin{equation*}
J = \max(0, 1 - s + s_c)
\end{equation*}

$s$ is the score of a window that have a location in the center. $s_c$ is the score of a window that doesn't have a location in the center. For full objective function: Sample several corrupt windows per true one. Sum over all training windows. It is similar to negative sampling in word2vec.

We will use gradient descent to update the parameter so as to minimize the loss function. The key is how to calculate the gradient with respect to the model parameters, namely $\nabla_{\theta} J(\theta)$. Here we use $\theta$ to represent the hyperthetic parameters, it can include the $W$ and other parameters of the model.

\begin{equation*}
\theta^{\text{new}} = \theta^{\text{old}} - \alpha\nabla_{\theta} J(\theta)
\end{equation*}

#### Gradients and Jacobians Matrix

At first, let us layout the input and all the equations in this simple neural network.
| Input Layer | Hidden Layer | Output Layer |
|:---:|:---:|:---:|
| $\boldsymbol{x}$ | $\boldsymbol{z} = \boldsymbol{W} \boldsymbol{x} + \boldsymbol{b}, \\ \boldsymbol{h} = f(\boldsymbol{z})$ | $ s = \boldsymbol{u}^{\mathsf{T}}\boldsymbol{h}$ |

To update the parameters in this model, namely $\boldsymbol{x}, \boldsymbol{W}, \boldsymbol{b}$, we would like to compute the derivitavies of $s$ with respect to all these parameters. We have to use the chain rule to compute it. What chain rule says is that we can compute the partial derivatives of each individual functions and then multiply them together to get the derivative with respect the specific variable. For example, $\frac{\partial s}{\partial \boldsymbol{b}}$ is computed as $\frac{\partial s}{\partial \boldsymbol{b}} = \frac{\partial s}{\partial \boldsymbol{h}} \cdot \frac{\partial \boldsymbol{h}}{\partial \boldsymbol{z}} \cdot \frac{\partial \boldsymbol{z}}{\partial \boldsymbol{b}}$. This seems very easy to understand, but when it comes to implemenation in vectorized format, it become confusing for those who doesn't work on matrix calculus for quite a while like me. I want to get the points straight here. What exactly is $\frac{\partial s}{\partial \boldsymbol{b}}$ and $\frac{\partial \boldsymbol{h}}{\partial \boldsymbol{z}}$. Note both  $\boldsymbol{h}$ and $\boldsymbol{z}$ are vectors. To calculate these two gradient, simply remember the following two rules:

1. Given a function with 1 output and $n$ inputs $f(\boldsymbol{x}) = f(x_1, x_2, \cdots, x_n)$, it's gradient is a vector of partial derivatives with respect to each input(take the gradient element wise).
\begin{equation*}
\frac{\partial f}{\partial \boldsymbol{x}} = \Bigg [
\frac{\partial f}{\partial x_1},
\frac{\partial f}{\partial x_2},
\cdots,
\frac{\partial f}{\partial x_n},
\Bigg ]
\end{equation*}

2. Given a function with $m$ output and $n$ inputs $\boldsymbol{f}(\boldsymbol{x}) = \big [f_1(x_1, x_2, \cdots, x_n), \cdots, f_m(x_1, x_2, \cdots, x_n) \big ]$, it's gradient is an $m \times n$ matrix of partial derivatives $(\frac{\partial \boldsymbol{f}}{\partial \boldsymbol{x}})_{ij} = \dfrac{\partial f_i}{\partial x_j}$. This matrix is also called Jacobian matrix. 

\begin{equation*}
\frac{\partial \boldsymbol{f}}{\partial \boldsymbol{x}} 
= \begin{bmatrix}
\dfrac{\partial \boldsymbol{f}}{\partial x_1} & \cdots & \dfrac{\partial \boldsymbol{f}}{\partial x_n} 
\end{bmatrix}
= \begin{bmatrix}
\dfrac{\partial f_1}{\partial x_1} & \cdots & \dfrac{\partial f_1}{\partial x_n}\\
\vdots & \ddots & \vdots\\
\dfrac{\partial f_m}{\partial x_1} & \cdots & \dfrac{\partial f_m}{\partial x_n} \end{bmatrix}
\end{equation*}

#### Computing gradients with the chain rule 

With these two rules we can calculate the partials. We will use $\frac{\partial s}{\partial \boldsymbol{b}}$ as an example.

\begin{equation*}
\frac{\partial s}{\partial \boldsymbol{b}} = \frac{\partial s}{\partial \boldsymbol{h}} \frac{\partial \boldsymbol{h}}{\partial \boldsymbol{z}} \frac{\partial \boldsymbol{z}}{\partial \boldsymbol{b}}
\end{equation*}

* $\frac{\partial s}{\partial \boldsymbol{h}} = \frac{\partial}{\partial \boldsymbol{h}}(\boldsymbol{u}^{\mathsf{T}}\boldsymbol{h}) = \boldsymbol{u}^{\mathsf{T}}$
*  $\frac{\partial \boldsymbol{h}}{\partial \boldsymbol{z}} = \frac{\partial}{\partial \boldsymbol{z}}(f(\boldsymbol{z})) = \begin{pmatrix}
f'(z_1) & & 0 \\
& \ddots & \\
0 & & f'(z_n) 
\end{pmatrix} = \mathrm{diag} (f' (\boldsymbol{z}))$
*  $\frac{\partial \boldsymbol{z}}{\partial \boldsymbol{b}} = \frac{\partial}{\partial \boldsymbol{b}}(\boldsymbol{W}\boldsymbol{x} + \boldsymbol{b}) = \boldsymbol{I}$

Therefore,

\begin{equation*}
\frac{\partial s}{\partial \boldsymbol{b}} = \boldsymbol{u}^{\mathsf{T}} \mathrm{diag}(f'(\boldsymbol{z})) \boldsymbol{I} = \boldsymbol{u}^{\mathsf{T}} \circ f'(\boldsymbol{z})
\end{equation*}

Similarly we can calculate the partial with respect to $\boldsymbol{W}$ and $\boldsymbol{x}$. Since the top layer partials are already calculated, we can reuese the results. We denote those reusable partials as $\delta$ meaning local error signal.

\begin{align*}
\frac{\partial s}{\partial \boldsymbol{W}} & =  \frac{\partial s}{\partial \boldsymbol{h}} \frac{\partial \boldsymbol{h}}{\partial \boldsymbol{z}} \frac{\partial \boldsymbol{z}}{\partial \boldsymbol{W}} \\ \\
\frac{\partial s}{\partial \boldsymbol{x}} & = \frac{\partial s}{\partial \boldsymbol{h}} \frac{\partial \boldsymbol{h}}{\partial \boldsymbol{z}} \frac{\partial \boldsymbol{z}}{\partial \boldsymbol{x}} \\ \\
\frac{\partial s}{\partial \boldsymbol{b}} & =  \underbrace{\frac{\partial s}{\partial \boldsymbol{h}}
\frac{\partial \boldsymbol{h}}{\partial \boldsymbol{z}}}_{\mathrm{reuse}} \frac{\partial \boldsymbol{z}}{\partial \boldsymbol{b}} = \boldsymbol{\delta} \frac{\partial \boldsymbol{z}}{\partial \boldsymbol{b}} = \boldsymbol{\delta} = \boldsymbol{u}^{\mathsf{T}} \circ f'(\boldsymbol{z})
\end{align*}

#### Shape convention

What does the shape of the derivatives looks like in practice? How can we make the chain rule computation efficient? According to the aforementioned gradient calculation rules, $\frac{\partial s}{\partial \boldsymbol{W}}$ is a row vector. The chain rule gave 

\begin{equation*}
\frac{\partial s}{\partial \boldsymbol{W}}  =  \boldsymbol{\delta} \frac{\partial \boldsymbol{z}}{\partial \boldsymbol{W}}.
\end{equation*}

We know from Jacobians that  $\frac{\partial\boldsymbol{z}}{\partial \boldsymbol{W}} = \frac{\partial}{\partial \boldsymbol{W}}(\boldsymbol{W}\boldsymbol{x} + \boldsymbol{b}) =  \boldsymbol{x}^{\mathsf{T}}$. We may arrived the result that $\frac{\partial s}{\partial \boldsymbol{W}}  =  \boldsymbol{\delta} \ \boldsymbol{x}^{\mathsf{T}}$. This is actually not quite wirte. The correct form should be 

\begin{align*}
\underbrace{\frac{\partial s}{\partial \boldsymbol{W}}}_{n \times m}  = \underbrace{\boldsymbol{\delta}^{\mathsf{T}}}_{n \times 1} \ \underbrace{\boldsymbol{x}^{\mathsf{T}}}_{1 \times m}.
\end{align*}

You may wonder why is this form instead of the one we derived directly. The explanation from the CS224N is that we would like to follow the shape convention so as to make the chain rule implementation more efficient (matrix multiplication instead of loops). __Different from the Jacobian form, shape convention states that the shape of the gradient is the same shape of parameters.__ The resolution here is to use Jacobian form as much as possible and reshape to follow the convention at the end. Because Jacobian form makes the chain rule easy and the shape convention makes the implementation of gradient descent easy.

### Computation graphs and backpropagation

We have shown how to compute the partial derivatives using chain rule. This is almost the backpropagation alrogithm. If we want to add on to that and make the algorithm complete, the only thing we need is how to reuse gradient computed for higher layers. We leverage the computation graph to explain this. From the computation graph, you'll see the intuiation of reusing the partial derivatives computed for higher layers in computing derivatives for lower layers so as to minimize computation.

We represent our neural net equations as a computation graph as following:

![computation graph.png-31.8kB][7]
 
* source nodes represent inputs 
* interior nodes represent operations
* edges pass along result of the operation

When following computation order to carry out the computations from inputs, it is enssetially forward propagation. Backpropagation is to pass along gradients backwards. This can be illustrated in the following computation graph.

![computation graph backprop.png-50.6kB][8]

The partial derivative with respect to a parameter reflect how changing the parameter would effect the output value. The output value in the backprop is usually a loss function (or error). Intuitively, you can see backprop is push the error back to the lower layer through a bunch of operation nodes, the arrived error is a measure of the error at that particular layer, training is try to reduce this backprop-ed error by adjusting the local parameters, the effect of reducing the local error will be forward propagated to the output, the error at the output should also reduced. We use gradient descent to make this process to converge as soon as possible. 

To understand it better, let's look at a single node as following. 

![local gradient chain rule.png-63.1kB][9]

We define "local gradient" for each node as the gradient of it's output with respect to it's input. By chain rule, the downstream gradient is equal to the multiplication of the upstream gradient and the local gradient. When having multipe local gradients, they are pushed back to each input using chain rule. CS224N lecture 4 slides have step by step example of backprop. From the example, we can got some intuitions about some nodes' effects. For example, when push back gradients along outward branches from a single node, the gradients should be sumed up; "+" node distributes the upstream gradient to each summand; "max" node simply routes the upstream gradients.

When update gradient with backprop, you should compute all the gradients at once. With this computation graph notion, following routine captures the gist of backprop in a very decent manner.

> 1.  Fprop: visit nodes in topological sorted order
 * Compute value of node given predecessors
2. Bprop:
  - initialize output gradient = 1
  - visit nodes in reverse order:
      - Compute gradient wrt each node using gradient wrt successors
      - $\{y_1, y_2, \cdots, y_2\} = \mathrm{successors\ of\ } x$
      
\begin{equation*}\frac{\partial z}{\partial \boldsymbol{x}} = \sum_{i=i}^n \frac{\partial z}{\partial y_i} \frac{\partial y_i}{\partial x} \end{equation*}

If done correctly, the big $O$ complexity of forward propagation and backpropagation is the same.

### Automatic differentiation

The gradient computation can be automatically inferred from the
symbolic expression of the fprop. but this is not commonly used in practice. Each node type needs to know how to compute its output and how to compute the gradient wrt its inputs given the gradient wrt its output. Modern DL frameworks (Tensorflow, PyTorch, etc.) do backpropagation for you but mainly leave layer/node writer to hand-calculate the local derivative. Following is a simple demo of how to implement forward propagation and backpropagation.

```python
class MultiplyGate(object):
    def forward(x, y):
        z = x * y
        self.x = x # must keep these around!
        self.y = y
        return z
        
    def backword(dz):
        dx = self.y * dz # [dz/dx * dL/dz]
        dy = self.x * dz # [dz/dy * dL/dz]
        return [dx, dy]
```

9. nonlinear activation functions: sigmoid, tanh, hard tanh, ReLU
10. Learning rates: start 0.001. power of 10. halve the learning rate every k epochs. formula: $lr = lr_0 e^{-k t}$ for each epoch $t$

## Regularization
LFD

1. starting point: L2 regularization

  [1]: http://cs231n.github.io/assets/nn1/neuron.png
  [2]: http://cs231n.github.io/assets/nn1/neuron_model.jpeg
  [3]: http://static.zybuluo.com/iurnah/vwr6yjxddccv1qhl0g9sdcro/simplified%20neuron.png
  [4]: https://web.stanford.edu/class/archive/cs/cs224n/cs224n.1174/lecture_notes/cs224n-2017-notes3.pdf
  [5]: http://static.zybuluo.com/iurnah/m3g37lozjn6dq9sqa1x05ph0/Neural_network.png
  [6]: http://static.zybuluo.com/iurnah/jtmvh9jagsxznbfd4bscavpb/Screen%20Shot%202019-04-18%20at%2010.50.59%20PM.png
  [7]: http://static.zybuluo.com/iurnah/8bknov4w2x9igornns45qkty/computation%20graph.png
  [8]: http://static.zybuluo.com/iurnah/axcuqzot3sjkopc0pauxnxd1/computation%20graph%20backprop.png
  [9]: http://static.zybuluo.com/iurnah/b8zvwgl64h6y4y98lxj0vpxr/local%20gradient%20chain%20rule.png

## Part 3 Language Model and RNN

### n-gram

* assumption: $\boldsymbol{x}^{(t+1)}$ depends only on previous $n-1$ words.
* Sparsity problem, --> backoff (n-1)-gram
* practically, n has to be less than 5.

### Neural language model

* fixed-window neural language model
* Recurrent Neural Ne