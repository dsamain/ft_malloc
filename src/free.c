#include "../malloc.h"

void free(void *ptr) {
    dbg("\n");
    dbg("free of ptr %p(block %p) size: %d\n", ptr, (t_block *)((char *)ptr - sizeof(t_block)), ((t_block *)((char *)ptr - sizeof(t_block)))->size);

    if (!ptr) {
        return;
    }

    pthread_mutex_lock(&g_mutex);

    char *p = (char *)ptr;
    p -= sizeof(t_block);
    ((t_block *)p)->is_free = 1;

    // defragmentation
    t_block *block = (t_block *)p;
    if (block->prev && block->prev->is_free) {
        block->prev->size += block->size;
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
        block = block->prev;
    }

    if (block->next && block->next->is_free) {
        block->size += block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }

    // if prev && next == NULL, then zone is empty -> unmap
    if (block->prev || block->next)
        pthread_mutex_unlock(&g_mutex);
        return;

    t_zone *zone = (t_zone *)((char *)block - sizeof(t_zone));

    // if zone is the only of TINY or SMALL, don't unmap
    if (zone->type != LARGE && !(zone->prev || zone->next)) {
        pthread_mutex_unlock(&g_mutex);
        return;
    }

    if (zone->prev) {
        zone->prev->next = zone->next;
    } else {
        g_zones[zone->type] = zone->next;
    }

    if (zone->next) {
        zone->next->prev = zone->prev;
    }

    dbg("unmap zone %p\n", zone);
    munmap(zone, zone->size);

    pthread_mutex_unlock(&g_mutex);


    //dbg("free of ptr %p(block %p) size: %d\n", ptr, (t_block *)((char *)ptr - sizeof(t_block)), ((t_block *)((char *)ptr - sizeof(t_block)))->size);
}