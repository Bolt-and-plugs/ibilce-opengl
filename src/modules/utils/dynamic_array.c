#include "dynamic_array.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static int _darray_resize(DynamicArray *array) {
  assert(array != NULL);
  size_t new_capacity = (array->capacity == 0) ? 8 : array->capacity * 2;
  void *new_data = realloc(array->data, new_capacity * array->element_size);
  if (new_data == NULL) {
    return -1;
  }

  array->data = new_data;
  array->capacity = new_capacity;
  return 0;
}

DynamicArray *darray_create(size_t element_size, size_t initial_capacity) {
  if (element_size == 0) {
    return NULL;
  }
  DynamicArray *array = (DynamicArray *)malloc(sizeof(DynamicArray));
  if (array == NULL) {
    return NULL;
  }
  array->size = 0;
  array->element_size = element_size;
  array->capacity = (initial_capacity > 0) ? initial_capacity : 8;
  array->data = malloc(array->capacity * array->element_size);
  if (array->data == NULL) {
    free(array);
    return NULL;
  }

  return array;
}

void darray_destroy(DynamicArray *array) {
  if (array != NULL) {
    free(array->data);
    free(array);
  }
}

int darray_push(DynamicArray *array, const void *element) {
  assert(array != NULL && element != NULL);

  if (array->size >= array->capacity)
    if (_darray_resize(array) != 0)
      return -1;

  void *dest = (char *)array->data + (array->size * array->element_size);
  memcpy(dest, element, array->element_size);
  array->size++;
  return 0;
}

int darray_pop(DynamicArray *array, void *out_element) {
  assert(array != NULL);

  if (array->size == 0)
    return -1;

  array->size--;

  if (out_element != NULL) {
    void *src = (char *)array->data + (array->size * array->element_size);
    memcpy(out_element, src, array->element_size);
  }

  return 0;
}

int darray_get(const DynamicArray *array, size_t index, void *out_element) {
  assert(array != NULL && out_element != NULL);

  if (index >= array->size)
    return -1;

  const void *src = (const char *)array->data + (index * array->element_size);
  memcpy(out_element, src, array->element_size);

  return 0;
}

int darray_set(DynamicArray *array, size_t index, const void *element) {
  assert(array != NULL && element != NULL);

  if (index >= array->size)
    return -1;

  void *dest = (char *)array->data + (index * array->element_size);
  memcpy(dest, element, array->element_size);

  return 0;
}

size_t darray_size(const DynamicArray *array) {
  assert(array != NULL);
  return array->size;
}

size_t darray_capacity(const DynamicArray *array) {
  assert(array != NULL);
  return array->capacity;
}
