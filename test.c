#include "malloc.h"

enum test {
    pouet = 42,
};

int main() {

    int n = 2000, m = 100;
    for (int i = 0; i < n; i++) {
        char *ptr = (char *)ft_malloc(m);
        printf("malloc ret : %p\n", ptr);
        for (int j = 0; j < m; j++) {
            ptr[j] = 'a';
        }
        if (rand() % 2) {
            free(ptr);
        }
    }
    char *ptr = (char *)ft_malloc(m * 2);
}