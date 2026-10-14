#ifndef STORAGE_H
#define STORAGE_H

#include "component_registry.h"
#include "entity_registry.h"
#include <stdbool.h>

#define INITIAL_ENTITIES 1024

struct storage {
  struct entity_registry entities;
  struct component_registry components;
};

struct storage *storage_new(void);
void storage_free(struct storage *s);

static inline struct component_pool *
storage_register_component(struct storage *storage, size_t size) {
  return component_registry_add(&storage->components, size, INITIAL_ENTITIES);
}

static inline entity storage_create_entity(struct storage *storage) {
  return entity_registry_issue(&storage->entities);
}

static inline int storage_destroy_entity(struct storage *storage,
                                         entity e) {
  const int res = entity_registry_revoke(&storage->entities, e);
  if (res == -1) {
    return -1;
  }

  component_registry_purge_entity(&storage->components, e);
  return 0;
}

static inline bool storage_exists(struct storage *s, entity e) {
  return entity_registry_exists(&s->entities, e);
}

void *storage_add_component(struct storage *s, struct component_pool *pool, entity e);
void *storage_set_component(struct storage *s, struct component_pool *pool, entity e, const void *value);
int storage_unset_component(struct storage *s, struct component_pool *pool, entity e);

#endif
