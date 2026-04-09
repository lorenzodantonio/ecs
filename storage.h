#pragma once

#include "component_registry.h"
#include "entity_registry.h"
#include <stdlib.h>

struct storage {
  struct entity_registry *entities;
  struct component_registry *components;
};

struct storage *storage_new(void);
void storage_free(struct storage *s);

static inline size_t storage_register_component(struct storage *storage,
                                                size_t size) {
  return component_registry_add(storage->components, size, INVALID_ENTITY_IDX);
}

static inline struct component_pool *storage_get_pool(struct storage *storage,
                                                      size_t component_id) {
  return component_registry_get(storage->components, component_id);
}

static inline void storage_iterator_init(struct storage *storage,
                                         struct iterator *iter,
                                         size_t component_count,
                                         size_t component_ids[]) {
  struct component_pool *pools[64];
  for (size_t i = 0; i < component_count; i++) {
    pools[i] = component_registry_get(storage->components, component_ids[i]);
  }
  iterator_init(iter, component_count, pools);
}

static inline size_t storage_create_entity(struct storage *storage) {
  return entity_registry_next(storage->entities);
}

static inline void *storage_emplace_component(struct component_pool *pool,
                                              entity e) {
  return component_pool_emplace(pool, entity_get_index(e));
}

static inline void storage_delete_entity(struct storage *storage,
                                         size_t entity) {
  component_registry_purge_entity(storage->components, entity);
  entity_registry_delete(storage->entities, entity);
}
