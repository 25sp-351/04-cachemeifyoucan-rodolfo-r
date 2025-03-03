#include <stdlib.h>
#include <stdio.h>
#include "cache.h"
#include "cut_list.h"

typedef struct CacheEntry {
    int rod_length;
    CutList* solution;
} CacheEntry;

// LIFO Cache implementation
struct Cache {
    CacheEntry* entries;
    int capacity;
    int size;
    int hits;
    int misses;
    int evictions;
};

Cache* cache_init(int capacity) {
    Cache* cache = malloc(sizeof(Cache));
    if (!cache) return NULL;

    cache->entries = malloc(sizeof(CacheEntry) * capacity);
    if (!cache->entries) {
        free(cache);
        return NULL;
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->hits = 0;
    cache->misses = 0;
    cache->evictions = 0;
    
    return cache;
}

CutList* cache_get(Cache* cache, int rod_length) {
    for (int i = 0; i < cache->size; i++) {
        if (cache->entries[i].rod_length == rod_length) {
            cache->hits++;
            return cache->entries[i].solution;
        }
    }
    
    cache->misses++;
    return NULL;
}

void cache_put(Cache* cache, int rod_length, CutList* solution) {
    for (int i = 0; i < cache->size; i++) {
        if (cache->entries[i].rod_length == rod_length) {
            cutlist_free(cache->entries[i].solution);
            cache->entries[i].solution = solution;
            return;
        }
    }
    
    if (cache->size == cache->capacity) {
        cutlist_free(cache->entries[cache->size - 1].solution);
        cache->evictions++;
        
        cache->entries[cache->size - 1].rod_length = rod_length;
        cache->entries[cache->size - 1].solution = solution;
    } else {
        cache->entries[cache->size].rod_length = rod_length;
        cache->entries[cache->size].solution = solution;
        cache->size++;
    }
}

void cache_free(Cache* cache) {
    if (!cache) return;
    
    for (int i = 0; i < cache->size; i++) {
        cutlist_free(cache->entries[i].solution);
    }
    
    free(cache->entries);
    free(cache);
}

void cache_print_stats(Cache* cache) {
    printf("Cache Statistics (LIFO Policy):\n");
    printf("  Size: %d/%d\n", cache->size, cache->capacity);
    printf("  Hits: %d\n", cache->hits);
    printf("  Misses: %d\n", cache->misses);
    printf("  Evictions: %d\n", cache->evictions);
    printf("  Hit ratio: %.2f%%\n", (cache->hits > 0 || cache->misses > 0) ? ((float)cache->hits / (cache->hits + cache->misses) * 100) : 0);
}