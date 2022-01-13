for numtest in 1 2 3 4
do
	echo Test de example$numtest.o
    NS=$( ./main Examples_loader/example$numtest.o -NS )
    i=0
    while [ $i -lt $NS ]
    do
        readelf -x $i Examples_loader/example$numtest.o | ./supprime_espaces > base
        ./main Examples_loader/example$numtest.o $i | ./supprime_espaces > test
        diff test base
        rm base test
        i=$( expr $i + 1 )
    done
done
