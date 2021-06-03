#include <stdlib.h> /* qsort, malloc, free */
#include <stdio.h>  /* printf */
#include <string.h> /* strlen */
#include <math.h>   /* pow */
#include "hash_table.h"

#define ASCII_SIZE 128

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
    uint n = strlen(txt);
    // printf("%d\n", n);

    triplet *triplets = (triplet*) malloc(n * (sizeof (triplet)));
    for (int i = 0; i < n; i++) {
        triplets[i].lex_i = (int)txt[i];
        triplets[i].lex_ip2ek = 0;
        triplets[i].i = i;
    }

    qsort(triplets, n, sizeof(*triplets), comp);

    int k = 0;
    uint p2k = pow(2, k);
    while (p2k < n){
        int *rank = create_rank(triplets, n);
        for (int i = 0; i < n; i++){
            triplets[i].lex_i = rank[i];
            triplets[i].lex_ip2ek = rank[(int)(i + pow(2, k)) % n];
            triplets[i].i = i;
        }
        qsort(triplets, n, sizeof(*triplets), comp);
        k++;
        p2k = pow(2, k);
    }

    int *suffix_array = (int *) malloc(n * (sizeof (int)));
    for (int i = 0; i < n; i++) {
        suffix_array[i] = triplets[i].i;
    }
    return suffix_array;
}

char *txt2bwt(char *txt) {
    uint n = strlen(txt);
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

uint get_alphabet_size(char *str) {
    char contains[256];
    for (uint i=0; i<256; i++) {contains[i] = 0;}
    for (uint i=0; i<strlen(str); i++) {
        if (contains[str[i]] == 0) {
            contains[str[i]] = 1;
        }
    }
    uint asize = 0;
    for (uint i=0; i<256; i++) {
        if (contains[i] == 1) {
            asize++;
        }
    }
    return asize;
}

void print_array(char *array, uint n) {
    for (uint i=0; i<n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void uint_array_initialize(uint *array, uint n) {
    for (uint i=0; i<n ; i++) {
        array[i] = 0;
    }
}

char *get_ctoidx();
uint *get_first();
uint *get_ranks();


char *bwt2txt(char *bwt) {
    char char2index[ASCII_SIZE];
    for (uint i=0; i<ASCII_SIZE; i++) { char2index[i] = 0; }
    for (uint i=0; i<strlen(bwt); i++) {
        if (char2index[bwt[i]] == 0) {
            char2index[bwt[i]] = 1;
        }
    }
    char n = 0;
    for (uint i=0; i<ASCII_SIZE; i++) {
        n += char2index[i];
        if (char2index[i] == 1) {
            char2index[i] = n-1;
        }
    }

//    uint first[n+1];
//    uint_array_initialize(first, n+1);
//
//    for (uint i=0; i<strlen(bwt); i++) {
//        printf("%d ", char2index[bwt[i]]);
//        char c = char2index[bwt[i]];
//        first[c+1]++;
//    }
//    printf("\n");
//
//    for (uint i=0; i<n+1; i++) {
//        printf("%d ", first[i]);
//    }
//    printf("\n");

//    uint l = strlen(bwt);
//    uint rank[strlen(bwt)];
//    uint_array_initialize(rank, l);

    char ctoidx[128];                   // char2index
    // char idxtoc[3] = {'$', 'a', 'b'};
    // uint first[4] = {0, 1, 3, 2};   // these should be prefix sums
    uint first_cumsum[4] = {0, 1, 4, 6};
    uint rank[6] = {0, 1, 0, 0, 1, 2};

    char *txt = (char*) malloc(strlen(bwt) * sizeof (*txt));
    char next_char = '$';
    uint cur_pos = 0;

    for (uint i=0; i < strlen(bwt); i++) {
        txt[strlen(bwt) - 1 - i] = next_char;
        cur_pos = first_cumsum[char2index[next_char]] + rank[cur_pos];
        next_char = bwt[cur_pos];
    }

    return txt;
}

void print_suffix_array(int *suffix_array, uint n) {
    for (uint i=0; i < n; i++) {
        printf("%d ", suffix_array[i]);
    }
    printf("\n");
}

int main(void) {
    char original[] = "abaab$";
    printf("original:\t%s\n", original);

    int *suffix_array = create_suffix_array(original);
    print_suffix_array(suffix_array, strlen(original));

    char *bwt = txt2bwt(original);
    printf("bwt:\t%s\n", bwt);

    char *reconstructed = bwt2txt(bwt);
    printf("reconstructed:\t%s\n", reconstructed);

    return 0;
}
