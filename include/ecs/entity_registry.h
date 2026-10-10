#ifndef ENTITY_REGISTRY_H
#define ENTITY_REGISTRY_H

#include "entity.h"
#include <stddef.h>

struct entity_registry {
  entity head;
  size_t count;
  size_t cursor;
  size_t capacity;  // indexes limit
  entity *slots;
};

int entity_registry_init(struct entity_registry *r, size_t capacity);
void entity_registry_deinit(struct entity_registry *r);

int entity_registry_exists(const struct entity_registry *r, entity e);

entity entity_registry_next(struct entity_registry *r);
int entity_registry_delete(struct entity_registry *r, entity e);

#endif
