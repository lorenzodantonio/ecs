#include "test_component_registry.h"
#include "component_registry.h"
#include <assert.h>
#include <stdint.h>

void component_registry_new__succeeds(void) {
  struct component_registry *registry = component_registry_new();
  assert(registry->count == 0);
  component_registry_free(registry);
}

void component_registry_add__succeeds(void) {
  struct component_registry *registry = component_registry_new();
  size_t component_size = sizeof(struct x { int y; });
  size_t max_entities = 16;
  int component_id =
      component_registry_add(registry, component_size, max_entities);

  assert(component_id == 0);

  struct component_pool *pool = component_registry_get(registry, component_id);
  assert(pool->component_size == component_size);
  assert(pool->capacity == max_entities);
  component_registry_free(registry);
}
