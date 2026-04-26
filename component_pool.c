#include "component_pool.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void *component_pool_emplace(struct component_pool *pool, entity e) {
  if (pool->entities.count >= pool->capacity) {
    sparse_set_dense_realloc_nocheck(&pool->entities);
    pool->data =
        realloc(pool->data, pool->component_size * pool->entities.capacity);
    assert(pool->data);
  }

  uint32_t idx = entity_get_index(e);
  uint32_t page_num = sparse_set_get_page(idx);
  uint32_t offset = sparse_set_get_offset(idx);

  if (pool->entities.pages[page_num] == NULL) {
    sparse_set_allocate_page_nocheck(&pool->entities, page_num);
  }
  sparse_set_push_nocheck(&pool->entities, page_num, offset, e);

  return component_pool_get_by_position(pool, pool->entities.count - 1);
}

int component_pool_remove(struct component_pool *pool, entity e) {
  uint32_t e_idx = entity_get_index(e);

  uint32_t page_num = sparse_set_get_page(e_idx);
  uint32_t offset = sparse_set_get_offset(e_idx);

  if (!pool->entities.pages[page_num]) {
    // page does not exist; fails
    return -1;
  }

  uint32_t dense_idx = pool->entities.pages[page_num][offset];
  if (dense_idx == UINT32_MAX) {
    // does not exist; fails
    return -1;
  }

  pool->entities.pages[page_num][offset] = UINT32_MAX;
  if (dense_idx == --pool->entities.count) {
    // last element, do not swap;
    // just decrease count, value clean up is not required
    return 0;
  }

  size_t count = pool->entities.count;
  memcpy(component_pool_get_by_position(pool, dense_idx),
         component_pool_get_by_position(pool, count), pool->component_size);

  entity last_entity = pool->entities.dense[count];
  uint32_t last_idx = entity_get_index(last_entity);

  sparse_set_map_nocheck(&pool->entities, sparse_set_get_page(last_idx),
                         sparse_set_get_offset(last_idx), dense_idx,
                         last_entity);

  return 0;
}

void component_pool_free(struct component_pool *pool) {
  sparse_set_free(&pool->entities);
  free(pool->data);
}
