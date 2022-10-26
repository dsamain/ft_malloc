#include "../malloc.h"

void put_base(unsigned long long n, size_t b, short f) {
    write(1, "0x", 2 * f);
    if (n >= b)
        put_base(n / b, b, 0);
    write(1, &"0123456789abcdef"[n % b], 1);
}

int count_zone() {
    int ret = 0;
    for (int i = 0; i < 3; i++) {
        t_zone *zone = g_zones[i];
        while (zone != NULL) {
            ret++;
            zone = zone->next;
        }
    }
    return ret;
}

void bubble_sort_yes(t_zone **zones, int size) {
    for (int _ = 0; _ < size - 1; _++) {
        for (int j = 0; j < size - 1; j++) {
            if (zones[j] > zones[j + 1]) {
                t_zone *tmp = zones[j];
                zones[j] = zones[j + 1];
                zones[j + 1] = tmp;
            }
        }
    }
}

void show_alloc_mem() {
    pthread_mutex_lock(&g_mutex);
    char *zone_type[] = {"TINY", "SMALL", "LARGE"};

    unsigned long long total = 0;
    t_zone *zones[count_zone()];

    int cnt = 0;
    for (int i = 0; i < 3; i++) {
        t_zone *zone = g_zones[i];
        while (zone != NULL) {
            zones[cnt++] = zone;
            zone = zone->next;
        }
    }

    bubble_sort_yes(zones, cnt);

    t_zone *cur_zone = (cnt ? zones[0] : NULL);
    for (int i = 0; i < cnt; i++) {
        write(1, zone_type[zones[i]->type], 5);
        write(1, " : ", 3);
        put_base((unsigned long long)zones[i], 16, 1);
        write(1, "\n", 1);
        t_block *cur_block = (t_block *)((char *)zones[i] + sizeof(t_zone));
        while (cur_block && cur_zone->block_count) {
            put_base((unsigned long long)cur_block, 16, 1);
            write(1, " - ", 3);
            put_base((unsigned long long)((char *)cur_block + cur_block->size - 1), 16, 1);
            write(1, " : ", 3);
            put_base(cur_block->size, 10, 0);
            write(1, " bytes\n", 7);
            total += cur_block->size;
            cur_block = cur_block->next;
        }
    }
    write(1, "Total : ", 8);
    put_base(total, 10, 0);
    write(1, " bytes\n", 7);
    pthread_mutex_unlock(&g_mutex);
}

void show_alloc_mem_ex() {
    pthread_mutex_lock(&g_mutex);
    char *zone_type[] = {"TINY", "SMALL", "LARGE"};

    unsigned long long total = 0;
    t_zone *zones[count_zone()];

    int cnt = 0;
    for (int i = 0; i < 3; i++) {
        t_zone *zone = g_zones[i];
        while (zone != NULL) {
            zones[cnt++] = zone;
            zone = zone->next;
        }
    }

    bubble_sort_yes(zones, cnt);

    t_zone *cur_zone = (cnt ? zones[0] : NULL);
    for (int i = 0; i < cnt; i++) {
        t_block *cur_block = (t_block *)((char *)zones[i] + sizeof(t_zone));
        while (cur_block && cur_zone->block_count) {
            put_base((unsigned long long)cur_block, 16, 1);
            write(1, " - ", 3);
            put_base((unsigned long long)((char *)cur_block + cur_block->size - 1), 16, 1);
            write(1, "(", 1);
            write(1, zone_type[zones[i]->type], 5);
            write(1, ") : \n", 5);
            for (int i = 0; i < cur_block->size; i++) {
                if (i % 16 == 0 && i)
                    write(1, "\n", 1);
                put_base(*((unsigned char *)cur_block + sizeof(t_block) + i), 16, 1);
                write(1, " ", 1);
            }
            cur_block = cur_block->next;
            write(1, "\n\n", 2 - (cur_block == NULL && i == cnt - 1));
        }
    }
    pthread_mutex_unlock(&g_mutex);
}