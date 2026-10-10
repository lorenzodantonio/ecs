#include "ecs/storage.h"

struct storage *storage_new(void) {
  struct storage *storage = malloc(sizeof(*storage));
  if (!storage) {
    return NULL;
  }

  if (entity_registry_init(&storage->entities, ENTITY_IDX_MASK) == -1) {
    free(storage);
    return NULL;
  }
  component_registry_init(&storage->components);

  return storage;
}

void *storage_emplace_component(struct storage *storage, struct component_pool *pool, entity e) {
  if (!entity_registry_exists(&storage->entities, e)) {
    return NULL;
  }
  if (component_pool_get_by_entity(pool, e) != NULL) {
    return NULL;
  }
  return component_pool_emplace(pool, e);
}

void storage_free(struct storage *s) {
  entity_registry_deinit(&s->entities);
  component_registry_deinit(&s->components);
  free(s);
}
