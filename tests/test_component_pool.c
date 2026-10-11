#include "ecs/component_pool.h"
#include <assert.h>

void component_pool_init__succeeds(void) {
    struct component_pool p;
    component_pool_init(&p, 0, sizeof(int), 8);

    assert(p.id == 0);
    assert(p.component_size == sizeof(int));
    assert(p.entities.capacity == 8);

    component_pool_deinit(&p);
}

void component_pool_init__fails_when_size_is_zero(void) {
    struct component_pool p;
    int res = component_pool_init(&p, 0, 0, 8);
    assert(res == -1);
}

void component_pool_emplace__succeds(void) {
    struct component_pool p;

    const uint32_t init_cap = 8;
    component_pool_init(&p, 0, sizeof(int), init_cap);

    entity e;

    for (size_t i = 0; i < init_cap; i++) {
        e = entity_new(i, 0);
        component_pool_emplace(&p, e);
        assert(p.entities.count == i + 1);
    }

    e = entity_new(init_cap + 1, 0);
    component_pool_emplace(&p, e);
    assert(p.entities.capacity == init_cap * 2);
    assert(p.entities.count == (init_cap + 1));

    component_pool_deinit(&p);
}

int main(void) {
    component_pool_init__succeeds();
    component_pool_init__fails_when_size_is_zero();

    component_pool_emplace__succeds();

    return 0;
}