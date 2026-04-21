#include "datastructure.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int sparse_set_init(struct sparse_set *set, uint32_t capacity) {
  set->sparse = malloc(sizeof(uint32_t) * (size_t)capacity);
  set->dense = malloc(sizeof(entity) * capacity);

  for (size_t i = 0; i < capacity; i++) {
    set->sparse[i] = UINT32_MAX;
  }

  set->count = 0;
  set->capacity = capacity;

  return 0;
}

struct sparse_set *sparse_set_new(uint32_t capacity) {
  struct sparse_set *set = (struct sparse_set *)malloc(sizeof(*set));
  sparse_set_init(set, capacity);
  return set;
}

int sparse_set_push(struct sparse_set *set, entity e) {
  uint32_t idx = entity_get_index(e);
  if (set->sparse[idx] != UINT32_MAX) {
    return -1;
  }

  set->sparse[idx] = set->count;
  set->dense[set->count++] = e;

  return 0;
}

int sparse_set_remove(struct sparse_set *set, entity e) {
  uint32_t idx = entity_get_index(e);
  uint32_t position = set->sparse[idx];
  if (position == UINT32_MAX) {
    return -1;
  }

  set->sparse[idx] = UINT32_MAX;
  if (position == --set->count) {
    return 0;
  }

  entity last_entity = set->dense[set->count];
  set->sparse[entity_get_index(last_entity)] = position;
  set->dense[position] = last_entity;

  return 0;
}

void sparse_set_free(struct sparse_set *set) {
  free(set->sparse);
  free(set->dense);
}
