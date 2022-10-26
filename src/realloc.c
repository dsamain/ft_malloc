#include "../malloc.h"

	
void *realloc(void *ptr, size_t size) {

    dbg("\n");
    dbg("realloc of size %d(%d) on %p\n", size, size + sizeof(t_block), ptr);

    pthread_mutex_lock(&g_mutex);
    if (ptr == NULL) {
        return malloc(size);
    } else if (size == 0) {
        free(ptr);
        return NULL;
    } 

    t_block *block = (t_block *)((char *)ptr - sizeof(t_block));

    if (block->size >= size + sizeof(t_block)) {
        pthread_mutex_unlock(&g_mutex);
        return ptr;
    } else {
        void *new_ptr = malloc(size);
        if (new_ptr == NULL) {
            pthread_mutex_unlock(&g_mutex);
            return NULL;
        }
        for (int i = 0; i < block->size - sizeof(t_block); i++) {
            ((char *)new_ptr)[i] = ((char *)ptr)[i];
        }
        free(ptr);
        return new_ptr;
    }
}