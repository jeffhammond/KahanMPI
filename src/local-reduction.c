float BasicSum(int n, float * input)
{
    float sum = 0.0f;
    for (int i=0; i<n; ++i) {
        sum += input[i];
    }
    return sum;
}

/* Adapted from http://en.wikipedia.org/wiki/Kahan_summation_algorithm */

float KahanSum(int n, float * input)
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

float KahanSumStrided(int n, float * input, int stride)
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

void KahanWrapper(int commsize, int count, float * tempbuf, float * result)
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
        float * kahanbuf = malloc(commsize*sizeof(float));
        assert(kahanbuf!=NULL);

        for (int j=0; j<count; ++j) {
            for (int i=0; i<commsize; ++i) {
                kahanbuf[i] = tempbuf[i*commsize+j];
            }
            result[j] = KahanSum(commsize, kahanbuf);
            //result[j] = BasicSum(commsize, kahanbuf);
        }

        free(kahanbuf);
#else
        for (int j=0; j<count; ++j) {
            result[j] = KahanSumStrided(commsize, kahanbuf, count);
        }
        printf("hi\n");
#endif
        return;
}
