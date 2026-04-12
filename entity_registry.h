#pragma once

#include "entity.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct entity_registry {
  entity head;
  size_t cursor;
  size_t capacity;
  entity entries[INVALID_ENTITY_IDX];
};

struct entity_registry *entity_registry_new(void);
void entity_registry_free(struct entity_registry *reg);

int entity_registry_exists(struct entity_registry *reg, entity e);

entity entity_registry_next(struct entity_registry *reg);
int entity_registry_delete(struct entity_registry *reg, entity e);
