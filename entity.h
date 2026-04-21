#pragma once

#include <stdint.h>

typedef uint32_t entity;

#define ENTITY_INDEX_BITS 20

#define ENTITY_PACK(index, version)                                            \
  ((entity)(((version) << ENTITY_INDEX_BITS) | ((index) & 0xFFFFF)))

enum {
  ENTITY_IDX_MASK = 0xFFFFF,
  ENTITY_VER_MASK = 0xFFF,
  INVALID_ENTITY = ENTITY_PACK(ENTITY_IDX_MASK, 0xFFF),
};

static inline entity entity_new(uint32_t index, uint32_t version) {
  return ENTITY_PACK(index, version);
}

static inline uint32_t entity_get_index(entity e) {
  return e & ENTITY_IDX_MASK;
}

static inline uint32_t entity_get_version(entity e) {
  return e >> ENTITY_INDEX_BITS;
}
