#include "vds.h"

vds_array vds_array_create( size_t element_size ) {
    vds_array a    = { 0 };
    a.element_size = element_size;
    return a;
}

void vds_array_destroy( vds_array *array ) { free( array->data ); }

void vds_array_push( vds_array *array, void *v ) {
    if ( array->size == array->capacity ) {
        array->capacity = array->capacity ? array->capacity * 2 : 1;
        array->data
            = realloc( array->data, array->capacity * array->element_size );
    }
    memcpy( array->data + array->size * array->element_size,
            v,
            array->element_size );
    array->size++;
}
