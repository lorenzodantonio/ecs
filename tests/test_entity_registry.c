#include "ecs/entity_registry.h"
#include <assert.h>

void entity_registry_init__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  assert(r.cursor == 0);
  entity_registry_deinit(&r);
}

void entity_registry_init_fails_on_exceeding_capacity(void) {
  struct entity_registry r;
  int res = entity_registry_init(&r, ENTITY_IDX_MASK + 1);
  assert(res == -1);
  entity_registry_deinit(&r);
}

void entity_registry_delete__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity e = entity_registry_next(&r);
  entity_registry_delete(&r, e);
  assert(entity_registry_exists(&r, e) == 0);
  entity_registry_deinit(&r);
}

void entity_registry_delete__fails_if_entity_does_not_exist(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity e = entity_new(10, 0);
  int res = entity_registry_delete(&r, e);
  assert(res == -1);
  entity_registry_deinit(&r);
}

void entity_registry_delete__fails_if_entity_already_deleted(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity e = entity_registry_next(&r);
  assert(entity_registry_delete(&r, e) == 0);
  assert(entity_registry_delete(&r, e) == -1);
  entity_registry_deinit(&r);
}

void entity_registry_next__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  const size_t id = entity_registry_next(&r);
  assert(id == 0);
  entity_registry_deinit(&r);
}

void entity_registry_next__reuse_last_index_deleted(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity old = entity_registry_next(&r);
  entity expected = entity_new(entity_get_index(old), 1);
  entity_registry_delete(&r, old);

  assert(entity_registry_next(&r) == expected);
  entity_registry_deinit(&r);
}

void entity_registry_exists__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  const size_t id = entity_registry_next(&r);
  assert(entity_registry_exists(&r, id) == 1);
  entity_registry_deinit(&r);
}

void entity_registry_exists__fails(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  assert(entity_registry_exists(&r, 100) == 0);
  entity_registry_deinit(&r);
}

void entity_registry_next__stops_before_overflow(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);

  entity res;
  for (size_t i = 0; i < 4; i++) {
    res = entity_registry_next(&r);
    assert(res != INVALID_ENTITY);
  }

  res = entity_registry_next(&r);
  assert(res == INVALID_ENTITY);

  entity_registry_deinit(&r);
}

int main(void) {
  entity_registry_init__succeeds();
  entity_registry_init_fails_on_exceeding_capacity();

  entity_registry_delete__succeeds();
  entity_registry_delete__fails_if_entity_does_not_exist();
  entity_registry_delete__fails_if_entity_already_deleted();

  entity_registry_next__succeeds();
  entity_registry_next__reuse_last_index_deleted();

  entity_registry_exists__succeeds();
  entity_registry_exists__fails();

  entity_registry_next__stops_before_overflow();

  return 0;
}