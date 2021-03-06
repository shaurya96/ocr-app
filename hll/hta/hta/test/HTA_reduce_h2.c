#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Debug.h"
#include "HTA.h"
#include "HTA_operations.h"
#include "test.h"
#define MATRIX_WIDTH (3*5*4)
#define MATRIX_SIZE (MATRIX_WIDTH*MATRIX_WIDTH)

void SUMSQUARE(HTA* h1, HTA* dummy, void* s)
{
    int32_t sum = *(int32_t*)s;
    Tuple iter[h1->height - 1];
    Tuple_iterator_begin(h1->dim, h1->height-1, iter);
    do {
        HTA* tile = HTA_iterator_to_hta(h1, iter);
        int32_t* p = (int32_t*) HTA_get_ptr_raw_data(tile);
        int num_elem = tile->leaf.num_elem;
        for(int i = 0; i < num_elem; i++)
        {
            int32_t x = p[i];
            sum += x * x;
        }
    } while(Tuple_iterator_next(&h1->tiling, iter));

    *((int32_t*)s) = sum;
}

int hta_main(int argc, char** argv, int pid)
{
    int32_t M[MATRIX_SIZE];
    int i, err;
    int32_t result = 0;
    int32_t check = 0;

    Tuple t0 = Tuple_create(2, 3, 3);
    Tuple t1 = Tuple_create(2, 5, 5);
    Tuple flat_size = Tuple_create(2, MATRIX_WIDTH, MATRIX_WIDTH);

    Tuple mesh = HTA_get_vp_mesh(2);
    Tuple_print(&mesh);

    Dist dist;
    Dist_init(&dist, DIST_BLOCK, &mesh);
    // create an empty shell
    HTA *h = HTA_create_with_pid(pid, 2, 3, &flat_size, 0, &dist, HTA_SCALAR_TYPE_INT32,
            2, t0, t1);

    srand(time(NULL));
    // create a 2D matrix
    for(i = 0; i < MATRIX_SIZE; i++) {
        M[i] = i%100;
    }

    HTA_init_with_array(h, M);
    HTA_reduce_h2(REDUCE_SUM, SUMSQUARE, &result, h, h);

    // dump HTA data
    // HTA_to_array_int32_t(h, M);

    for(i = 0; i < MATRIX_SIZE; i++) {
        check += M[i]*M[i];
    }

    printf("Check result\n");
    printf("result = %d\n", result);
    printf("check = %d\n", check);
    if(result == check) {
        printf("** result matches! **\n");
        err = SUCCESS;
    }
    else {
        printf("** result does not match! **\n");
        err = ERR_UNMATCH;
    }

    HTA_destroy(h);

    exit(err);
    return 0;
}
