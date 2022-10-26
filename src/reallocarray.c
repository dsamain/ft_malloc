#include "../malloc.h"

void *reallocarray(void *ptr, size_t nmemb, size_t size) {

    pthread_mutex_lock(&g_mutex);
    if (UINT_MAX / nmemb < size) {
        errno = ENOMEM;
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }
    char *ret = realloc(ptr, nmemb * size);
    for (int i = 0; ret && i < nmemb * size; i++) {
        ret[i] = 0;
    }
    pthread_mutex_unlock(&g_mutex);
    return ret;
}