#include "ecs/component_registry.h"
#include <assert.h>

void component_registry_new__succeeds(void) {
  struct component_registry registry;
  component_registry_init(&registry);
  assert(registry.count == 0);
}

void component_registry_add__succeeds(void) {
  struct component_registry registry;
  component_registry_init(&registry);
  
  const size_t component_size = sizeof(struct x { int y; });
  const size_t max_entities = 16;
  struct component_pool *pool =
      component_registry_add(&registry, component_size, max_entities);

  assert(pool->id == 0);
  assert(pool->component_size == component_size);
  assert(pool->entities.capacity == max_entities);
}

int main(void) {
  component_registry_new__succeeds();
  component_registry_add__succeeds();

  return 0; // 0 = PASSED per CTest
}
