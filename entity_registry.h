#pragma once

#include "datastructure.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t entity;

static inline entity entity_new(uint32_t index, uint32_t version) {
  return (entity)(version << 20 | (index & 0xFFFFF));
}

static inline uint32_t entity_get_index(entity e) {
  return ~(0xFFF << 20) & e;
};

static inline uint32_t entity_get_version(entity e) { return e >> 20; };

enum { EMPTY_ENTITY = 0xFFFFFFFF };

struct entity_registry {
  entity head;
  size_t cursor;
  size_t capacity;
  uint32_t *items;
};

struct entity_registry *entity_registry_new(size_t capacity);
void entity_registry_free(struct entity_registry *registry);

int entity_registry_exists(struct entity_registry *registry, entity e);

entity entity_registry_next(struct entity_registry *registry);
int entity_registry_delete(struct entity_registry *registry, entity e);
