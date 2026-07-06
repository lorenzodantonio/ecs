#ifndef TEST_DATASTRUCTURE_H
#define TEST_DATASTRUCTURE_H

void sparse_set_new__succeeds(void);

void sparse_set_push__succeds(void);
void sparse_set_push__fails_if_element_already_exists(void);

void sparse_set_remove__succeeds_with_last_element(void);
void sparse_set_remove__swaps_last_dense_array_element(void);
void sparse_set_remove__fails_if_element_does_not_exist(void);

#endif
