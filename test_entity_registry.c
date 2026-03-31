#include "entity_registry.h"
#include <assert.h>

void entity_registry_new__succeeds(void) {
  struct entity_registry *r = entity_registry_new(16);
  assert(r->active.count == 0);
  assert(r->head == 0);
  assert(r->capacity == 16);
  entity_registry_free(r);
}

void entity_registry_delete__succeeds(void) {
  struct entity_registry *r = entity_registry_new(16);
  size_t e = entity_registry_next(r);
  entity_registry_delete(r, e);
  assert(entity_registry_exists(r, e) == 0);
  entity_registry_free(r);
}

void entity_registry_delete__fails_with_already_deleted_entity(void) {
  struct entity_registry *r = entity_registry_new(16);
  size_t e = entity_registry_next(r);
  entity_registry_delete(r, e);
  assert(entity_registry_exists(r, e) == 0);
  int res = entity_registry_delete(r, e);
  assert(res == -1);

  entity_registry_free(r);
}

void entity_registry_delete__fails_if_entity_does_not_exist(void) {
  struct entity_registry *r = entity_registry_new(16);
  int res = entity_registry_delete(r, 10);
  assert(res == -1);
  entity_registry_free(r);
}

void entity_registry_delete__fails_if_entity_already_deleted(void) {
  struct entity_registry *r = entity_registry_new(16);
  size_t id = entity_registry_next(r);
  assert(entity_registry_delete(r, id) == 0);
  assert(entity_registry_delete(r, id) == -1);
  entity_registry_free(r);
}

void entity_registry_next__succeeds(void) {
  struct entity_registry *r = entity_registry_new(16);
  size_t id = entity_registry_next(r);
  assert(id == 0);
  entity_registry_free(r);
}

void entity_registry_next__reuse_last_deleted(void) {
  struct entity_registry *r = entity_registry_new(16);
  size_t old = entity_registry_next(r);
  entity_registry_delete(r, old);
  size_t new = entity_registry_next(r);
  assert(old == new);

  entity_registry_free(r);
}

void entity_registry_exists__succeeds(void) {
  struct entity_registry *r = entity_registry_new(16);
  size_t id = entity_registry_next(r);
  assert(entity_registry_exists(r, id) == 1);
  entity_registry_free(r);
}

void entity_registry_exists__fails(void) {
  struct entity_registry *r = entity_registry_new(16);
  assert(entity_registry_exists(r, 100) == 0);
  entity_registry_free(r);
}
