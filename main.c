#include "component_registry.h"
#include "entity_registry.h"
#include "storage.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int x;
  int y;
} Position;

typedef struct {
  int vx, vy;
} Velocity;

size_t position_id;
size_t velocity_id;

void initialize(struct storage *storage) {
  position_id = storage_register_component(storage, sizeof(Position));
  velocity_id = storage_register_component(storage, sizeof(Velocity));
}

void reset_position(struct storage *storage) {
  struct component_pool *pool = storage_get_pool(storage, position_id);

  for (size_t i = 0; i < pool->sparse_set.count; i++) {
    Position *pos = component_pool_get_by_position(pool, i);

    if (pos->x >= 600) {
      pos->x = 0;
    }
    if (pos->y >= 600) {
      pos->y = 0;
    }
  }
}

void move(struct storage *storage, float dt) {
  struct iterator iter;
  storage_iterator_init(storage, &iter, 2,
                        (size_t[]){position_id, velocity_id});

  while (iterator_next(&iter)) {
    Position *pos = iter.data[position_id];
    Velocity *vel = iter.data[velocity_id];

    pos->x += vel->vx * dt;
    pos->y += vel->vy * dt;
  }
}

int main(void) {
  struct storage *storage = storage_new();
  initialize(storage);

  entity id0 = storage_create_entity(storage);
  entity id1 = storage_create_entity(storage);

  struct component_pool *velocities = storage_get_pool(storage, velocity_id);

  Velocity *v0 = storage_component_emplace(velocities, id0);
  v0->vx = 8;
  v0->vy = 8;

  Velocity *v1 = storage_component_emplace(velocities, id1);
  v1->vx = 4;
  v1->vy = 4;

  struct component_pool *positions = storage_get_pool(storage, position_id);

  Position *p0 = storage_component_emplace(positions, id0);
  p0->x = 0;
  p0->y = 0;

  Position *p1 = storage_component_emplace(positions, id1);
  p1->x = 64;
  p1->y = 64;

  for (size_t i = 0; i < 1000; i++) {
    reset_position(storage);
    move(storage, 1);
  }

  storage_free(storage);
  return 0;
}
