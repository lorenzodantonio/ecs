#include "test_entity.h"

#include "entity_registry.h"
#include <assert.h>

void entity_new__succeeds(void) {
  entity e = entity_new(2, 12);
  assert(entity_get_version(e) == 12);
  assert(entity_get_index(e) == 2);
}
