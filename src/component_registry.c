#include "ecs/component_registry.h"
#include <assert.h>

void component_registry_init(struct component_registry *r) {
  r->count = 0;
}

void component_registry_deinit(struct component_registry *r) {
  for (size_t i = 0; i < r->count; i++) {
    component_pool_deinit(&r->pools[i]);
    r->count = 0;
  }
}

struct component_pool *
component_registry_add(struct component_registry *r,
                       size_t component_size, size_t capacity) {
  if (r->count >= MAX_COMPONENTS)
    return NULL;

  const size_t component_id = r->count;
  struct component_pool *pool = &r->pools[component_id];
  if (component_pool_init(pool, component_id, component_size, capacity) != 0)
    return NULL;

  r->count++;
  return pool;
}

int component_registry_purge_entity(struct component_registry *r, entity e) {
  for (size_t i = 0; i < r->count; i++)
    component_pool_remove(&r->pools[i], e);
  return 0;
}

void join_init(struct join *iter, size_t component_count,
               struct component_pool **pools) {
  assert(component_count > 0);

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
  iter->cursor = leader->entities.count;
  iter->component_count = component_count;
}

int join_next(struct join *j) {
  size_t cursor = 0;
  entity e = INVALID_ENTITY;

  int match = 0;
  while (!match && j->cursor) {
    cursor = --j->cursor;
    e = j->leader->entities.dense[cursor];

    int inner_match = 1;
    size_t k = 0;
    while (inner_match && k < j->component_count - 1) {
      struct component_pool *f = j->followers[k++];
      const uint32_t pos = sparse_set_find(&f->entities, e);
      inner_match = pos != UINT32_MAX;
      if (inner_match) {
        j->data[f->id] = component_pool_get_by_position(f, pos);
      }
    }
    match = inner_match;
  }

  if (!match) {
    return 0;
  }

  j->entity = e;
  j->data[j->leader->id] = component_pool_get_by_position(j->leader, cursor);

  return match;
}
