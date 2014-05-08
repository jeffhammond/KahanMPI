#include <cstdio>   /* declares printf  */
#include <cstdlib>  /* declares NULL    */
#include <cmath>    /* declares ldexp() */
#include <new>      /* declares new[]   */

#include "kahan.h"

int main(int argc, char * argv[])
{
    size_t n = (argc>1) ? atol(argv[1]) : 100;
    if (n<100) n*=100;

    printf("Testing KahanSum for n=%zu\n", n);

    double * buf1 = new double[n];
    double * buf2 = new double[n];

    for (size_t i=0; i<n; ++i)
        buf1[i] = ldexp(1.0, i%64 - 32);
    for (size_t i=0; i<n; ++i)
        buf2[i] = buf1[n-i-1];
#ifdef DEBUG
    for (size_t i=0; i<n; ++i)
        printf("buf1[%d] = %40.20lf buf2[%d] = %40.20lf \n", i, buf1[i], i, buf2[i] );
#endif

    double b1 = BasicSum<double>(n,buf1);
    double k1 = KahanSum<double>(n,buf1);
    double s1 = KahanSumStrided<double>(n,buf1,1);
    double b2 = BasicSum<double>(n,buf2);
    double k2 = KahanSum<double>(n,buf2);
    double s2 = KahanSumStrided<double>(n,buf2,1);

    delete[] buf2;
    delete[] buf1;

    printf("BasicSum        result 1 = %40.20lf \n", b1);
    printf("KahanSum        result 1 = %40.20lf \n", k1);
    printf("KahanSumStrided result 1 = %40.20lf \n", s1);
    printf("difference             1 = %40.20lf \n", b1-k1);
    printf("BasicSum        result 2 = %40.20lf \n", b2);
    printf("KahanSum        result 2 = %40.20lf \n", k2);
    printf("KahanSumStrided result 2 = %40.20lf \n", s2);
    printf("difference             2 = %40.20lf \n", b2-k2);
    fflush(stdout);

    return 0;
}
