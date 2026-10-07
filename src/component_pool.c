#include "ecs/component_pool.h"


int component_pool_init(struct component_pool *pool, size_t id,
                         size_t component_size, size_t capacity) {
  pool->id = id;
  pool->component_size = component_size;
  if (component_size == 0) {
    return -1;
  }

  pool->data = malloc(component_size * capacity);
  if (pool->data == NULL) {
    return -1;
  }

  if (sparse_set_init(&pool->entities, capacity) == -1) {
    free(pool->data);
    return -1;
  }

  return 0;
}

void *component_pool_emplace(struct component_pool *pool, entity e) {
  if (pool->entities.count >= pool->entities.capacity) {
    void *resized =
        realloc(pool->data, pool->component_size * (pool->entities.capacity * 2));

    if (resized == NULL) {
      return NULL;
    }
    pool->data = resized;

    if (sparse_set_dense_realloc_nocheck(&pool->entities)) {
      return NULL;
    }
  }
  const uint32_t page_num = sparse_set_get_page(entity_get_index(e));

  if (pool->entities.pages[page_num] == NULL) {
    if (sparse_set_allocate_page_nocheck(&pool->entities, page_num) == NULL) {
      return NULL;
    }
  }
  sparse_set_push_nocheck(&pool->entities, e);

  return component_pool_get_by_position(pool, pool->entities.count - 1);
}

int component_pool_remove(struct component_pool *pool, entity e) {
  const uint32_t e_idx = entity_get_index(e);

  const uint32_t page_num = sparse_set_get_page(e_idx);
  const uint32_t offset = sparse_set_get_offset(e_idx);

  if (!pool->entities.pages[page_num]) {
    // page does not exist; fails
    return -1;
  }

  const uint32_t dense_idx = pool->entities.pages[page_num][offset];
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

  const size_t count = pool->entities.count;
  memcpy(component_pool_get_by_position(pool, dense_idx),
         component_pool_get_by_position(pool, count), pool->component_size);

  const entity last_entity = pool->entities.dense[count];
  sparse_set_map_nocheck(&pool->entities, last_entity, dense_idx);

  return 0;
}

void component_pool_deinit(struct component_pool *pool) {
  sparse_set_deinit(&pool->entities);
  free(pool->data);
}
