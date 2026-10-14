#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include "entity.h"
#include "component_pool.h"

#define MAX_COMPONENTS 64

struct component_registry {
  size_t count;
  struct component_pool pools[MAX_COMPONENTS];
};

void component_registry_init(struct component_registry *r);
void component_registry_deinit(struct component_registry *r);

struct component_pool *
component_registry_add(struct component_registry *r,
                       size_t component_size, size_t capacity);

int component_registry_purge_entity(struct component_registry *r,
                                    entity e);

struct join {
  size_t cursor;
  entity entity;
  size_t component_count;
  struct component_pool *leader;
  struct component_pool *followers[MAX_COMPONENTS - 1];
  void *data[MAX_COMPONENTS];
};

void join_init(struct join *iter, size_t component_count,
                   struct component_pool **pools);

static inline int join_next(struct join *j) {
  size_t cursor = 0;
  entity e = INVALID_ENTITY;

  int match = 0;
  while (!match && j->cursor) {
    cursor = --j->cursor;
    e = j->leader->entities.dense[cursor];

    int inner_match = 1;
    size_t k = 0;
    while (inner_match && k < j->component_count - 1) {
      struct component_pool *f = j->followers[k++];
      const uint32_t pos = sparse_set_find(&f->entities, e);
      inner_match = pos != UINT32_MAX;
      if (inner_match) {
        j->data[f->id] = component_pool_get_by_position(f, pos);
      }
    }
    match = inner_match;
  }

  if (!match) {
    return 0;
  }

  j->entity = e;
  j->data[j->leader->id] = component_pool_get_by_position(j->leader, cursor);

  return match;
}

static inline void *join_get_component(struct join *j, struct component_pool *pool) {
  return j->data[pool->id];
}

#endif
