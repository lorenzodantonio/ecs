#pragma once

#include "component_pool.h"
#include "entity.h"

#define MAX_COMPONENTS 64

struct component_registry {
  size_t count;
  struct component_pool pools[MAX_COMPONENTS];
};

void component_registry_init(struct component_registry *registry);
void component_registry_free(struct component_registry *registry);

struct component_pool *
component_registry_add(struct component_registry *registry,
                       size_t component_size, size_t capacity);

int component_registry_purge_entity(struct component_registry *registry,
                                    entity e);

struct iterator {
  size_t cursor;
  entity entity;
  size_t component_count;
  struct component_pool *leader;
  struct component_pool *followers[MAX_COMPONENTS - 1];
  void *data[MAX_COMPONENTS];
};

void iterator_init(struct iterator *iter, size_t component_count,
                   struct component_pool **pools);
int iterator_next(struct iterator *iter);

static inline void *iterator_get_field(struct iterator *iter,
                                       struct component_pool *pool) {
  return iter->data[pool->id];
}
