#include <stdlib.h> /* qsort, malloc, free */
#include <stdio.h>  /* printf */
#include <string.h> /* strlen */
#include <math.h>   /* pow */

#define ASCII_SIZE 128
#define EMPTY (-1)

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

void uint_array_initialize(uint *array, uint n) {
    for (uint i=0; i<n ; i++) {
        array[i] = 0;
    }
}

char *get_ctoidx(char *bwt) {
    char *ctoidx = (char *)malloc(ASCII_SIZE * sizeof (*ctoidx));
    for (uint i=0; i<ASCII_SIZE; i++) { ctoidx[i] = EMPTY; }
    for (uint i=0; i<strlen(bwt); i++) {
        if (ctoidx[bwt[i]] == EMPTY) {
            ctoidx[bwt[i]] = 1;
        }
    }
    char n = 0;
    for (uint i=0; i<ASCII_SIZE; i++) {
        if (ctoidx[i] == 1) {
            ctoidx[i] = n;
            n++;
        }
    }
    return ctoidx;
}

uint *get_first(char *bwt, const char *ctoidx) {
    uint l = get_alphabet_size(bwt);
    uint *first = malloc(sizeof *first * (l+1));
    uint_array_initialize(first, l+1);

    for (uint i=0; i<strlen(bwt); i++) {
        char c = ctoidx[bwt[i]];
        first[c+1]++;
    }

    for (uint i=1; i<l+1; i++) {
        first[i] += first[i-1];
    }
    return first; // {0, 1, 4, 6}
}

char *get_idxtoc(char *bwt, const char *ctoidx) {
    uint l = get_alphabet_size(bwt);
    char *idxtoc = malloc(sizeof *idxtoc * l);
    for (uint i=0; i < ASCII_SIZE; i++) {
        if (ctoidx[i] != EMPTY) {
            idxtoc[ctoidx[i]] = i;
        }
    }
    return idxtoc; // {'$', 'a', 'b'}
}

uint *get_ranks(char *bwt, char *ctoidx) {
    uint n = strlen(bwt);
    uint *ranks = malloc(sizeof *ranks * n);
    uint_array_initialize(ranks, n);
    char *idxtoc = get_idxtoc(bwt, ctoidx);

    for (uint i=0; i<get_alphabet_size(bwt); i++) {
        uint rank = 0;
        for (uint j=0; j<n; j++) {
            if (bwt[j] == idxtoc[i]) {
                ranks[j] = rank;
                rank++;
            }
        }
    }
    return ranks; // {0, 1, 0, 0, 1, 2}
};


char *bwt2txt(char *bwt) {
    char *ctoidx = get_ctoidx(bwt);
    uint *first = get_first(bwt, ctoidx);
    uint *ranks = get_ranks(bwt, ctoidx);

    char *txt = (char*) malloc(strlen(bwt) * sizeof (*txt));
    char next_char = '$';
    uint cur_pos = 0;

    for (uint i=0; i < strlen(bwt); i++) {
        txt[strlen(bwt) - 1 - i] = next_char;
        cur_pos = first[ctoidx[next_char]] + ranks[cur_pos];
        next_char = bwt[cur_pos];
    }
    return txt;
}

void print_suffix_array(int *suffix_array, uint n) {
    printf("suffix array:\t");
    for (uint i=0; i < n; i++) {
        printf("%d ", suffix_array[i]);
    }
    printf("\n");
}

int main(void) {
    char original[] = "abaababbabbababbabbabshshhhsnajkahb$";
    printf("original:\t%s\n", original);

    int *suffix_array = create_suffix_array(original);
    print_suffix_array(suffix_array, strlen(original));

    char *bwt = txt2bwt(original);
    printf("bwt:\t%s\n", bwt);

    char *reconstructed = bwt2txt(bwt);
    printf("reconstructed:\t%s\n", reconstructed);

    return 0;
}
