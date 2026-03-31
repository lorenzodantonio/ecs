#include "entity_registry.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int entity_registry_exists(struct entity_registry *registry, size_t entity) {
  return entity < registry->active.count &&
         registry->active.sparse[entity] != SIZE_MAX;
}

struct entity_registry *entity_registry_new(size_t capacity) {
  struct entity_registry *registry = malloc(sizeof(*registry));
  if (!registry) {
    return NULL;
  }

  list_init(&registry->reusable, capacity);
  sparse_set_init(&registry->active, capacity);

  registry->capacity = capacity;
  registry->head = 0;
  return registry;
}

void entity_registry_free(struct entity_registry *registry) {
  free(registry->active.dense);
  free(registry->active.sparse);
  free(registry);
}

size_t entity_registry_next(struct entity_registry *registry) {
  size_t id;
  if (registry->reusable.head != SIZE_MAX) {
    id = list_pop(&registry->reusable);
  } else {
    id = registry->head++;
  }

  sparse_set_push(&registry->active, id);
  return id;
}

int entity_registry_delete(struct entity_registry *registry, size_t entity) {
  int removed = sparse_set_remove(&registry->active, entity);
  if (removed == -1) {
    return -1;
  }

  list_push(&registry->reusable, entity);
  return 0;
}
