#include "../malloc.h"

int find_ptr(void *ptr) {

    for (int i = 0; i < 3; i++) {
        t_zone *cur_zone = g_zones[i];
        while (cur_zone) {
            if (ptr >= (void *)cur_zone && ptr < (void *)cur_zone + cur_zone->size) {
                t_block *cur_block = (t_block *)((void *)cur_zone + sizeof(t_zone));
                while (cur_block) {
                    if (ptr == (void *)cur_block + sizeof(t_block)) {
                        return 1;
                    }
                    cur_block = cur_block->next;
                }
            }
            cur_zone = cur_zone->next;
        }
    }
    return 0;
}