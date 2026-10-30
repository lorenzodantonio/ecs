#include "ecs/join.h"
#include <assert.h>

void join_init(struct join *j, size_t component_count,
               struct component_pool **pools) {
  assert(component_count > 0);

  size_t i = 0;
  struct component_pool *leader = pools[i];

  for (i++; i < component_count; i++) {
    if (pools[i]->entities.count < leader->entities.count) {
      j->followers[i - 1] = leader;
      leader = pools[i];
    } else {
      j->followers[i - 1] = pools[i];
    }
  }
  j->leader = leader;
  j->entity = INVALID_ENTITY;
  j->cursor = leader->entities.count;
  j->component_count = component_count;
}
