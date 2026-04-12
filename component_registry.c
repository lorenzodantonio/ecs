#include "component_registry.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct component_registry *component_registry_new() {
  struct component_registry *registry = malloc(sizeof(*registry));

  registry->count = 0;
  // registry->capacity = capacity;
  return registry;
}

void component_registry_free(struct component_registry *registry) {
  free(registry);
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

int component_pool_remove(struct component_pool *pool, entity e) {
  //
  size_t e_idx = entity_get_index(e);
  size_t dense_idx = pool->entities.sparse[e_idx];
  if (dense_idx == SIZE_MAX) {
    // does not exist; fails
    return -1;
  }

  pool->entities.sparse[e_idx] = SIZE_MAX;
  if (dense_idx == --pool->entities.count) {
    // last element, do not swap;
    // just decrease count, value clean up is not required
    return 0;
  }

  size_t count = pool->entities.count;
  memcpy(component_pool_get_by_position(pool, dense_idx),
         component_pool_get_by_position(pool, count), pool->component_size);

  entity last_entity = pool->entities.dense[count];

  pool->entities.sparse[entity_get_index(last_entity)] = dense_idx;
  pool->entities.dense[dense_idx] = last_entity;

  return 0;
}

int component_registry_purge_entity(struct component_registry *registry,
                                    entity e) {
  for (size_t i = 0; i < registry->count; i++) {
    if (registry->pools[i].entities.sparse[entity_get_index(e)] != SIZE_MAX) {
      component_pool_remove(&registry->pools[i], e);
    }
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
          inner_match && f->entities.sparse[entity_get_index(e)] != SIZE_MAX;
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
