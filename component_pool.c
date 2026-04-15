#include "component_pool.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int component_pool_remove(struct component_pool *pool, entity e) {
  //
  uint32_t e_idx = entity_get_index(e);
  uint32_t dense_idx = pool->entities.sparse[e_idx];
  if (dense_idx == UINT32_MAX) {
    // does not exist; fails
    return -1;
  }

  pool->entities.sparse[e_idx] = UINT32_MAX;
  if (dense_idx == --pool->entities.count) {
    // last element, do not swap;
    // just decrease count, value clean up is not required
    return 0;
  }

  size_t count = pool->entities.count;
  memcpy(component_pool_get_by_position(pool, dense_idx),
         component_pool_get_by_position(pool, count), pool->component_size);

  entity last_entity = pool->entities.dense[count];

  pool->entities.sparse[entity_get_index(last_entity)] = dense_idx;
  pool->entities.dense[dense_idx] = last_entity;

  return 0;
}

void component_pool_free(struct component_pool *pool) {
  sparse_set_free(&pool->entities);
  free(pool->data);
}
