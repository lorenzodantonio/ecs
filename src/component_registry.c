#include "ecs/component_registry.h"

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
