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

void reset_position(struct component_pool *positions) {
  for (size_t i = 0; i < positions->entities.count; i++) {
    Position *pos = component_pool_get_by_position(positions, i);

    if (pos->x >= 600) {
      pos->x = 0;
    }
    if (pos->y >= 600) {
      pos->y = 0;
    }
  }
}

void move(struct component_pool *positions, struct component_pool *velocities,
          float dt) {
  struct iterator iter;
  iterator_init(&iter, 2, (struct component_pool *[]){positions, velocities});

  while (iterator_next(&iter)) {
    Position *pos = iterator_get_field(&iter, positions);
    Velocity *vel = iterator_get_field(&iter, velocities);

    pos->x += vel->vx * dt;
    pos->y += vel->vy * dt;
  }
}

int main(void) {
  struct storage *storage = storage_new();

  struct component_pool *positions =
      storage_register_component(storage, sizeof(Position));
  struct component_pool *velocities =
      storage_register_component(storage, sizeof(Velocity));

  entity id0 = storage_create_entity(storage);
  entity id1 = storage_create_entity(storage);

  Velocity *v0 = component_pool_emplace(velocities, id0);
  v0->vx = 8;
  v0->vy = 8;

  Velocity *v1 = component_pool_emplace(velocities, id1);
  v1->vx = 4;
  v1->vy = 4;

  Position *p0 = component_pool_emplace(positions, id0);
  p0->x = 0;
  p0->y = 0;

  Position *p1 = component_pool_emplace(positions, id1);
  p1->x = 64;
  p1->y = 64;

  for (size_t i = 0; i < 1000; i++) {
    reset_position(positions);
    move(positions, velocities, 1);
  }

  storage_free(storage);
  return 0;
}
