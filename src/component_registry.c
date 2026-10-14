#include "ecs/component_registry.h"
#include <assert.h>

void component_registry_init(struct component_registry *r) {
  r->count = 0;
}

void component_registry_deinit(struct component_registry *r) {
  for (size_t i = 0; i < r->count; i++) {
    component_pool_deinit(&r->pools[i]);
  }
  r->count = 0;
}

struct component_pool *
component_registry_add(struct component_registry *r, size_t component_size, size_t capacity) {
  if (r->count >= MAX_COMPONENTS) {
    return NULL;
  }

  const size_t component_id = r->count;
  struct component_pool *pool = &r->pools[component_id];
  if (component_pool_init(pool, component_id, component_size, capacity) != 0) {
    return NULL;
  }

  r->count++;
  return pool;
}

int component_registry_purge_entity(struct component_registry *r, entity e) {
  for (size_t i = 0; i < r->count; i++) {
    component_pool_remove(&r->pools[i], e);
  }
  return 0;
}

void join_init(struct join *j, size_t component_count,
               struct component_pool **pools) {
  assert(component_count > 0);

  size_t i = 0;
  struct component_pool *leader = pools[i];

  for (i++; i < component_count; i++) {
    if (pools[i]->entities.count < leader->entities.count) {
      j->followers[i - 1] = leader;
      leader = pools[i];
    } else {
      j->followers[i - 1] = pools[i];
    }
  }
  j->leader = leader;
  j->entity = INVALID_ENTITY;
  j->cursor = leader->entities.count;
  j->component_count = component_count;
}
