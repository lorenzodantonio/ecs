#pragma once

#include "datastructure.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_COMPONENTS 128

struct component_pool {
  size_t id;
  size_t capacity;
  size_t component_size;
  struct sparse_set sparse_set;
  void *data;
};

int component_pool_remove(struct component_pool *pool, size_t entity);

struct component_registry {
  size_t count;
  struct component_pool pools[MAX_COMPONENTS];
};

struct component_registry *component_registry_new();
void component_registry_free(struct component_registry *registry);

size_t component_registry_add(struct component_registry *registry,
                              size_t component_size, size_t capacity);

int component_registry_purge_entity(struct component_registry *registry,
                                    size_t entity);

static inline struct component_pool *
component_registry_get(struct component_registry *registry,
                       size_t component_id) {
  return &registry->pools[component_id];
}

static inline void *component_pool_get_by_position(struct component_pool *pool,
                                                   size_t position) {
  return pool->data + position * pool->component_size;
}

static inline void *component_pool_get_by_entity(struct component_pool *pool,
                                                 size_t entity) {
  size_t pos = pool->sparse_set.sparse[entity];
  return component_pool_get_by_position(pool, pos);
}

static inline void *component_pool_emplace(struct component_pool *pool,
                                           uint32_t index) {
  int res = sparse_set_push(&pool->sparse_set, index);
  if (res == -1) {
    fprintf(stderr, "error mapping entity to sparse set\n");
    return NULL;
  }

  return component_pool_get_by_position(pool, pool->sparse_set.count - 1);
}

struct iterator {
  size_t cursor;
  size_t entity;
  size_t component_count;
  struct component_pool *leader;
  struct component_pool *followers[MAX_COMPONENTS - 1];
  void *data[MAX_COMPONENTS];
};

void iterator_init(struct iterator *iter, size_t component_count,
                   struct component_pool **pools);
int iterator_next(struct iterator *iter);
