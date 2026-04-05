#pragma once

#include "datastructure.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t entity;

#define ENTITY_INDEX_BITS 20

#define ENTITY_PACK(index, version)                                            \
  ((entity)(((version) << ENTITY_INDEX_BITS) | ((index) & 0xFFFFF)))

enum {
  INVALID_ENTITY_IDX = 0xFFFFF,
  INVALID_ENTITY_VER = 0xFFF,
  INVALID_ENTITY = ENTITY_PACK(INVALID_ENTITY_IDX, INVALID_ENTITY_VER),
};

static inline entity entity_new(uint32_t index, uint32_t version) {
  return ENTITY_PACK(index, version);
}

static inline uint32_t entity_get_index(entity e) {
  return e & INVALID_ENTITY_IDX;
};

static inline uint32_t entity_get_version(entity e) {
  return e >> ENTITY_INDEX_BITS;
};

struct entity_registry {
  entity head;
  size_t cursor;
  size_t capacity;
  uint32_t items[INVALID_ENTITY_IDX];
};

struct entity_registry *entity_registry_new();
void entity_registry_free(struct entity_registry *registry);

int entity_registry_exists(struct entity_registry *registry, entity e);

entity entity_registry_next(struct entity_registry *registry);
int entity_registry_delete(struct entity_registry *registry, entity e);
