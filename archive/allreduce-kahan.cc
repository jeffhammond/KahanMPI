#ifdef DEBUG
#  include <cstdio>
#endif

#include <cstdlib>  /* declares NULL    */
#include <cmath>    /* declares ldexp() */
#include <new>      /* declares new[]   */
#include <mpi.h>

#define MPIX_KAHAN_SUM (MPI_Op)0x99999999

template <class T>
T BasicSum(int n, T * input) 
{
    T sum = T(0);
    for (int i=0; i<n; ++i) 
        sum += input[i];
    return sum;
}

/* Adapted from http://en.wikipedia.org/wiki/Kahan_summation_algorithm */
template <class T>
T KahanSum(int n, T * input) 
{
    T sum = T(0);
    T c   = T(0);                // A running compensation for lost low-order bits.
    for (int i=0; i<n; ++i) {
        T y = input[i] - c;      // So far, so good: c is zero.
        T t = sum + y;           // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;       // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;                 // Algebraically, c should always be zero. Beware overly-aggressive optimising compilers!
    }                            // Next time around, the lost low part will be added to y in a fresh attempt.
    return sum;
}

template <class T>
T KahanSumStrided(int n, T * input, int s) 
{
    T sum = T(0);
    T c   = T(0);                // A running compensation for lost low-order bits.
    for (int i=0; i<n; i+=s) {
        T y = input[i] - c;      // So far, so good: c is zero.
        T t = sum + y;           // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;       // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;                 // Algebraically, c should always be zero. Beware overly-aggressive optimising compilers!
    }                            // Next time around, the lost low part will be added to y in a fresh attempt.
    return sum;
}

#ifdef TEST_KAHAN_SUM
int main(int argc, char * argv[])
{
  int provided, rank;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank==0) 
  {
    int n = (argc>1) ? atoi(argv[1]) : 100;
    if (n<100) n*=100;

    printf("Testing KahanSum for n=%d\n", n);

    double * buf1 = new double[n];
    double * buf2 = new double[n];

    for (int i=0; i<n; ++i)
        buf1[i] = ldexp(1.0, i%64 - 32);
    for (int i=0; i<n; ++i)
        buf2[i] = buf1[n-i-1];
#ifdef DEBUG
    for (int i=0; i<n; ++i)
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
  }

  MPI_Finalize();

  return 0;
}
#endif

template <class T>
void KahanWrapper(int commsize, int count, T * tempbuf, T * result) 
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

        for (int j=0; j<count; ++j) {
            for (int i=0; i<commsize; ++i) {
                kahanbuf[i] = tempbuf[i*commsize+j];
            }
            result[j] = KahanSum<T>(commsize, kahanbuf);
        }

        delete[] kahanbuf;

        return;
}

#ifdef TEST_KAHAN_WRAPPER
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
#endif

#ifdef __cplusplus
extern "C" {
#endif

int My_Allreduce_kahan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm ) 
{
    int rc = MPI_SUCCESS;

#ifdef DEBUG
    fprintf(stderr, "Using deterministic MPI_Allreduce \n");
#endif

    int commrank = 0;
    rc = MPI_Comm_rank(comm, &commrank);
    if (rc!=MPI_SUCCESS) return rc;

    int commsize = 0;
    rc = MPI_Comm_size(comm, &commsize);
    if (rc!=MPI_SUCCESS) return rc;

    int typesize = 0;
    rc = MPI_Type_size(datatype, &typesize);
    if (rc!=MPI_SUCCESS) return rc;

    void * tempbuf = NULL;
    rc = MPI_Alloc_mem( (MPI_Aint)(commsize*count*typesize), MPI_INFO_NULL, &tempbuf);
    if (rc!=MPI_SUCCESS) return rc;

#ifdef USE_LOG_N_ALGORITHM
    int root = 0;
    rc = MPI_Gather(sendbuf, count, datatype, tempbuf, count, datatype, root, comm);
#else
    rc = MPI_Allgather(sendbuf, count, datatype, tempbuf, count, datatype, comm);
#endif
    if (rc!=MPI_SUCCESS) return rc;

#ifdef USE_LOG_N_ALGORITHM
    if (commrank==root)
#endif
    {
        /* reduce tempbuf into recvbuf */
#if defined(USE_REDUCE_LOCAL)
        for (int i=0; i<commsize; ++i) {
            rc = MPI_Reduce_local( &(tempbuf[i*count]), recvbuf, count, datatype, op);
            if (rc!=MPI_SUCCESS) return rc;
        }
#elif defined(USE_KAHAN_SUM)
        if (datatype==MPI_DOUBLE)
            //KahanWrapper<double>(commsize, count, (double*)tempbuf, (double*)recvbuf);
            KahanWrapper<double>(commsize, count, static_cast<double*>(tempbuf), static_cast<double*>(recvbuf) );
        else {
            MPI_Abort(MPI_COMM_WORLD, 1);
            printf("KahanWrapper not instantiated for this type! \n");
        }
#else
#  error You need to define either USE_REDUCE_LOCAL or USE_KAHAN_SUM!
#endif
    }

#ifdef USE_KAHAN_SUM
#endif

#ifdef USE_LOG_N_ALGORITHM
    rc = MPI_Bcast(recvbuf, count, datatype, root, comm);
    if (rc!=MPI_SUCCESS) return rc;
#endif

    return MPI_SUCCESS;
}

/* MPI 3.0 added const to input i.e. read-only arguments... */
#if MPI_VERSION >= 3
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm ) 
#else
int MPI_Allreduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm ) 
#endif
{
    int rc = MPI_SUCCESS;

    if (op==MPIX_KAHAN_SUM)
        rc = My_Allreduce_kahan(sendbuf, recvbuf, count, datatype, op, comm); 
    else
        rc = PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
    if (rc!=MPI_SUCCESS) return rc;

    return MPI_SUCCESS;
}

#ifdef TEST_KAHAN_ALLREDUCE
int main(int argc, char * argv[])
{

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif
