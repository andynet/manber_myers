#include <stdlib.h>
#include "hash_table.h"

int
main(void)
{
    hash_set_t *set = hash_set_create(6);
    hash_set_print(set);
    hash_set_free(set);
    set = NULL;
    hash_set_print(set);
}
