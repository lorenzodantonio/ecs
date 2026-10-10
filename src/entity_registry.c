#include "ecs/entity_registry.h"
#include <stdlib.h>

int entity_registry_exists(const struct entity_registry *r, entity e) {
  const uint32_t idx = entity_get_index(e);
  return idx < r->cursor && r->slots[idx] == e;
}

int entity_registry_init(struct entity_registry *r, size_t capacity) {
  if (capacity == 0) {
    return -1;
  }

  if (capacity > ENTITY_IDX_MASK) {
    return -1;
  }

  r->slots = malloc(sizeof(entity) * capacity);
  if (r->slots == NULL) {
    return -1;
  }

  r->count = 0;
  r->head = INVALID_ENTITY;
  r->cursor = 0;
  r->capacity = capacity;

  return 0;
}

void entity_registry_deinit(struct entity_registry *r) {
  free(r->slots);
  r->slots = NULL;
}

entity entity_registry_next(struct entity_registry *r) {
  uint32_t idx, ver;

  if (r->head != INVALID_ENTITY) {
    idx = entity_get_index(r->head);
    ver = entity_get_version(r->head) + 1;
    r->head = r->slots[idx];
  } else {
    if (r->cursor >= r->capacity) {
      return INVALID_ENTITY;
    }
    idx = r->cursor++;
    ver = 0;
  }

  const entity e = entity_new(idx, ver);
  r->slots[idx] = e;

  r->count += 1;

  return e;
}

int entity_registry_delete(struct entity_registry *r, entity e) {
  if (!entity_registry_exists(r, e)) {
    return -1;
  }

  const uint32_t idx = entity_get_index(e);
  r->slots[idx] = r->head;
  r->head = e;

  r->count--;

  return 0;
}
