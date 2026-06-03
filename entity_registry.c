#include "entity_registry.h"

int entity_registry_exists(const struct entity_registry *r, entity e) {
  const uint32_t idx = entity_get_index(e);
  return idx < r->cursor && r->entries[idx] == e;
}

void entity_registry_init(struct entity_registry *r) {
  for (size_t i = 0; i < ENTITY_IDX_MASK; i++) {
    r->entries[i] = INVALID_ENTITY;
  }
  r->head = INVALID_ENTITY;
  r->cursor = 0;
}

entity entity_registry_next(struct entity_registry *r) {
  uint32_t idx, ver;

  if (r->head != INVALID_ENTITY) {
    idx = entity_get_index(r->head);
    ver = entity_get_version(r->head) + 1;
    r->head = r->entries[idx];
  } else {
    idx = r->cursor++;
    ver = 0;
  }

  const entity e = entity_new(idx, ver);
  r->entries[idx] = e;

  return e;
}

int entity_registry_delete(struct entity_registry *r, entity e) {
  if (!entity_registry_exists(r, e)) {
    return -1;
  }

  const uint32_t idx = entity_get_index(e);
  r->entries[idx] = r->head;
  r->head = e;

  return 0;
}
