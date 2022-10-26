#include "../malloc.h"

void *calloc(size_t nmemb, size_t size) {

    pthread_mutex_lock(&g_mutex);
    // prevent overflow
    if (UINT_MAX / nmemb < size) {
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }

    size_t total_size = nmemb * size;

    void *ptr = malloc(total_size);
    for (int i = 0; ptr && i < total_size; i++) {
        ((char *)ptr)[i] = 0;
    }
    pthread_mutex_unlock(&g_mutex);
    return ptr;
}