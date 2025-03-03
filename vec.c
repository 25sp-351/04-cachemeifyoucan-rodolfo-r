#include "vec.h"
#include <stdlib.h>
#include <string.h>
#define SIZE_INCREMENT 10;

Vec new_vec( size_t element_size ) {
    Vec v = malloc( sizeof( struct vec ) );

    if ( v == NULL ) {
        return NULL;
    }

    v->element_size = element_size;
    v->base = NULL;
    v->length = 0;
    v->allocated = 0;

    return v;
}

Vec vec_copy( Vec v ) {
    Vec new_v = malloc( sizeof ( struct vec ) );

    new_v->element_size = v->element_size;
    new_v->length = v->length;
    new_v->allocated = v->allocated;
    size_t mult = v->allocated * v->element_size;
    new_v->base = malloc( mult );
    memcpy(new_v->base, v->base, mult);

    return new_v;
}

Vec vec_free( Vec v ) {
    if( v != NULL && v->base != NULL ) {
        free( v->base );
        free( v );
        return NULL;
    } else {
        return NULL;
    }
}

size_t vec_length(Vec v) {
    return v->length;
}

void vec_add( Vec v, void* item ) {
    const char* message = "memory allocation failure";

    if( v == NULL ){    
    v->allocated = SIZE_INCREMENT;
    v->base = malloc(v->element_size * v->allocated);
    v->length = 0;
    }
    
    if( v->length == v->allocated ) {

        size_t new_allocated = v->allocated + SIZE_INCREMENT;
        void* new_base = realloc( v->base, new_allocated * v->element_size );

        if (new_base == NULL) {
            return message; // Handle memory allocation failure
        }
        v->base = new_base;
        v->allocated = new_allocated;
    }
    /*NOTES
    what is happening in this line?

    1. Pointer Arithmetic ( (char*)v->base + (v->length * v->element_size ) )

     - So when we multiply ( v->length * v->element_size ) we get the offset in bytes, this is the position to where the new element should be placed.

     - Since we cast the void* pointer to a char* pointer, this allows for byte-wise pointer arithmetic. 

     - Lastly, when add the offset to ( (char*)v->base )it gives the address to where the new element should be placed.item

     2. Copying the Element ( memcpy( destination, source, size ) )

     - memcpy is a library function that copies the size in bytes from the source we include and address to the destination address

     - destination: ( (char*)v->base + ( v->length * v->element_size ))
     this is the address where the new element should be copied

     - source: v->element_size
     is the number of bytes to copy, which is the size of one element

     Summary:
     it copies the new element from the item pointer to the correct position in the vector's memory, ensuring that the new element is added to the end of the vector. The position is calculated based on the current length of the vector and the size of each element.
    */
    memcpy((char*)v->base + (v->length * v->element_size), item, v->element_size); 
    //memcpy( destination, source, size )

    v->length++;
}

void* vec_items(Vec v){
    if( v == NULL ){ 
    v->allocated = SIZE_INCREMENT;
    v->base = malloc(v->element_size * v->allocated);
    v->length = 0;
    } else {
        return v->base;
    }
}
