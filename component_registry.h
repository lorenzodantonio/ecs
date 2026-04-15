#pragma once

#include "component_pool.h"
#include "datastructure.h"
#include "entity.h"
#include <stdint.h>

#define MAX_COMPONENTS 64

struct component_registry {
  size_t count;
  struct component_pool pools[MAX_COMPONENTS];
};

void component_registry_init(struct component_registry *registry);
void component_registry_free(struct component_registry *registry);

size_t component_registry_add(struct component_registry *registry,
                              size_t component_size, size_t capacity);

int component_registry_purge_entity(struct component_registry *registry,
                                    entity e);

static inline struct component_pool *
component_registry_get(struct component_registry *registry,
                       size_t component_id) {
  return &registry->pools[component_id];
}

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
