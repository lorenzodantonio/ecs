#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include "entity.h"
#include "component_pool.h"

#define MAX_COMPONENTS 64

struct component_registry {
  size_t count;
  struct component_pool pools[MAX_COMPONENTS];
};

void component_registry_init(struct component_registry *r);
void component_registry_deinit(struct component_registry *r);

struct component_pool *
component_registry_add(struct component_registry *r,
                       size_t component_size, size_t capacity);

int component_registry_purge_entity(struct component_registry *r,
                                    entity e);

#endif
