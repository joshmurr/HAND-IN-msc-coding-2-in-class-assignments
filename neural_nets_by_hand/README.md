# Neural Networks by Hand

Following along with Andrej Karpathy's [Hackers Guide to Neural Networks](https://karpathy.github.io/neuralnets/) and porting the code to Python. The initial code is from the [Msc-Coding-2 Github page](https://github.com/ual-cci/MSc-Coding-2/blob/master/Week-5.md) but I've continued with the tutorial to get the simple 2-layer neural network working.

Neural networks are a huge and complicated topic, but I don't often like to learn the specifics of a library or API like Tensorflow without at least know a little bit about what is happening under the hood. So for my own curiosities sake I thought it would be useful to see the _Hackers Guide to NN_ through to the end. I did find it useful to see the literal implementation of a neuron and how it can "learn" through tweaking the gradient of the derivative in backpropagation.

It was also a good excercise in writing classes in Python which is new to me.

Although these 'toy examples' are not particularly useful, I would be interested to see if there is some practical application in writing an exremely light-weight neural network which could sit on an Arduino or something, learning from simple data sources.

---

## To run:

```
jupyter-notebook
```

The simple 2-layer network based on a __Support Vector Machine__ (SVM) achieves the following results:

```
Training accuracy at iter 0: 0.3333333333333333
Training accuracy at iter 25: 0.3333333333333333
Training accuracy at iter 50: 0.5
Training accuracy at iter 75: 0.5
Training accuracy at iter 100: 0.6666666666666666
Training accuracy at iter 125: 0.8333333333333334
Training accuracy at iter 150: 0.6666666666666666
Training accuracy at iter 175: 0.8333333333333334
Training accuracy at iter 200: 1.0
Training accuracy at iter 225: 0.8333333333333334
Training accuracy at iter 250: 1.0
Training accuracy at iter 275: 0.8333333333333334
Training accuracy at iter 300: 0.8333333333333334
Training accuracy at iter 325: 0.8333333333333334
Training accuracy at iter 350: 1.0
Training accuracy at iter 375: 1.0
```
