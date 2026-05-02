#include "test_component_registry.h"
#include "component_registry.h"
#include <assert.h>
#include <stdint.h>

void component_registry_new__succeeds(void) {
  struct component_registry registry;
  component_registry_init(&registry);
  assert(registry.count == 0);
}

void component_registry_add__succeeds(void) {
  struct component_registry registry;
  component_registry_init(&registry);
  size_t component_size = sizeof(struct x { int y; });
  size_t max_entities = 16;
  struct component_pool *pool =
      component_registry_add(&registry, component_size, max_entities);

  assert(pool->id == 0);
  assert(pool->component_size == component_size);
  assert(pool->entities.capacity == max_entities);
}
