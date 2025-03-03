#ifndef CACHE_H
#define CACHE_H

#include "cut_list.h"

typedef struct Cache Cache;

Cache* cache_init(int capacity);

CutList* cache_get(Cache* cache, int rod_length);


void cache_put(Cache* cache, int rod_length, CutList* solution);

void cache_free(Cache* cache);

void cache_print_stats(Cache* cache);

#endif /* CACHE_H */