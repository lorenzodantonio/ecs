#include "datastructure.h"
#include <assert.h>
#include <stdint.h>

void sparse_set_new__succeeds(void) {
  struct sparse_set *s = sparse_set_new(8);
  assert(s != NULL);
  assert(s->capacity == 8);
  assert(s->count == 0);

  for (size_t i = 0; i < s->capacity; i++) {
    assert(s->sparse[i] == UINT32_MAX);
  }

  sparse_set_free(s);
}

void sparse_set_push__succeds(void) {
  struct sparse_set *s = sparse_set_new(8);
  entity e = entity_new(1, 4);
  int r = sparse_set_push(s, e);
  assert(r == 0);
  assert(s->count == 1);
  size_t dense_pos = s->sparse[entity_get_index(e)];
  assert(dense_pos == s->count - 1);
  sparse_set_free(s);
}

void sparse_set_push__fails_if_element_already_exists(void) {
  struct sparse_set *s = sparse_set_new(8);
  entity e = entity_new(1, 4);
  sparse_set_push(s, e);
  int r = sparse_set_push(s, e);
  assert(r == -1);
  assert(s->count == 1);

  sparse_set_free(s);
}

void sparse_set_remove__succeeds_with_last_element(void) {
  struct sparse_set *s = sparse_set_new(8);
  entity e = entity_new(1, 4);
  sparse_set_push(s, e);

  int r = sparse_set_remove(s, e);
  assert(r == 0);
  assert(s->count == 0);
  assert(s->sparse[entity_get_index(e)] == UINT32_MAX);

  sparse_set_free(s);
}

void sparse_set_remove__swaps_last_dense_array_element(void) {
  struct sparse_set *s = sparse_set_new(8);
  entity e = entity_new(1, 4);
  size_t e_idx = entity_get_index(e);
  sparse_set_push(s, e);
  sparse_set_push(s, entity_new(3, 3));

  entity last_entity = s->dense[s->count - 1];
  size_t to_delete_dense_pos = s->sparse[e_idx];

  int r = sparse_set_remove(s, e);
  assert(r == 0);
  assert(s->dense[to_delete_dense_pos] == last_entity);
  assert(s->sparse[e_idx] == UINT32_MAX);

  sparse_set_free(s);
}

void sparse_set_remove__fails_if_element_does_not_exist(void) {
  struct sparse_set *s = sparse_set_new(8);
  entity e = entity_new(1, 4);
  int r = sparse_set_remove(s, e);
  assert(r == -1);
  assert(s->sparse[entity_get_index(e)] == UINT32_MAX);
  assert(s->count == 0);

  sparse_set_free(s);
}
