#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

float BasicSumFloat(int n, float * input)
{
    float sum = 0.0f;
    for (int i=0; i<n; ++i) {
        sum += input[i];
    }
    return sum;
}

/* Adapted from http://en.wikipedia.org/wiki/Kahan_summation_algorithm */

static inline float KahanSumFloat(int n, float * input)
{
    float sum = 0.0f;
    float c   = 0.0f;                // A running compensation for lost low-order bits.
    for (int i=0; i<n; ++i) {
        float y = input[i] - c;      // So far, so good: c is zero.
        float t = sum + y;           // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;           // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;                     // Algebraically, c should always be zero. Beware overly-aggressive optimising compilers!
    }                                // Next time around, the lost low part will be added to y in a fresh attempt.
    return sum;
}

static inline float KahanSumStridedFloat(int n, float * input, int stride)
{
    float sum = 0.0f;
    float c   = 0.0f;                // A running compensation for lost low-order bits.
    for (int i=0; i<n; i+=stride) {
        float y = input[i] - c;      // So far, so good: c is zero.
        float t = sum + y;           // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;           // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;                     // Algebraically, c should always be zero. Beware overly-aggressive optimising compilers!
    }                                // Next time around, the lost low part will be added to y in a fresh attempt.
    return sum;
}

static inline void KahanTransposeFloat(int commsize, int count, const float * restrict in, float * restrict out)
{
    for (int j=0; j<count; ++j) {
        for (int i=0; i<commsize; ++i) {
            out[i+commsize*j] = in[i*commsize+j];
        }
    }
}

static inline void KahanWrapperFloat(int commsize, int count, float * tempbuf, float * result)
{
        /* The Kahan algorithm reduces a contiguous vector to a scalar.
         * Since we do not get a contiguous vector from (all)gather
         * unless count=1, * we have to do one of the following:
         * (1) transpose the data, which is either hard in the case of in-place
         *     or uses twice the memory in the case of out-of-place, or
         * (2) form each contiguous vector as needed, or
         * (3) modify KahanSum to work with stride>1.
         * We currently do 2. */

#if 0
        float * tempbuf = malloc(commsize*sizeof(float));
        assert(tempbuf!=NULL);

        for (int j=0; j<count; ++j) {
            for (int i=0; i<commsize; ++i) {
                tempbuf[i] = tempbuf[i*commsize+j];
            }
            result[j] = KahanSumFloat(commsize, tempbuf);
            //result[j] = BasicSumFloat(commsize, tempbuf);
        }

        free(tempbuf);
#elif 0
        for (int j=0; j<count; ++j) {
            result[j] = KahanSumStridedFloat(commsize, tempbuf, count);
        }
        printf("hi\n");
#else
        float * transpose = malloc(commsize * count * sizeof(float));
        KahanTransposeFloat(commsize, count, tempbuf, transpose);
        for (int j=0; j<count; ++j) {
            result[j] = KahanSumFloat(commsize, &(transpose[j]) );
        }
        free(transpose);
#endif
        return;
}
