#include "../malloc.h"

void *reallocarray(void *ptr, size_t nmemb, size_t size) {

    pthread_mutex_lock(&g_mutex);

    if (!find_ptr(ptr)) {
        dbg("reallocarray: ptr %p not found\n", ptr);
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }

    if (nmemb && UINT_MAX / nmemb < size) {
        errno = ENOMEM;
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }
    pthread_mutex_unlock(&g_mutex);
    char *ret = realloc(ptr, nmemb * size);
    for (int i = 0; ret && i < nmemb * size; i++) {
        ret[i] = 0;
    }
    return ret;
}
