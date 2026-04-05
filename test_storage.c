#include "test_storage.h"
#include "storage.h"
#include <assert.h>

void storage_new__succeeds(void) {
  struct storage *s = storage_new();
  assert(s->entities != NULL);
  assert(s->components != NULL);
  storage_free(s);
}
