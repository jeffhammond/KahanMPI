#include <new>

template<typename I, typename T>
static inline
T BasicSum(I n, T * input)
{
    T sum = T(0);
    for (I i=0; i<n; ++i)
        sum += input[i];
    return sum;
}

/* Adapted from http://en.wikipedia.org/wiki/Kahan_summation_algorithm */

template<typename I, typename T>
static inline
T KahanSum<T>(I n, T * input)
{
    T sum = T(0);
    T c   = T(0);                // A running compensation for lost low-order bits.
    for (I i=0; i<n; ++i) {
        T y = input[i] - c;      // So far, so good: c is zero.
        T t = sum + y;           // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;       // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;                 // Algebraically, c should always be zero. Beware overly-aggressive optimising compilers!
    }                            // Next time around, the lost low part will be added to y in a fresh attempt.
    return sum;
}

template<typename I, typename T>
static inline
T KahanSumStrided(I n, T * input, I s /* stride */)
{
    T sum = T(0);
    T c   = T(0);                // A running compensation for lost low-order bits.
    for (I i=0; i<n; i+=s) {
        T y = input[i] - c;      // So far, so good: c is zero.
        T t = sum + y;           // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;       // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;                 // Algebraically, c should always be zero. Beware overly-aggressive optimising compilers!
    }                            // Next time around, the lost low part will be added to y in a fresh attempt.
    return sum;
}

template<typename I, typename T>
static inline
void KahanTWrapper(I commsize, I count, T * tempbuf, T * result)
{
        /* The Kahan algorithm reduces a contiguous vector to a scalar.
         * Since we do not get a contiguous vector from (all)gather
         * unless count=1, * we have to do one of the following:
         * (1) transpose the data, which is either hard in the case of in-place
         *     or uses twice the memory in the case of out-of-place, or
         * (2) form each contiguous vector as needed, or
         * (3) modify KahanSum to work with stride>1.
         * We currently do 2. */

        T * kahanbuf = new T[commsize];

        for (I j=0; j<count; ++j) {
            for (I i=0; i<commsize; ++i) {
                kahanbuf[i] = tempbuf[i*commsize+j];
            }
            result[j] = KahanSum<I,T>(commsize, kahanbuf);
        }

        delete[] kahanbuf;

        return;
}
