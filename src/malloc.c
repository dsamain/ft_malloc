#include "../malloc.h"

t_zone *g_zones = NULL; // list of all zones


enum e_zone_type get_zone_type(size_t size) {
    if (size <= TINY_BLOCK_SIZE) {
        return TINY;
    } else if (size <= SMALL_BLOCK_SIZE) {
        return SMALL;
    } else {
        return LARGE;
    }
}

t_zone *create_zone(size_t size, enum e_zone_type type) {
    t_zone *ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (ret == MAP_FAILED) {
        return NULL;
    }
    ret->type = type;
    ret->next = NULL;
    ret->prev = NULL;
    ret->size = size;
    ret->free_size = size - sizeof(t_zone);
    ret-> block_count = 0;
    return ret;
}

void push_zone(t_zone *zone) {
    if (g_zones == NULL) {
        g_zones = zone;
    } else {
        zone->next = g_zones;
        g_zones->prev = zone;
        g_zones = zone;
    }
}

char *new_block(size_t block_size) {

    enum e_zone_type type = get_zone_type(block_size);

    t_zone *zone = g_zones;
    while (zone != NULL) {
        if (zone->type == type && zone->free_size >= block_size) {
            break;
        }
        zone = zone->next;
    }

    if (!zone) {
        zone = create_zone(type == TINY ? TINY_ZONE_SIZE : SMALL_ZONE_SIZE, type);
        if (!zone) {
            return NULL;
        }
        push_zone(zone);
    }

    t_block *ret;
    ret = (t_block *)((char *)zone + sizeof(t_zone));
    if (zone->block_count) {
        while (ret->next) {
            ret = ret->next;
        }
        ret->next = (t_block *)((char *)ret + ret->size);
        ret->next->prev = ret;
        ret = (t_block *)(char *)ret + ret->size;
    }  else {
        ret->prev = NULL;
    }
    ret->next = NULL;
    ret->size = block_size;

    zone->free_size -= block_size;
    zone->block_count++;
    
    return (char *)ret;
}

t_block *get_block(size_t size) {

    // find heap -> when found 
    // if no heap 
}


void *ft_malloc(size_t size) {
    // find a block in the list of zones
    // if no block is found, create a new block
    // if no heap have enough space create a new heap
    // if no zone is found, create a new zone

    //return new_block(size + sizeof(t_block)) + sizeof(t_block);
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
}