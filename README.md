# ECS
![Status](https://img.shields.io/badge/status-experimental-orange)

## Overview
ECS-based data management library

## Entity
An entity is a unique identifier composed of an index in the least significant bits and a version in the remaining 12.
When an entity is deleted its index is listed for reuse, but upon recycling the version will be increased.

```c
entity foo = storage_create_entity(storage); // index: 0 version: 0
storage_create_entity(storage); // index: 1 version: 0
storage_delete_entity(storage,  foo);
storage_create_entity(storage); // index: 0 version: 1
storage_create_entity(storage); // index: 2 version: 0
```

The maximum number of active entities is 1.048.575 (2^20 - 1), each index can be recycled up to 4,096 times;

## Component
Any C data type can be a component, registering it returns a component pool.
```c
typedef struct {
    int x;
    int y;
} Position;

int main(void) {
    struct storage *storage = storage_new();
    struct component_pool *positions = storage_register_component(storage, sizeof(Position));
    return 0;
}
```

Each component is associated with a pool which stores the component data in a contiguous array; a sparse set maps entities to component data.

## Single component iteration
```c
...
void reset_position(struct component_pool *positions) {
  for (size_t i = positions->entities.count; i-- > 0; ) {
    Position *pos = component_pool_get_by_position(positions, i);

    if (pos->x >= 600) {
      pos->x = 0;
    }
    if (pos->y >= 600) {
      pos->y = 0;
    }
  }
}
...
```

## Iterator/View: Iterate through multiple pools
An iterator can be instantiated in order to scan multiple component pools looking for entity data.
```c
...
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
```
