#ifndef __dynam_arr
#define __dynam_arr

#include <stddef.h> // For size_t

typedef struct DynamicArray {
  void *data;
  size_t size;
  size_t capacity;
  size_t element_size;
} DynamicArray;

DynamicArray *darray_create(size_t element_size, size_t initial_capacity);
void darray_destroy(DynamicArray *array);
int darray_push(DynamicArray *array, const void *element);
int darray_pop(DynamicArray *array, void *out_element);
int darray_get(const DynamicArray *array, size_t index, void *out_element);
int darray_set(DynamicArray *array, size_t index, const void *element);
size_t darray_size(const DynamicArray *array);
size_t darray_capacity(const DynamicArray *array);

#endif
