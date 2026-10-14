#include "ecs/entity_registry.h"
#include <assert.h>

void entity_registry_init__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  assert(r.cursor == 0);
  entity_registry_deinit(&r);
}

void entity_registry_init__fails_when_capacity_is_zero(void) {
  struct entity_registry r;
  assert(entity_registry_init(&r, 0) == -1);
}

void entity_registry_init__fails_when_capacity_exceeds(void) {
  struct entity_registry r;
  assert(entity_registry_init(&r, ENTITY_IDX_MASK + 1) == -1);
}

void entity_registry_revoke__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity e = entity_registry_issue(&r);
  entity_registry_revoke(&r, e);
  assert(entity_registry_exists(&r, e) == 0);
  entity_registry_deinit(&r);
}

void entity_registry_revoke__fails_if_entity_does_not_exist(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity e = entity_new(10, 0);
  assert(entity_registry_revoke(&r, e) == -1);
  entity_registry_deinit(&r);
}

void entity_registry_revoke__fails_if_entity_already_deleted(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity e = entity_registry_issue(&r);
  assert(entity_registry_revoke(&r, e) == 0);
  assert(entity_registry_revoke(&r, e) == -1);
  entity_registry_deinit(&r);
}

void entity_registry_issue__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity e = entity_registry_issue(&r);
  assert(e == entity_new(0, 0));
  entity_registry_deinit(&r);
}

void entity_registry_issue__recycle_last_deletion(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  entity old = entity_registry_issue(&r);
  entity expected = entity_new(entity_get_index(old), 1);
  entity_registry_revoke(&r, old);

  assert(entity_registry_issue(&r) == expected);
  entity_registry_deinit(&r);
}

void entity_registry_exists__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  const size_t id = entity_registry_issue(&r);
  assert(entity_registry_exists(&r, id) == 1);
  entity_registry_deinit(&r);
}

void entity_registry_exists__fails(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);
  assert(entity_registry_exists(&r, 100) == 0);
  entity_registry_deinit(&r);
}

void entity_registry_issue__stops_before_overflow(void) {
  struct entity_registry r;
  entity_registry_init(&r, 4);

  entity res;
  for (size_t i = 0; i < 4; i++) {
    res = entity_registry_issue(&r);
    assert(res != INVALID_ENTITY);
  }

  res = entity_registry_issue(&r);
  assert(res == INVALID_ENTITY);

  entity_registry_deinit(&r);
}

int main(void) {
  entity_registry_init__succeeds();
  entity_registry_init__fails_when_capacity_is_zero();
  entity_registry_init__fails_when_capacity_exceeds();

  entity_registry_revoke__succeeds();
  entity_registry_revoke__fails_if_entity_does_not_exist();
  entity_registry_revoke__fails_if_entity_already_deleted();

  entity_registry_issue__succeeds();
  entity_registry_issue__recycle_last_deletion();
  entity_registry_issue__stops_before_overflow();

  entity_registry_exists__succeeds();
  entity_registry_exists__fails();

  return 0;
}