#ifdef DEBUG
#  include <cstdio>
#endif

#include <cstdlib>  /* declares NULL    */
#include <cmath>    /* declares ldexp() */
#include <new>      /* declares new[]   */
#include <mpi.h>

#define MPIX_KAHAN_SUM (MPI_Op)0x99999999

int main(int argc, char * argv[])
{
    int count    = 10;
    int commsize = 128;

    printf("Testing KahanWrapper for count=%d, commsize=%d\n", count, commsize);

    double * buf1 = new double[count*commsize];
    double * buf2 = new double[commsize];
    double * res1 = new double[count];
    double * res2 = new double[count];

    for (int i=0; i<(count*commsize); ++i)
        buf1[i] = ldexp(1.0, i%64 - 32);

    KahanWrapper<double>(commsize, count, buf1, res1);

    for (int j=0; j<count; ++j) {
        for (int i=0; i<commsize; ++i) {
            buf2[i] = buf1[i*commsize+j];
        }
        res2[j] = KahanSum<double>(commsize, buf2);
        printf("res1[%d] = %40.20lf res2[%d] = %40.20lf diff = %40.20lf\n", j, res1[j], j, res2[j], res1[j]-res2[j] );
    }
            
    fflush(stdout);

    delete[] res2;
    delete[] res1;
    delete[] buf2;
    delete[] buf1;

    return 0;
}
