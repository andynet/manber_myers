#include <stdio.h>
#include "hash_table.h"

int main(void) {
    ht_table_t *table = create_table(19);
    
    insert(table, "a", 10);
    insert(table, "a", 20);
    insert(table, "01", 15);
    insert(table, "10", 6); 
    insert(table, "d", 4); 
    insert(table, "ls", 7); 
    insert(table, "b", 9); 
    print_map(table);

    ht_item_t *item = search(table, "01");
    print_item(item);
    printf("%d\n", ht_item_get_value(item));
    item = search(table, "b");
    print_item(item);

    free_table(table);
 
    return 0;
}
