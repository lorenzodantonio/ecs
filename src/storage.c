#include "ecs/storage.h"
#include <stdlib.h>

struct storage *storage_new(void) {
  struct storage *s = malloc(sizeof *s);
  if (!s) {
    return NULL;
  }

  if (entity_registry_init(&s->entities, ENTITY_IDX_MASK) != 0) {
    free(s);
    return NULL;
  }

  component_registry_init(&s->components);
  return s;
}

void storage_free(struct storage *s) {
  entity_registry_deinit(&s->entities);
  component_registry_deinit(&s->components);
  free(s);
}

void *storage_add_component(struct storage *storage, struct component_pool *pool, entity e) {
  if (!entity_registry_exists(&storage->entities, e)) {
    return NULL;
  }

  if (component_pool_find(pool, e) != UINT32_MAX) {
    return NULL;
  }

  return component_pool_emplace(pool, e);
}

void *storage_set_component(struct storage *s, struct component_pool *pool, entity e, const void *value) {
  if (!entity_registry_exists(&s->entities, e)) {
    return NULL;
  }

  void *c = component_pool_get_by_entity(pool, e);
  if (!c) {
    c = component_pool_emplace(pool, e);
    if (!c) {
      return NULL;
    }
  }

  memcpy(c, value, pool->component_size);
  return c;
}

int storage_unset_component(struct storage *s, struct component_pool *pool, entity e) {
  if (!entity_registry_exists(&s->entities, e)) {
    return 0;
  }
  return component_pool_remove(pool, e);
}
