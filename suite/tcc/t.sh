
tcc -w -nostdlib -E test/t.c -o test/t.pp.c
tcc -w -nostdlib -r test/t.c -o test/t.a

#  js pp
tcc -w -nostdlib -nostdinc -E -x c test/tpp.js -o test/tpp.o.js