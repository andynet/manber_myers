#include <stdlib.h> /* qsort, malloc, free */
#include <stdio.h>  /* printf */
#include <string.h> /* strlen */
#include <math.h>   /* pow */
#include "hash_table.h"

typedef struct {
    int lex_i;
    int lex_ip2ek;  /* i plus 2 to the power of k */
    int i;
} triplet;

int comp(const void * e1, const void * e2) {
    int e1_lex_i = ((triplet*)e1)->lex_i;
    int e2_lex_i = ((triplet*)e2)->lex_i;
    if (e1_lex_i > e2_lex_i) {return  1;}
    if (e1_lex_i < e2_lex_i) {return -1;}

    if (((triplet*)e1)->lex_ip2ek > ((triplet*)e2)->lex_ip2ek) {return  1;}
    if (((triplet*)e1)->lex_ip2ek < ((triplet*)e2)->lex_ip2ek) {return -1;}

    return 0;
}

int *create_rank(triplet *triplets, int n) {
    int c = 0;
    int *rank = (int*) malloc(n * (sizeof (int)));
    for (int i = 1; i < n; i++){
        if (
            triplets[i].lex_i != triplets[i-1].lex_i 
            || triplets[i].lex_ip2ek != triplets[i-1].lex_ip2ek
        ) {
            c++;
        }
        rank[triplets[i].i] = c;
    }
    return rank;
}

int *create_suffix_array(char *txt) {
    int n = strlen(txt);
    printf("%d\n", n);

    triplet *triplets = (triplet*) malloc(n * (sizeof (triplet)));
    for (int i = 0; i < n; i++) {
        triplets[i].lex_i = (int)txt[i];
        triplets[i].lex_ip2ek = 0;
        triplets[i].i = i;
    }

    qsort(triplets, n, sizeof(*triplets), comp);

    int k = 0;
    while ((int)pow(2, k) < n){
        int *rank = create_rank(triplets, n);
        for (int i = 0; i < n; i++){
            triplets[i].lex_i = rank[i];
            triplets[i].lex_ip2ek = rank[(int)(i + pow(2, k)) % n];
            triplets[i].i = i;
        }
        qsort(triplets, n, sizeof(*triplets), comp);
        k++;
    }

    int *suffix_array = (int *) malloc(n * (sizeof (int)));
    for (int i = 0; i < n; i++) {
        suffix_array[i] = triplets[i].i;
    }
    return suffix_array;
}

char *txt2bwt(char *txt) {
    int n = strlen(txt);
    int *suffix_array = create_suffix_array(txt);
    char *bwt = (char*) malloc(n * sizeof (char));
    int i;
    for (i = 0; i < n; i++) {
        bwt[i] = txt[(n + (suffix_array[i] - 1)) % n];
    }
    /* bwt[i] = '\0'; */
    return bwt;
}

ht_table_t *construct_first(char *bwt) {
    ht_table_t *table = create_table(5);
    ht_item_t *tmp;
    char key[2];
    int value;

    key[1] = '\0';
    for (int i = 0; i < strlen(bwt); i++) {
        key[0] = bwt[i];
        tmp = search(table, key);
        if (tmp == NULL) {
            insert(table, key, 1);
        } else {
            value = ht_item_get_value(tmp);
            value++;
            insert(table, key, value);
        }
    }
    return table;
}

int main(void) {
    char original[] = "abaab$";

    printf("original:\t%s\n", original);
    int *suffix_array = create_suffix_array(original);

    for (int i=0; i < strlen(original); i++) {
        printf("%d ", suffix_array[i]);
    }
    printf("\n");

    char *bwt = txt2bwt(original);
    printf("bwt:\t%s\n", bwt);

    ht_table_t *table = construct_first(bwt);
    print_map(table);
    /* 
    char *reconstructed = bwt2txt(bwt);
    printf("reconstructed:\t%s\n", reconstructed);
    */

    return 0;
}
