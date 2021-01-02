# Divide & Conquer

The goal of this project was to create a skeleton interface for the parallel programming padigram Divide and Conquer that clients would use without dealing with concurrency themselves.

This skeleton takes 4 lambdas in the constructor which represent the `divide`, `combine`, `base` and `threshold` functions respectively. The skeleton then sovles the problem and handles load-balancing, work stealing and task dependencies automatically. This [paper](https://dl.acm.org/doi/pdf/10.1145/3002125.3002128?fbclid=IwAR0BW5V69q3riFZCufD5Tdy6ABGkSESfV5gDZBatBoDbQ3iY4Au7jnZhars) was used as inspiration.

# How to run
 * `cd src`
 * `make`
 * `./main`
