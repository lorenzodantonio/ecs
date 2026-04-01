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
  float vx, vy;
} Velocity;

size_t position_id;
size_t velocity_id;

void initialize(struct storage *storage) {
  position_id = storage_register_component(storage, sizeof(Position));
  velocity_id = storage_register_component(storage, sizeof(Velocity));
}

void update_position(struct storage *storage, float dt) {
  (void)dt;
  struct component_pool *pool = storage_get_pool(storage, position_id);

  for (size_t i = 0; i < pool->sparse_set.count; i++) {
    Position *pos = component_pool_get_by_position(pool, i);
    pos->x++;
    pos->y--;
  }
}

void update_position_and_velocity(struct storage *storage, float dt) {
  (void)dt;

  struct iterator iter;
  storage_iterator_init(storage, &iter, 2,
                        (size_t[]){position_id, velocity_id});

  while (iterator_next(&iter)) {
    Position *pos = iter.data[position_id];
    Velocity *vel = iter.data[velocity_id];
    pos->y += vel->vy;
    vel->vy--;
  }
}

int main(void) {
  struct storage *storage = storage_new(16);
  initialize(storage);

  size_t id0 = storage_create_entity(storage);
  size_t id1 = storage_create_entity(storage);

  struct component_pool *velocities = storage_get_pool(storage, velocity_id);

  Velocity *vel1 = component_pool_emplace(velocities, id0);
  vel1->vx = 12.0f;
  vel1->vy = 23.0f;

  Velocity *vel2 = component_pool_emplace(velocities, id1);
  vel2->vx = 13.0f;
  vel2->vy = 24.0f;

  size_t id2 = storage_create_entity(storage);
  size_t id3 = storage_create_entity(storage);
  struct component_pool *positions = storage_get_pool(storage, position_id);

  Position *pos1 = component_pool_emplace(positions, id2);
  pos1->x = 3;
  pos1->y = 4;

  Position *pos2 = component_pool_emplace(positions, id3);
  pos2->x = 23;
  pos2->y = 43;

  for (size_t i = 0; i < 10; i++) {
    update_position(storage, 0);
    update_position_and_velocity(storage, 0);
  }
  component_registry_print(storage->components);
  storage_delete_entity(storage, id2);

  component_registry_print(storage->components);
  fflush(stdout);

  storage_free(storage);
  return 0;
}
