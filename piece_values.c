#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "piece_values.h"
#include "vec.h"

int compare_piece_values(const void* a, const void* b) {
    const PieceLengthValue* pa = (const PieceLengthValue*)a;
    const PieceLengthValue* pb = (const PieceLengthValue*)b;
    
    if (pa->value != pb->value) {
        return pb->value - pa->value;  // Descending by value
    }
    
    return pb->length - pa->length;  // Descending by length
}

Vec read_piece_values() {
    Vec v = new_vec(sizeof(PieceLengthValue));
    
    if (v == NULL) {
        return NULL;
    }
    
    PieceLength length;
    PieceValue value;
    
    while (scanf("%d %d", &length, &value) == 2) {
        PieceLengthValue wood;
        wood.length = length;
        wood.value = value;
        vec_add(v, &wood);
    }
    
    qsort(v->base, v->length, v->element_size, compare_piece_values);
    
    return v;
}

Vec read_piece_values_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }
    
    Vec v = new_vec(sizeof(PieceLengthValue));
    if (v == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for value list\n");
        fclose(file);
        return NULL;
    }
    
    PieceLength length;
    PieceValue value;
    
    while (fscanf(file, "%d %d", &length, &value) == 2) {
        if (length <= 0) {
            fprintf(stderr, "Error: Invalid length %d in input file\n", length);
            vec_free(v);
            fclose(file);
            return NULL;
        }
        
        PieceLengthValue wood;
        wood.length = length;
        wood.value = value;
        vec_add(v, &wood);
    }
    
    fclose(file);
    
    if (v->length == 0) {
        fprintf(stderr, "Error: No valid pieces found in input file\n");
        vec_free(v);
        return NULL;
    }
    
    qsort(v->base, v->length, v->element_size, compare_piece_values);
    
    return v;
}

void print_piece_values(Vec ptr) {
    if (ptr == NULL) {
        return;
    }
    
    printf("Piece values (%zu items):\n", ptr->length);
    
    for (size_t i = 0; i < ptr->length; i++) {
        PieceLengthValue* wood = (PieceLengthValue*)((char*)ptr->base + i * ptr->element_size);
        printf("  Length: %d, Value: %d\n", wood->length, wood->value);
    }
}

bool pvs_equal(PieceLengthValue *pv1, PieceLengthValue *pv2) {
    if (pv1 == NULL || pv2 == NULL) {
        return false;
    }
    return (pv1->length == pv2->length) && (pv1->value == pv2->value);
}