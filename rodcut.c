#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdnoreturn.h>

#include "piece_values.h"
#include "cut_list.h"
#include "cache.h"
#include "vec.h"

#define CACHE_SIZE 50  

void usage(char *program_name);

int main(int argc, char *argv[]) {
    if (argc != 2 || !strcmp(argv[1], "-h")) {
        usage(argv[0]);
    }

    Vec value_list = read_piece_values_from_file(argv[1]);
    if (value_list == NULL) {
        fprintf(stderr, "Error reading value list from file.\n");
        return 1;
    }

    
    Cache *cache = cache_init(CACHE_SIZE);
    if (!cache) {
        fprintf(stderr, "Error initializing cache.\n");
        vec_free(value_list);
        return 1;
    }

    int rod_length;
    char line[256];

    printf("Enter rod lengths (one per line):\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) {
            break;
        }
        
        if (sscanf(line, "%d", &rod_length) != 1 || rod_length < 1) {
            fprintf(stderr, "Invalid rod length. Please enter a positive integer.\n");
            continue;
        }
        
        printf("Processing rod length: %d\n", rod_length);
        
        CutList *cl = optimal_cutlist_for(value_list, rod_length, cache);
        
        cutlist_print(cl);
        
        cutlist_free(cl);
        
        printf("\n");
    }
    
    cache_print_stats(cache);
    
    cache_free(cache);
    vec_free(value_list);
    
    return 0;
}

noreturn void usage(char *program_name) {
    fprintf(stderr,
            "Usage:\n"
            "%s [-h] values_file\n"
            "values_file = file containing (length,value) pairs, one pair per line\n",
            program_name);
    exit(1);
}