#include <cassert>
#include <cmath>    /* declares ldexp() */
#include <iostream>
#include <new>      /* declares new[]   */

template <class T>
T BasicSum(size_t n, T * input)
{
    T sum = T(0);
    for (auto i=0; i<n; ++i)
        sum += input[i];
    return sum;
}

/* Adapted from http://en.wikipedia.org/wiki/Kahan_summation_algorithm */
template <class T>
T KahanSum(size_t n, T * input)
{
    T sum = T(0);
    T c   = T(0);                // A running compensation for lost low-order bits.
    for (auto i=0; i<n; ++i) {
        T y = input[i] - c;      // So far, so good: c is zero.
        T t = sum + y;           // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;       // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;                 // Algebraically, c should always be zero. Beware overly-aggressive optimising compilers!
    }                            // Next time around, the lost low part will be added to y in a fresh attempt.
    return sum;
}

/* Adapted from http://en.wikipedia.org/wiki/Kahan_summation_algorithm */
template <class T>
T KahanSumSIMD(size_t n, T * input)
{
    assert(n%4==0);

    T sum[4] = {0};
    T c[4]   = {0};
    _Pragma("clang loop vectorize(enable)")
    for (auto i=0; i<n; i+=4) {
        T y[4];
        T t[4];
        y[0] = input[i+0] - c[0];
        y[1] = input[i+1] - c[1];
        y[2] = input[i+2] - c[2];
        y[3] = input[i+3] - c[3];
        t[0] = sum[0] + y[0];
        t[1] = sum[1] + y[1];
        t[2] = sum[2] + y[2];
        t[3] = sum[3] + y[3];
        c[0] = (t[0] - sum[0]) - y[0];
        c[1] = (t[1] - sum[1]) - y[1];
        c[2] = (t[2] - sum[2]) - y[2];
        c[3] = (t[3] - sum[3]) - y[3];
        sum[0] = t[0];
        sum[1] = t[1];
        sum[2] = t[2];
        sum[3] = t[3];
    }
    return KahanSum(4,sum);
}

int main(int argc, char * argv[])
{
    size_t n = 8;

    printf("Testing KahanSum for n=%zu\n", n);

    auto * buf1 = new float[n];
    auto * buf2 = new float[n];

    for (auto i=0; i<n; i+=3) {
        buf1[i+0] = 1.0e7f;
        buf1[i+1] = 1.0f;
        buf1[i+2] = 1.0e-7f;
    }
    for (auto i=0; i<n; ++i)
        buf2[i] = buf1[n-i-1];

    std::cout.precision(1);
    //std::cout.setf(std::ios::fixed, std::ios::floatfield);
    for (auto i=0; i<n; ++i)
        std::cout << "buf1[" << i << "] = " << buf1[i] << " buf2[" << i << "] = " << buf2[i] << std::endl;

    auto b1 = BasicSum<float>(n,buf1);
    auto k1 = KahanSum<float>(n,buf1);
    auto s1 = KahanSumSIMD<float>(n,buf1);
    auto b2 = BasicSum<float>(n,buf2);
    auto k2 = KahanSum<float>(n,buf2);
    auto s2 = KahanSumSIMD<float>(n,buf2);

    delete[] buf2;
    delete[] buf1;

    std::cout.precision(14);
    std::cout << "BasicSum        result 1 = " << b1    << std::endl;
    std::cout << "KahanSum        result 1 = " << k1    << std::endl;
    std::cout << "KahanSumSIMD    result 1 = " << s1    << std::endl;
    std::cout << "difference             1 = " << b1-k1 << std::endl;
    std::cout << "BasicSum        result 2 = " << b2    << std::endl;
    std::cout << "KahanSum        result 2 = " << k2    << std::endl;
    std::cout << "KahanSumSIMD    result 2 = " << s2    << std::endl;
    std::cout << "difference             2 = " << b2-k2 << std::endl;

    return 0;
}
