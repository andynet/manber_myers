#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

/* 
https://www.journaldev.com/35238/hash-table-in-c-plus-plus
https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm
*/

struct ht_item_t {
    char *key;
    int value;
};

struct ht_table_t {
    ht_item_t **items;
    int size;
    int count;
};

int get_hash(char *key, int size) {
    int hash = 0;
    for (int i=0; key[i]; i++) {
        hash += key[i];
    }
    return hash % size;
}

ht_item_t *create_item(char *key, int value) {
    ht_item_t *item = (ht_item_t*) malloc(sizeof (ht_item_t));
    item->key = (char*) malloc(strlen(key) + 1);

    strcpy(item->key, key);
    item->value = value;

    return item;
}

void free_item(ht_item_t *item) {
    free(item->key);
    free(item);
    return;
}

int ht_item_get_value(ht_item_t *item) {
    return item->value;
}

ht_table_t *create_table(int size) {
    ht_table_t *table = (ht_table_t*) malloc(sizeof (ht_table_t));
    table->size = size;
    table->count = 0;
    table->items = (ht_item_t**) calloc(table->size, sizeof (ht_item_t*));
    for (int i=0; i<table->size; i++) {
        table->items[i] = NULL;
    }
    return table;
}

void free_table(ht_table_t *table) {
    for (int i=0; i<table->size; i++){
        if (table->items[i] != NULL) {
            free_item(table->items[i]);
        }
    }
    free(table->items);
    free(table);
    return;
}

void print_map(ht_table_t *table) {
    printf("size: %d\n", table->size);
    printf("count: %d\n", table->count);

    for(int i = 0; i<table->size; i++) {
        ht_item_t *item = table->items[i];
        if(item != NULL)
            printf("(%s\t, %d\t)\n", item->key, item->value);
        else
            printf("(\t, \t)\n");
    }
    printf("\n");
}

void insert(ht_table_t *table, char *key, int value) {
    int hash = get_hash(key, table->size);

    ht_item_t *item = table->items[hash];
    while (item != NULL){
        if (strcmp(item->key, key) == 0) {
            item->value = value;
            return;
        } else {
            hash++;
            item = table->items[hash];
        }
    }

    item = create_item(key, value);
    table->items[hash] = item;
    table->count++;
    return;
}

ht_item_t *search(ht_table_t *table, char *key) {
    int hash = get_hash(key, table->size);
    ht_item_t *item = table->items[hash];
    while (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            return item;
        } else {
            hash++;
            item = table->items[hash];
        }
    }
    return NULL;
}

void print_item(ht_item_t *item) {
    if(item != NULL) {
        printf("(%s, %d)\n", item->key, item->value);
    } else {
        printf("(\t, \t)\n");
    } 
}

struct hash_set_t 
{
    char **elements;
    int size;
    int count;
};

hash_set_t *
hash_set_create(int size)
{
    hash_set_t *set = (hash_set_t *) malloc(sizeof (hash_set_t));
    set->size = size;
    set->count = 0;
    set->elements = (char **) calloc(set->size, sizeof (char *));
    for (int i = 0; i < set->size; i++) {
        set->elements[i] = NULL;
    }
    return set;
}

void
element_print(char *element)
{
    if (element == NULL)
        printf("NULL\n");
    else
        printf("%s\n", element);    
}

void
hash_set_print(hash_set_t *set)
{
    if (set != NULL) {
        printf("size: %d\n", set->size);
        printf("count: %d\n", set->count);
        for (int i = 0; i < set->size; i++)
            element_print(set->elements[i]);
    } else {
        printf("Set is NULL\n");
    }
}

void
hash_set_free(hash_set_t *set)
{
    for (int i=0; i < set->size; i++) {
        if (set->elements[i] != NULL) {
            free(set->elements[i]);
        }
    }
    free(set->elements);
    free(set);
    return;
}



























