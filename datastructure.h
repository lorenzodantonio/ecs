
#pragma once

#include <stdlib.h>

struct sparse_set {
  size_t *sparse;
  size_t *dense;
  size_t count;
  size_t capacity;
};

int sparse_set_init(struct sparse_set *set, size_t capacity);
struct sparse_set *sparse_set_new(size_t capacity);
int sparse_set_push(struct sparse_set *set, size_t entity);
int sparse_set_remove(struct sparse_set *set, size_t entity);
void sparse_set_free(struct sparse_set *set);

void sparse_set_print(struct sparse_set set);
