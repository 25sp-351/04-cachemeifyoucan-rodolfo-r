#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cut_list.h"
#include "cache.h"
#include "piece_values.h"
#include "vec.h"

// Recursive function
static CutList* find_optimal_cutlist(Vec value_list, int rod_length, Cache* cache) {
    // Check cache first
    if (cache) {
        CutList* cached = cache_get(cache, rod_length);
        if (cached) {
            return cutlist_copy(cached);
        }
    }
    
    // Base case: rod length is 0
    if (rod_length == 0) {
        CutList* cl = malloc(sizeof(CutList));
        if (!cl) return NULL;
        
        cl->total_value = 0;
        cl->cuts = new_vec(sizeof(int));
        
        // Cache the result
        if (cache) {
            cache_put(cache, rod_length, cutlist_copy(cl));
        }
        
        return cl;
    }
    
    int best_value = 0;
    CutList* best_cuts = NULL;
    
    for (size_t i = 0; i < value_list->length; i++) {
        PieceLengthValue* piece = (PieceLengthValue*)((char*)value_list->base + i * value_list->element_size);
        
        if (piece->length <= rod_length) {
            // Make this cut and recursively solve for remaining rod
            CutList* remaining_cuts = find_optimal_cutlist(value_list, rod_length - piece->length, cache);
            
            if (remaining_cuts) {
                int total_value = piece->value + remaining_cuts->total_value;
                
                if (total_value > best_value) {
                    best_value = total_value;
                    
                    if (best_cuts) {
                        cutlist_free(best_cuts);
                    }
                    
                    // Create new best cut list
                    best_cuts = malloc(sizeof(CutList));
                    if (!best_cuts) {
                        cutlist_free(remaining_cuts);
                        return NULL;
                    }
                    
                    best_cuts->total_value = total_value;
                    best_cuts->cuts = new_vec(sizeof(int));
                    
                    // Add current cut
                    int current_cut = piece->length;
                    vec_add(best_cuts->cuts, &current_cut);
                    
                    // Add remaining cuts
                    for (size_t j = 0; j < remaining_cuts->cuts->length; j++) {
                        int* cut = (int*)((char*)remaining_cuts->cuts->base + j * remaining_cuts->cuts->element_size);
                        vec_add(best_cuts->cuts, cut);
                    }
                }
                
                cutlist_free(remaining_cuts);
            }
        }
    }
    
    if (!best_cuts) {
        best_cuts = malloc(sizeof(CutList));
        if (!best_cuts) return NULL;
        
        best_cuts->total_value = 0;
        best_cuts->cuts = new_vec(sizeof(int));
    }
    
    // Cache the result
    if (cache) {
        cache_put(cache, rod_length, cutlist_copy(best_cuts));
    }
    
    return best_cuts;
}

CutList* optimal_cutlist_for(Vec value_list, int rod_length, Cache* cache) {
    return find_optimal_cutlist(value_list, rod_length, cache);
}

void cutlist_print(CutList* cl) {
    if (!cl) {
        printf("No solution\n");
        return;
    }
    
    printf("Total value: %d\n", cl->total_value);
    printf("Cuts: ");
    
    for (size_t i = 0; i < cl->cuts->length; i++) {
        int* cut = (int*)((char*)cl->cuts->base + i * cl->cuts->element_size);
        printf("%d ", *cut);
    }
    
    printf("\n");
}

void cutlist_free(CutList* cl) {
    if (!cl) return;
    
    if (cl->cuts) {
        vec_free(cl->cuts);
    }
    
    free(cl);
}

CutList* cutlist_copy(CutList* cl) {
    if (!cl) return NULL;
    
    CutList* copy = malloc(sizeof(CutList));
    if (!copy) return NULL;
    
    copy->total_value = cl->total_value;
    copy->cuts = vec_copy(cl->cuts);
    
    return copy;
}