#include "ecs/storage.h"
#include <assert.h>

void storage_new__succeeds(void) {
  struct storage *s = storage_new();
  assert(s != NULL);
  storage_free(s);
}

int main(void) {
  storage_new__succeeds();
  return 0;
}
