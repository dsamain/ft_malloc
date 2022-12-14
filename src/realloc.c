#include "../malloc.h"

	
void *realloc(void *ptr, size_t size) {

    dbg("\n");
    dbg("realloc of size %d(%d) on %p\n", size, size + sizeof(t_block), ptr);

    pthread_mutex_lock(&g_mutex);

    if (!find_ptr(ptr)) {
        dbg("realloc: ptr %p not found\n", ptr);
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }

    if (ptr == NULL) {
        pthread_mutex_unlock(&g_mutex);
        return malloc(size);
    } else if (size == 0) {
        pthread_mutex_unlock(&g_mutex);
        free(ptr);
        return NULL;
    } 

    t_block *block = (t_block *)((char *)ptr - sizeof(t_block));

    if (block->size >= size + sizeof(t_block)) {
        pthread_mutex_unlock(&g_mutex);
        return ptr;
    } else {
        pthread_mutex_unlock(&g_mutex);
        void *new_ptr = malloc(size);
        if (new_ptr == NULL) {
            return NULL;
        }
        for (int i = 0; i < block->size - sizeof(t_block); i++) {
            ((char *)new_ptr)[i] = ((char *)ptr)[i];
        }
        free(ptr);
        return new_ptr;
    }
}