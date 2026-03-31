#include "datastructure.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int sparse_set_init(struct sparse_set *set, size_t capacity) {
  set->sparse = malloc(sizeof(size_t) * capacity);
  set->dense = malloc(sizeof(size_t) * capacity);

  for (size_t i = 0; i < capacity; i++) {
    set->sparse[i] = SIZE_MAX;
  }

  set->count = 0;
  set->capacity = capacity;

  return 0;
}

struct sparse_set *sparse_set_new(size_t capacity) {
  struct sparse_set *set = (struct sparse_set *)malloc(sizeof(*set));
  sparse_set_init(set, capacity);
  return set;
}

int sparse_set_push(struct sparse_set *set, size_t entity) {
  if (set->sparse[entity] != SIZE_MAX) {
    return -1;
  }

  set->sparse[entity] = set->count;
  set->dense[set->count++] = entity;

  return 0;
}

int sparse_set_remove(struct sparse_set *set, size_t entity) {
  size_t position = set->sparse[entity];
  if (position == SIZE_MAX) {
    return -1;
  }

  set->sparse[entity] = SIZE_MAX;
  if (position == --set->count) {
    return 0;
  }

  size_t last_entity = set->dense[set->count];
  set->sparse[last_entity] = position;
  set->dense[position] = last_entity;

  return 0;
}

void sparse_set_free(struct sparse_set *set) {
  free(set->sparse);
  free(set->dense);
  free(set);
}

void sparse_set_print(struct sparse_set set) {
  printf("idx\t");
  for (size_t i = 0; i < set.capacity; i++) {
    printf("%zu ", i);
  }

  printf("\nsparse\t");
  for (size_t j = 0; j < set.capacity; j++) {
    if (set.sparse[j] != SIZE_MAX)
      printf("%zu ", set.sparse[j]);
    else
      printf("  ");
  }

  printf("\ndense\t");
  for (size_t k = 0; k < set.count; k++) {
    printf("%zu ", set.dense[k]);
  }

  printf("\n");
}

struct list *list_new(size_t capacity) {
  struct list *l = malloc(sizeof(*l));
  list_init(l, capacity);
  return l;
}

void list_init(struct list *l, size_t capacity) {
  l->items = malloc(sizeof(size_t) * capacity);
  for (size_t i = 0; i < capacity; i++) {
    l->items[i] = SIZE_MAX;
  }
  l->head = SIZE_MAX;
  l->capacity = capacity;
}

int list_push(struct list *list, size_t entity) {
  list->items[entity] = list->head;
  list->head = entity;
  return 0;
}

size_t list_pop(struct list *list) {
  size_t popped = list->head;
  list->head = list->items[popped];
  return popped;
}
