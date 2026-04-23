# Storage
![For Learning](https://img.shields.io/badge/purpose-learning-teal)
![Experimental](https://img.shields.io/badge/status-experimental-orange)

## Overview
Data storage based on ECS

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

## Iterator/View: Iterate through multiple pools
An iterator can be instantiated in order to scan multiple component pools looking for entity data.
```c
...
void move(struct component_pool *positions, struct component_pool *velocities,
          float dt) {
  struct iterator iter;
  iterator_init(&iter, 2, (struct component_pool *[]){positions, velocities});

  while (iterator_next(&iter)) {
    Position *pos = iter.data[positions->id];
    Velocity *vel = iter.data[velocities->id];

    pos->x += vel->vx * dt;
    pos->y += vel->vy * dt;
  }
}
```
