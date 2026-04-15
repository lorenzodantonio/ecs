#include "component_registry.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void component_registry_init(struct component_registry *reg) {
  //
  reg->count = 0;
}

size_t component_registry_add(struct component_registry *registry,
                              size_t component_size, size_t capacity) {
  assert(registry->count < MAX_COMPONENTS);

  size_t id = registry->count++;
  struct component_pool *pool = &registry->pools[id];
  pool->id = id;
  pool->component_size = component_size;
  pool->data = malloc(component_size * capacity);
  sparse_set_init(&pool->entities, capacity);
  return id;
}

int component_registry_purge_entity(struct component_registry *registry,
                                    entity e) {
  for (size_t i = 0; i < registry->count; i++) {
    component_pool_remove(&registry->pools[i], e);
  }

  return 0;
}

void iterator_init(struct iterator *iter, size_t component_count,
                   struct component_pool **pools) {
  size_t i = 0;
  struct component_pool *leader = pools[i];

  for (i++; i < component_count; i++) {
    if (pools[i]->entities.count < leader->entities.count) {
      iter->followers[i - 1] = leader;
      leader = pools[i];
    } else {
      iter->followers[i - 1] = pools[i];
    }
  }
  iter->leader = leader;
  iter->entity = INVALID_ENTITY;
  iter->cursor = 0;
  iter->component_count = component_count;
}

int iterator_next(struct iterator *iter) {
  size_t cursor;
  entity e = INVALID_ENTITY;

  int match = 0;
  while (!match && iter->cursor < iter->leader->entities.count) {
    e = iter->leader->entities.dense[iter->cursor];

    int inner_match = 1;
    size_t j = 0;
    while (inner_match && j < iter->component_count - 1) {
      struct component_pool *f = iter->followers[j++];
      inner_match =
          inner_match && f->entities.sparse[entity_get_index(e)] != UINT32_MAX;
    }
    match = inner_match;
    cursor = iter->cursor++;
  }

  if (!match) {
    return 0;
  }

  iter->entity = e;

  iter->data[iter->leader->id] =
      component_pool_get_by_position(iter->leader, cursor);
  for (size_t k = 0; k < iter->component_count - 1; k++) {
    iter->data[iter->followers[k]->id] =
        component_pool_get_by_entity(iter->followers[k], e);
  }

  return match;
}

void component_registry_free(struct component_registry *registry) {
  for (size_t i = 0; i < registry->count; i++) {
    component_pool_free(&registry->pools[i]);
  }
}
