echo Vous allez lire les sorties du programme et de la fonction readelf ligne par ligne.
echo Vérifiez que les informations sont consistantes. Appuyez sur Entree pour passer à la suite.
declare -a base
declare -a test

for numtest in 1 2 3
do
	echo ""
	echo Test de example$numtest.o :
	readelf -h Examples_loader/example$numtest.o > sortie
	split -d -l1 sortie base-
	./lecture_elf -h Examples_loader/example$numtest.o > sortie
	split -d -l1 sortie test-
	i=0
	s=$( printf "%02d\n" "$i" )
	while [ -e base-$s ]
	do
		cat base-$s 
		cat test-$s 
		read ligne 
		i=$( expr $i + 1 )
		s=$( printf "%02d\n" "$i" )
	done
	rm sortie base-?? test-??
done
