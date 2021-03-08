all: bwt

clean:
	rm -f test_ht hash_table hash_set_test bwt

bwt: src/bwt.c
	gcc -g src/bwt.c src/hash_table.c -o build/bwt -lm

hash_set_test: src/hash_set_test.c src/hash_table.c
	gcc -g hash_set_test.c hash_table.c -o hash_set_test
	./hash_set_test

test_ht: src/test_ht.c src/hash_table.c
	gcc -g test_ht.c hash_table.c -o test_ht
	./test_ht

test: bwt
	./bwt

