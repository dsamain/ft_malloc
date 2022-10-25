#include "malloc.h"

int main() {
    char *ptr = (char *)malloc(10);
    printf("test\n");
    printf("ptr = %p\n", ptr);
}