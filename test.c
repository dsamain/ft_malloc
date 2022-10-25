#include "malloc.h"
#include <stdlib.h>
#include <time.h>

enum test {
    pouet = 42,
};

int main() {


    //char *p= malloc(144497);
    //int i = 0;
    //while (1) {
        //p[i] = 'a';
        //printf("%d\n", i);
        //i += 1024;
    //}

    //char *p = ft_malloc(131071);

    //return 0;


    clock_t start = clock();
    {
        int n = 10, m = 100;
        for (int i = 0; i < n; i++) {
            char *ptr = (char *)ft_malloc(rand () % m);
            //printf("malloc ret : %p\n", ptr);
            for (int j = 0; j <= m; j++) {
                ptr[j] = 'a';
            }
            if (rand() % 2) {
                free(ptr);
            }
            //printf("i: %d\n", i);
        }
        //char *ptr = (char *)ft_malloc(m * 2);
    }
    return 0;
    printf("ft time: %f\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    start = clock();
    {
        int n = 100000, m = 100;
        for (int i = 0; i < n; i++) {
            char *ptr = (char *)ft_malloc(m);
            //printf("malloc ret : %p\n", ptr);
            for (int j = 0; j < m; j++) {
                ptr[j] = 'a';
            }
            if (rand() % 2) {
                free(ptr);
            }
        }
        char *ptr = (char *)ft_malloc(m * 2);
    }
    printf("std time: %f\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}