# KahanMPI

The goal of this project is to address the challenges of reproducibility
and accuracy faced by MPI applications when using floating-point reductions.

In particular, we aspire to implement numerically if not bit-wise
reproducible MPI reductions using a variety of well-known algorithms
for such purposes, including Kahan (compensated) summation.

# Known Issues

The CMake build system is broken.  Do not use it.  And by, I mean CMake.  Ever.
