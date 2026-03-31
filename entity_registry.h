#pragma once

#include "datastructure.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct entity_registry {
  size_t head;
  size_t capacity;
  size_t *items;

  size_t cursor;
};

struct entity_registry *entity_registry_new(size_t capacity);
void entity_registry_free(struct entity_registry *registry);

int entity_registry_exists(struct entity_registry *registry, size_t entity);

size_t entity_registry_next(struct entity_registry *registry);
int entity_registry_delete(struct entity_registry *registry, size_t entity);
