#include <stdio.h>

// Ecrit sur la sortie standard l'entree standard en supprimant tous les espaces et les tab.
int main() {
	char c;
	while (!feof(stdin)) {
		scanf("%c", &c);
		if (c!=' ' && c!='\t')
			printf("%c", c);
	}
	return 0;
}
