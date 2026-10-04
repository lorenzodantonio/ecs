#ifndef STORAGE_H
#define STORAGE_H

#include "component_registry.h"
#include "entity_registry.h"

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
  return entity_registry_next(&storage->entities);
}

static inline int storage_delete_entity(struct storage *storage,
                                         entity e) {
  const int res = entity_registry_delete(&storage->entities, e);
  if (res == -1) {
    return -1;
  }

  component_registry_purge_entity(&storage->components, e);
  return 0;
}

void *storage_emplace_component(struct storage *storage, struct component_pool *pool, entity e);

#endif
