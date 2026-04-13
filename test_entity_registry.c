#include "entity_registry.h"
#include <assert.h>

void entity_registry_new__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r);
  assert(r.cursor == 0);
}

void entity_registry_delete__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r);
  entity e = entity_registry_next(&r);
  entity_registry_delete(&r, e);
  assert(entity_registry_exists(&r, e) == 0);
}

void entity_registry_delete__fails_if_entity_does_not_exist(void) {
  struct entity_registry r;
  entity_registry_init(&r);
  entity e = entity_new(10, 0);
  int res = entity_registry_delete(&r, e);
  assert(res == -1);
}

void entity_registry_delete__fails_if_entity_already_deleted(void) {
  struct entity_registry r;
  entity_registry_init(&r);
  entity e = entity_registry_next(&r);
  assert(entity_registry_delete(&r, e) == 0);
  assert(entity_registry_delete(&r, e) == -1);
}

void entity_registry_next__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r);
  size_t id = entity_registry_next(&r);
  assert(id == 0);
}

void entity_registry_next__reuse_last_index_deleted(void) {
  struct entity_registry r;
  entity_registry_init(&r);
  entity old = entity_registry_next(&r);
  entity expected = entity_new(entity_get_index(old), 1);
  entity_registry_delete(&r, old);

  assert(entity_registry_next(&r) == expected);
}

void entity_registry_exists__succeeds(void) {
  struct entity_registry r;
  entity_registry_init(&r);
  size_t id = entity_registry_next(&r);
  assert(entity_registry_exists(&r, id) == 1);
}

void entity_registry_exists__fails(void) {
  struct entity_registry r;
  entity_registry_init(&r);
  assert(entity_registry_exists(&r, 100) == 0);
}
