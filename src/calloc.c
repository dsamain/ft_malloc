#include "../malloc.h"

void *calloc(size_t nmemb, size_t size) {

    pthread_mutex_lock(&g_mutex);
    // prevent overflow
    if (nmemb && UINT_MAX / nmemb < size) {
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }

    size_t total_size = nmemb * size;

    pthread_mutex_unlock(&g_mutex);

    void *ptr = malloc(total_size);
    for (int i = 0; ptr && i < total_size; i++)
        ((char *)ptr)[i] = 0;

    return ptr;
}
