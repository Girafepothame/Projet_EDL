for numtest in 1 2 3
do
	echo Test de example$numtest.o
	readelf -s Examples_loader/example$numtest.o | ./supprime_espaces > base
	./lecture_elf -s Examples_loader/example$numtest.o | ./supprime_espaces > test
	diff base test
	rm base test
done


