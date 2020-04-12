# Contextual Multi-Armed Bandit

https://wiki.rui-han.com/index.php/Private:Research/Multi-Armed_Bandit

## Multi-armed bandit problems

NB:

1. action is equivalent to arm, action $i$ means play the $i$-th arm

> $T$: total rounds
> $K$: number of arms
> $G_t(k)$: reward (gain) obtained by play the $k$-th arm at the $t$-th round
> $R_t(k)$: regret (loss) incurr by play the $k$-th arm at the $t$-th round
> $\mu^*$: the optimal expected payoff for the best arm (action)

## Brute force algorithm

* This doesn't apply to the real setting, because in the real bandit problem you
  can only play one arm at each round. But it doesn't heart that we evaluate
  this scenario as a benchmark for other algorithms.
* In this algorithm, we basically evaluate all arms in every round, pretended we
  are in god's angle and know the underline distribution of each arm.

## Optimistic initialization

* This is what we do in the supervised training,

## Random selection algorithm

* Each step, you select one classifier uniform randomly and calculate the reward.

## Greedy ($\epsilon$-Greedy) algorithm

* In each round, switch to a random arm with probability $\epsilon$, otherwise
  stick on the current optimal arm.
* The first few rounds are important. For example, if the optimal arm is
  selected in the beginning, this algorithm will achieve better performance; If
  the worst arm is selected in the beginning, it will stuck in getting the least reward.

## Upper Confidence bound

* Confidence bound is usually misunderstood by it's name. It is NOT the
  probability of certain outcome fall between the lower bound and upper bound
  (confidence interval). Confidence bound also called confidence interval, both
  the lower bound and upper bound are random variables. It describe the property
  of a method to come up a confidence bound in individual trails. It can be
  iterpreted as the probability that a realized confidence bound include the
  ture estimate.
* Notice UCB method is a classic statistic method. The expected reward $\mu_k$
  is treated as a fixed value, not a random variable. In Bayesian bandit, the
  expected reward is measured as random variable.
* For arm $a$, $\hat Q_t(a)$ is the sample mean, $\hat U_t(a)$ is the upper
  confidence bound. $Q_t(a)$ is the true mean, then we have
  $Q_t(a) \le \hat Q_t(a) + \hat U_t(a)$. $\hat U_t(a)$ is inverse proportional
  to $N_t(a)$, the total number of selection of arm $a$, because the more we
  played the less uncertain we have for $\hat Q_t(a)$, thus the upper bound
  shrink as $N_t(a)$ increases.
* In UCB method, we select the arm with highest upper confidence bound, this is
  because it is not played sufficient enough and we believe this arm have the
  high potential to be the optimal arm. Formally, UCB is equivalent to the
  following optimization problem:

$$
a_t = \operatorname*{arg\,max}_{a \in A} \hat Q_t(a) + \hat U_t(a)
$$

## UCB1

## Bayesian bandit (Thompson Sampling)

## Exploitation versus exploration

## Contextual multi-armed bandit

## Reference