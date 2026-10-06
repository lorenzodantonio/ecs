#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include "entity.h"
#include "component_pool.h"

#define MAX_COMPONENTS 64

struct component_registry {
  size_t count;
  struct component_pool pools[MAX_COMPONENTS];
};

void component_registry_init(struct component_registry *registry);
void component_registry_deinit(struct component_registry *registry);

struct component_pool *
component_registry_add(struct component_registry *registry,
                       size_t component_size, size_t capacity);

int component_registry_purge_entity(struct component_registry *registry,
                                    entity e);

struct join {
  size_t cursor;
  entity entity;
  size_t component_count;
  struct component_pool *leader;
  struct component_pool *followers[MAX_COMPONENTS - 1];
  void *data[MAX_COMPONENTS];
};

void join_init(struct join *iter, size_t component_count,
                   struct component_pool **pools);

int join_next(struct join *j);

static inline void *join_get_field(struct join *j, struct component_pool *pool) {
  return j->data[pool->id];
}

#endif
