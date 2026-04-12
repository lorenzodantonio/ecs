#include "entity_registry.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int entity_registry_exists(struct entity_registry *registry, entity e) {
  uint32_t idx = entity_get_index(e);
  return idx < registry->cursor && registry->entries[idx] == e;
}

struct entity_registry *entity_registry_new(void) {
  struct entity_registry *registry = malloc(sizeof(*registry));
  if (!registry) {
    return NULL;
  }

  for (size_t i = 0; i < INVALID_ENTITY_IDX; i++) {
    registry->entries[i] = INVALID_ENTITY;
  }
  registry->head = INVALID_ENTITY;
  registry->cursor = 0;
  registry->capacity = INVALID_ENTITY_IDX;

  return registry;
}

void entity_registry_free(struct entity_registry *registry) { free(registry); }

entity entity_registry_next(struct entity_registry *registry) {
  entity e;
  uint32_t idx;

  if (registry->head != INVALID_ENTITY) {
    idx = entity_get_index(registry->head);
    e = entity_new(idx, entity_get_version(registry->head) + 1);
    registry->head = registry->entries[idx];
  } else {
    e = entity_new(registry->cursor, 0);
    idx = registry->cursor++;
  }
  registry->entries[idx] = e;
  return e;
}

int entity_registry_delete(struct entity_registry *registry, entity entity) {
  if (!entity_registry_exists(registry, entity)) {
    return -1;
  }
  uint32_t idx = entity_get_index(entity);
  registry->entries[idx] = registry->head;
  registry->head = entity;
  return 0;
}
