# Commands

## Compile
```
$ make all
```

## Square Attack on 4-round AES
```
$ ./main_4r.o "ciphertexts_4r.txt"
```

## Square Attack on 5-round AES
```
$ ./main_5r.o "ciphertexts_5r.txt"
```

## Generate 2 lambda-sets for 4-round AES
```
$ ./main_gen.o "plaintexts" 2 4 "plaintexts.txt" 
```

## Generate ciphertexts for 2 lambda-sets with 4-round AES
```
$ ./main_gen.o "ciphertexts" 2 4 "ciphertexts_4r.txt" "plaintexts.txt" "key.txt"
```

## Generate 6 lambda-sets for 5-round AES
```
$ ./main_gen.o "plaintexts" 6 5 "plaintexts.txt" 
```

## Generate ciphertexts for 6 lambda-sets with 5-round AES
```
$ ./main_gen.o "ciphertexts" 6 5 "ciphertexts_5r.txt" "plaintexts.txt" "key.txt"
```
