#include <stdio.h>
#include <stdlib.h>






int main() {

	int code_ascii = getchar();
	/* Mieux vaut tard que jamais : j'ai appris que le pipe ne plaçait pas l'argument dans argv */
	/* On passe donc par getchar() pour lire l'entrée via echo */

	while (code_ascii != EOF) {

		for (int j = 6; j >= 0; j--) {

			if (code_ascii >= (1 << j)) {
				code_ascii -= (1 << j);
				printf("1");
			}

			else {
				printf("0");
			}

		}

		code_ascii = getchar();

	}

	printf("\n");

	return 0;

}
