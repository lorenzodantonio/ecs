#ifndef TEST_ENTITY_REGISTRY_H
#define TEST_ENTITY_REGISTRY_H

void entity_registry_new__succeeds(void);

void entity_registry_delete__succeeds(void);
void entity_registry_delete__fails_if_entity_does_not_exist(void);
void entity_registry_delete__fails_if_entity_already_deleted(void);

void entity_registry_next__succeeds(void);
void entity_registry_next__reuse_last_index_deleted(void);

void entity_registry_exists__succeeds(void);
void entity_registry_exists__fails(void);

#endif
