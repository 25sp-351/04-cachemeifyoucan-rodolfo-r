#ifndef VEC_H
#define VEC_H
#include <stdlib.h>

typedef struct vec {
  void* base;
  size_t element_size;
  size_t allocated;
  size_t length;
} *Vec;

Vec new_vec(size_t element_size);
Vec vec_copy(Vec v);
Vec vec_free(Vec v);
Vec vec_elements(Vec v);
void vec_add(Vec c, void* item);
void* vec_items(Vec v);

#endif 
