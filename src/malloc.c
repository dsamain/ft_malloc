
#include "../malloc.h"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
t_zone *g_zones[3] = {0, 0, 0}; // list of all zones

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
    if (type == LARGE) {
        size += size % getpagesize();
    }
    t_zone *ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);


    if (ret == MAP_FAILED) {
        return NULL;
    }
    ret->type = type;
    ret->next = NULL;
    ret->prev = NULL;
    ret->size = size;
    ret->free_size = size - sizeof(t_zone);
    ret->block_count = 0;

    dbg("create_zone %p of size %d (%s)\n", ret, size, type == TINY ? "TINY" : type == SMALL ? "SMALL" : "LARGE");
    return ret;
}

void push_zone(t_zone *zone) {
    enum e_zone_type type = zone->type;
    if (g_zones[type] == NULL) {
        g_zones[type] = zone;
    } else {
        zone->next = g_zones[type];
        g_zones[type]->prev = zone;
        g_zones[type] = zone;
    }
}

char *new_block(size_t block_size) {

    //dbg("malloc of size %d\n", block_size);
    dbg("zones : %p %p %p\n", g_zones[0], g_zones[1], g_zones[2]);

    enum e_zone_type type = get_zone_type(block_size);

    t_zone *zone = g_zones[type];
    while (zone != NULL) {
        if (zone->type == type && zone->free_size >= block_size) {
            break;
        }
        zone = zone->next;
    }

    if (!zone) {
        switch (type) {
            case TINY:
                zone = create_zone(TINY_ZONE_SIZE, TINY);
                break;
            case SMALL:
                zone = create_zone(SMALL_ZONE_SIZE, SMALL);
                break;
            case LARGE:
                zone = create_zone(block_size + sizeof(t_zone), LARGE);
                break;
        }
        if (!zone) {
            return NULL;
        }
        push_zone(zone);
    }

    t_block *ret;
    ret = (t_block *)(((char *)zone) + sizeof(t_zone));
    if (zone->block_count) {
        while (ret->next) {
            ret = (t_block *)(((char *)ret) + ret->size);
        }
        ret->next = (t_block *)(((char *)ret) + ret->size);
        ret->next->prev = ret;
        ret = ret->next;
    }  else {
        ret->prev = NULL;
    }
    ret->next = NULL;
    ret->size = block_size;
    ret->is_free = 0;

    zone->free_size -= block_size;
    zone->block_count++;

    dbg("new block: %p zone: %p(%s) size: %d offset: %d\n", ret, zone, type == TINY ? "TINY" : "SMALL", block_size, (char *)ret - (char *)zone);
    
    return (char *)ret;
}

char *get_block(size_t block_size) {
    t_zone *zone = g_zones[get_zone_type(block_size)];
    t_block *ret = NULL;
    while (zone != NULL) {
        if (zone->type == get_zone_type(block_size) && zone->block_count) {
            t_block *cur = (t_block *)((char *)zone + sizeof(t_zone));
            while (cur) {
                if (cur->size >= block_size && cur->is_free) {
                    cur->is_free = 0;
                    return (char *)cur;
                }
                cur = cur->next;
            }
        }
        zone = zone->next;
    }
    return NULL;
}


void *malloc(size_t size) {
    dbg("\n");
    dbg("malloc of size %d(%d)\n", size, size + sizeof(t_block));

    char test[100];
    test[0] = pthread_mutex_lock(&g_mutex) + '0';
    write(1, test, 1);

    char *ret;

    if (size) {
       ret = get_block(size + sizeof(t_block));

        if (!ret)
            ret = new_block(size + sizeof(t_block));
    }

    pthread_mutex_unlock(&g_mutex);
    return (ret ? (char *)ret + sizeof(t_block) : NULL);
}