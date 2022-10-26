
#include <time.h>
#include <string.h>

#ifdef STD
    #include <stdlib.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <sys/mman.h>
#else
    #include "malloc.h"
#endif

enum test {
    pouet = 42,
};

int main() {


    void *ptr = calloc(123, 2);
    for (int i= 0 ; i < 42 && ptr; i++) {
        printf("%d", ((char *)ptr)[i]);
    }
    show_alloc_mem();

    //clock_t start = clock();
    //{
        //int n = 10, m = 42;
        //for (int i = 0; i < n; i++) {
            //int sz = rand() % m + 1, tmp;
            //char *ptr = (char *)malloc(sz);
            //if (rand()% 3) {
                //sz = sz + ((tmp = (rand() % m)) - tmp / 2) / 3;
                ////printf("sz : %d\n", sz);
                //ptr = realloc(ptr, sz);
            //}
            //printf("malloc ret : %p\n", ptr);
            //for (int j = 0; j < sz; j++) {
                //ptr[j] = 'a';
            //}
            //if (rand() % 2) {
                //free(ptr);
            //}
        //}
    //}
}