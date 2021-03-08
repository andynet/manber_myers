#ifndef HT_H_
#define HT_H_

typedef struct ht_item_t ht_item_t;
typedef struct ht_table_t ht_table_t;

ht_item_t *create_item(char *key, int value);
void free_item(ht_item_t *item);
void print_item(ht_item_t *item);
int   ht_item_get_value(ht_item_t *item);
/* char *ht_item_get_key(ht_item_t *item); */

ht_table_t *create_table(int size);
void free_table(ht_table_t *table);
void print_map(ht_table_t *table);

void insert(ht_table_t *table, char *key, int value);
ht_item_t *search(ht_table_t *table, char *key);

/* hash_set */
typedef struct hash_set_t hash_set_t;

hash_set_t *hash_set_create(int size);
void        hash_set_print(hash_set_t *set);
void        hash_set_free(hash_set_t *set);
int         hash_set_is_member(hash_set_t *set, char *key);
char      **hash_set_to_array(hash_set_t *set);

#endif	/* HT_H_ */

