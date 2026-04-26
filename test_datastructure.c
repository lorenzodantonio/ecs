#include "datastructure.h"
#include <assert.h>
#include <stdint.h>

void sparse_set_new__succeeds(void) {
  struct sparse_set *s = sparse_set_new(8);
  assert(s != NULL);
  assert(s->capacity == 8);
  assert(s->count == 0);

  for (size_t i = 0; i < s->page_count; i++) {
    assert(s->pages[i] == NULL);
  }

  sparse_set_free(s);
}

void sparse_set_push__succeds(void) {
  struct sparse_set *s = sparse_set_new(8);
  entity e = entity_new(1, 4);
  int r = sparse_set_push(s, e);
  assert(r == 0);
  assert(s->count == 1);

  uint32_t idx = entity_get_index(e);
  uint32_t page = sparse_set_get_page(idx);
  uint32_t offset = sparse_set_get_offset(idx);
  size_t dense_pos = s->pages[page][offset];
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

  uint32_t idx = entity_get_index(e);
  uint32_t page = sparse_set_get_page(idx);
  uint32_t offset = sparse_set_get_offset(idx);
  assert(s->pages[page][offset] == UINT32_MAX);

  sparse_set_free(s);
}

void sparse_set_remove__swaps_last_dense_array_element(void) {
  struct sparse_set *s = sparse_set_new(8);
  entity e = entity_new(1, 4);
  size_t e_idx = entity_get_index(e);
  sparse_set_push(s, e);
  sparse_set_push(s, entity_new(3, 3));

  entity last_entity = s->dense[s->count - 1];
  uint32_t idx = entity_get_index(e);
  uint32_t page = sparse_set_get_page(idx);
  uint32_t offset = sparse_set_get_offset(idx);

  size_t to_delete_dense_pos =
      s->pages[sparse_set_get_page(e_idx)][sparse_set_get_offset(e_idx)];

  int res = sparse_set_remove(s, e);

  assert(res == 0);
  //   assert(s->count == 1);

  assert(s->dense[to_delete_dense_pos] == last_entity);

  uint32_t last_idx = entity_get_index(last_entity);
  uint32_t last_page = sparse_set_get_page(last_idx);
  uint32_t last_offset = sparse_set_get_offset(last_idx);
  assert(s->pages[last_page][last_offset] == to_delete_dense_pos);

  assert(s->pages[page][offset] == UINT32_MAX);

  sparse_set_free(s);
}

void sparse_set_remove__fails_if_element_does_not_exist(void) {
  struct sparse_set *s = sparse_set_new(8);
  entity e = entity_new(1, 4);
  int r = sparse_set_remove(s, e);
  assert(r == -1);

  uint32_t idx = entity_get_index(e);
  uint32_t page = sparse_set_get_page(idx);

  assert(s->pages[page] == NULL);
  assert(s->count == 0);

  sparse_set_free(s);
}
