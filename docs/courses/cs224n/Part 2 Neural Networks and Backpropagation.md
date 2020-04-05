# Part 2 Neural Networks and Backpropagation

Date: April 15th 2019
We discussed the softmax classifier in Part 1 and its major drawback that the classifier only gives linear decision boundaries. In Part 2, Neural Networks will be introduced to demonstrate that it can learn much more complex functions and nonlinear decision boundaries.

## Intro to Neural Networks

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

If we feed a vector of inputs through a bunch of logistic regression (sigmoid) functions, then we get a vector of outputs. The [CS224N lecture note 3 section 1.2][4] has a complete derivation on multiple sigmoid units. But we don’t have to decide ahead of time what variables these logistic regressions are trying to predict! It is the loss function that will direct what the intermediate hidden variables should be, to do a good job at predicting the targets for the next layer, etc.

## Forward Propagation in Matrix Notation

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

the difference is from how we denote the bias. The two are enssentially the same, but be cautions that the matrix dimentions are different.

# Word Window Classification Using Neural Networks

From now on, let's switch the notation to cs224n so that we can derive the backpropagation algorithm for word window classification and get more intuition about the backprop. The drawing of the neural nets in cs224n for word window classification is less dynamic and slitly different from the drawing from Andrew's ML class. The figure from cs224n may be slightly confusing at first, but it is good to understand it from this particular application, word window classification.

## Forward propagation

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

## Gradients and Jacobians Matrix

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

## Computing gradients with the chain rule 

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

## Shape convention

What does the shape of the derivatives looks like in practice? How can we make the chain rule computation efficient? According to the aforementioned gradient calculation rules, $\frac{\partial s}{\partial \boldsymbol{W}}$ is a row vector. The chain rule gave 

\begin{equation*}
\frac{\partial s}{\partial \boldsymbol{W}}  =  \boldsymbol{\delta} \frac{\partial \boldsymbol{z}}{\partial \boldsymbol{W}}.
\end{equation*}

We know from Jacobians that  $\frac{\partial\boldsymbol{z}}{\partial \boldsymbol{W}} = \frac{\partial}{\partial \boldsymbol{W}}(\boldsymbol{W}\boldsymbol{x} + \boldsymbol{b}) =  \boldsymbol{x}^{\mathsf{T}}$. We may arrived the result that $\frac{\partial s}{\partial \boldsymbol{W}}  =  \boldsymbol{\delta} \ \boldsymbol{x}^{\mathsf{T}}$. This is actually not quite wirte. The correct form should be 

\begin{align*}
\underbrace{\frac{\partial s}{\partial \boldsymbol{W}}}_{n \times m}  = \underbrace{\boldsymbol{\delta}^{\mathsf{T}}}_{n \times 1} \ \underbrace{\boldsymbol{x}^{\mathsf{T}}}_{1 \times m}.
\end{align*}

You may wonder why is this form instead of the one we derived directly. The explanation from the CS224N is that we would like to follow the shape convention so as to make the chain rule implementation more efficient (matrix multiplication instead of loops). __Different from the Jacobian form, shape convention states that the shape of the gradient is the same shape of parameters.__ The resolution here is to use Jacobian form as much as possible and reshape to follow the convention at the end. Because Jacobian form makes the chain rule easy and the shape convention makes the implementation of gradient descent easy.

## Computation graphs and backpropagation

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

## Automatic differentiation

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
2. f


  [1]: http://cs231n.github.io/assets/nn1/neuron.png
  [2]: http://cs231n.github.io/assets/nn1/neuron_model.jpeg
  [3]: http://static.zybuluo.com/iurnah/vwr6yjxddccv1qhl0g9sdcro/simplified%20neuron.png
  [4]: https://web.stanford.edu/class/archive/cs/cs224n/cs224n.1174/lecture_notes/cs224n-2017-notes3.pdf
  [5]: http://static.zybuluo.com/iurnah/m3g37lozjn6dq9sqa1x05ph0/Neural_network.png
  [6]: http://static.zybuluo.com/iurnah/jtmvh9jagsxznbfd4bscavpb/Screen%20Shot%202019-04-18%20at%2010.50.59%20PM.png
  [7]: http://static.zybuluo.com/iurnah/8bknov4w2x9igornns45qkty/computation%20graph.png
  [8]: http://static.zybuluo.com/iurnah/axcuqzot3sjkopc0pauxnxd1/computation%20graph%20backprop.png
  [9]: http://static.zybuluo.com/iurnah/b8zvwgl64h6y4y98lxj0vpxr/local%20gradient%20chain%20rule.png