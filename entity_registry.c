#include "entity_registry.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int entity_registry_exists(struct entity_registry *registry, size_t entity) {
  return entity < registry->cursor && registry->items[entity] == entity;
}

struct entity_registry *entity_registry_new(size_t capacity) {
  struct entity_registry *registry = malloc(sizeof(*registry));
  if (!registry) {
    return NULL;
  }

  registry->items = malloc(sizeof(size_t) * capacity);
  for (size_t i = 0; i < capacity; i++) {
    registry->items[i] = SIZE_MAX;
  }
  registry->head = SIZE_MAX;
  registry->cursor = 0;
  registry->capacity = capacity;

  return registry;
}

void entity_registry_free(struct entity_registry *registry) {
  free(registry->items);
  free(registry);
}

size_t entity_registry_next(struct entity_registry *registry) {
  size_t id;
  if (registry->head != SIZE_MAX) {
    id = registry->head;
    registry->head = registry->items[id];
  } else {
    id = registry->cursor++;
  }

  registry->items[id] = id;

  return id;
}

int entity_registry_delete(struct entity_registry *registry, size_t entity) {
  if (!entity_registry_exists(registry, entity)) {
    return -1;
  }
  registry->items[entity] = registry->head;
  registry->head = entity;
  return 0;
}
