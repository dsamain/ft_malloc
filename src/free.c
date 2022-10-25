#include "../malloc.h"

void free(void *ptr) {
    dbg("free of ptr %p(block %p)\n", ptr, (t_block *)((char *)ptr - sizeof(t_block)));
    ptr = (char *)ptr - sizeof(t_block);
    ((t_block *)ptr)->is_free = 1;
}