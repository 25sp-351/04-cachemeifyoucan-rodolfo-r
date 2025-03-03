#ifndef CUT_LIST_H
#define CUT_LIST_H

#include "vec.h"

typedef struct Cache Cache;

typedef struct CutList {
    int total_value;
    Vec cuts; 
} CutList;

CutList* optimal_cutlist_for(Vec value_list, int rod_length, Cache* cache);

void cutlist_print(CutList* cl);

void cutlist_free(CutList* cl);

CutList* cutlist_copy(CutList* cl);

#endif 