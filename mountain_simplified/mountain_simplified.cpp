#include <stdlib.h>
#include <chrono>
#include <iostream>

using namespace std::chrono;
using dataType = int;

#define MINBYTES (1 << 14)  /* First working set size */
#define MAXBYTES (1 << 27)  /* Last working set size */
#define MAXSTRIDE 80        /* Stride in bytes */
#define MAXELEMS MAXBYTES/sizeof(dataType)          /* Max number of elems */
#define MAXELEMSTRIDE MAXSTRIDE/sizeof(dataType)    /* Stride in elements */

dataType data[MAXELEMS];      /* The global array we'll be traversing */

void init_data(dataType *data, int n);
int test(int numElems, int stride);
double run(int size, int stride);

int main()
{
    uint64_t size;        /* Working set size (in bytes) */
    uint64_t stride;      /* Stride (in bytes) */
    double bandwidth; /* Average bandwidth (in MB/s) */

    /* Initialize each element in data */
    for (int i = 0; i < MAXELEMS; i++)
       data[i] = i % std::numeric_limits<dataType>::max();

    for (stride = 1; stride <= MAXELEMSTRIDE; stride++)
        printf("s%ld\t", stride);
    printf("\n");

    // for (size = (uint64_t)1<<19; size >= (uint64_t)1<<16; size -= 8192) {
    for (size = MAXBYTES; size >= MINBYTES; size >>= 1) {
        (size > (1 << 20))
            ? printf("%ldm\t", size / (1 << 20))
            : printf("%ldk\t", size / 1024);

        for (stride = 1; stride <= MAXELEMSTRIDE; stride++) {
            bandwidth = run(size, stride);
            std::cout << bandwidth << "\t";
            // printf("%.0ld\t", run(size, stride));
        }
        std::cout << std::endl;
        // printf("\n");
    }
    exit(0);
}

/* test - Iterate over first "elems" elements of array "data" with
 *        stride of "stride", using 4x4 loop unrolling.
 */
int test(int numElems, int stride) {
    dataType acc0 = 0;
    for (dataType i = 0; i < numElems; i += stride) {
        acc0 = acc0 + data[i];
    }
    return acc0;
}

/* run - Run test(elems, stride) and return read throughput (MB/s).
 *       "size" is in array elements, "stride" is in array elements
 */
double run(int size, int stride) {
    int repeat = 256;
    int arraySize = size / sizeof(dataType);    /* Size of array (bytes) */

    test(arraySize, stride);                    /* Warm up the cache */
    auto t0 = high_resolution_clock::now();
    for (int i = 0; i < repeat; ++i) {
        test(arraySize, stride);                /* Call test(elems,stride) */
    }
    auto t1 = high_resolution_clock::now();
    double time = duration_cast<nanoseconds>(t1 - t0).count() / 1000.0 / repeat;
    // printf("%f ", time);

    return (double)(size / stride) / (time);    /* Convert cycles to MB/s */
}

