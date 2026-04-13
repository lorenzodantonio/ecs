#include "storage.h"

struct storage *storage_new(void) {
  struct storage *storage = malloc(sizeof(*storage));
  if (!storage) {
    return NULL;
  }

  entity_registry_init(&storage->entities);
  component_registry_init(&storage->components);

  return storage;
}

void storage_free(struct storage *s) {
  //
  free(s);
}
