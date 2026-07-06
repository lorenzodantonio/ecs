#ifndef ENTITY_REGISTRY_H
#define ENTITY_REGISTRY_H

#include "entity.h"
#include <stdlib.h>

struct entity_registry {
  entity head;
  size_t cursor;
  entity entries[ENTITY_IDX_MASK];
};

void entity_registry_init(struct entity_registry *r);
int entity_registry_exists(const struct entity_registry *r, entity e);

entity entity_registry_next(struct entity_registry *r);
int entity_registry_delete(struct entity_registry *r, entity e);

#endif
