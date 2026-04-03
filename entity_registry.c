#include "entity_registry.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int entity_registry_exists(struct entity_registry *registry, entity e) {
  uint32_t idx = entity_get_index(e);
  return idx < registry->cursor && registry->items[idx] == e;
}

struct entity_registry *entity_registry_new(size_t capacity) {
  struct entity_registry *registry = malloc(sizeof(*registry));
  if (!registry) {
    return NULL;
  }

  registry->items = malloc(sizeof(entity) * capacity);
  for (size_t i = 0; i < capacity; i++) {
    registry->items[i] = EMPTY_ENTITY;
  }
  registry->head = EMPTY_ENTITY;
  registry->cursor = 0;
  registry->capacity = capacity;

  return registry;
}

void entity_registry_free(struct entity_registry *registry) {
  free(registry->items);
  free(registry);
}

entity entity_registry_next(struct entity_registry *registry) {
  entity e;
  uint32_t idx;

  if (registry->head != EMPTY_ENTITY) {
    idx = entity_get_index(registry->head);
    e = entity_new(idx, entity_get_version(registry->head) + 1);
    registry->head = registry->items[idx];
  } else {
    e = entity_new(registry->cursor, 0);
    idx = registry->cursor++;
  }
  registry->items[idx] = e;
  return e;
}

int entity_registry_delete(struct entity_registry *registry, entity entity) {
  if (!entity_registry_exists(registry, entity)) {
    return -1;
  }
  uint32_t idx = entity_get_index(entity);
  registry->items[idx] = registry->head;
  registry->head = entity;
  return 0;
}
