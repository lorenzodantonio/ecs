
#pragma once

#include "entity_registry.h"
#include <stdlib.h>

struct sparse_set {
  size_t *sparse;
  entity *dense;
  size_t count;
  size_t capacity;
};

int sparse_set_init(struct sparse_set *set, size_t capacity);
struct sparse_set *sparse_set_new(size_t capacity);
int sparse_set_push(struct sparse_set *set, entity e);
int sparse_set_remove(struct sparse_set *set, entity e);
void sparse_set_free(struct sparse_set *set);
