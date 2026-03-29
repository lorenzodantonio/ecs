#include "storage.h"

struct storage *storage_new(size_t max_entities) {
  struct storage *storage = malloc(sizeof(*storage));
  if (!storage) {
    return NULL;
  }

  storage->entities = entity_registry_new(max_entities);
  if (!storage->entities) {
    return NULL;
  }

  storage->components = component_registry_new(32);
  if (!storage->components) {
    return NULL;
  }

  return storage;
}

void storage_free(struct storage *s) {
  entity_registry_free(s->entities);
  component_registry_free(s->components);
  free(s);
}
