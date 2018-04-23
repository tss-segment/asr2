#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Error : missing argument\n");
	}

	else {

		srand(time(NULL));

		int borne = atoi(argv[1]);
		int c = getchar();
		float val = 0.0;
		float val2 = 0.0;
		int compte = 0;

		while (c != EOF) {

			val = 0.0;
			val2 = 0.0;
			compte = 0;

			while (c != '.') {

				val = 10.0*val + (float) (c - 48);
				c = getchar();

			}

			c = getchar();

			while (c != '\n') {

				val2 = 10.0*val2 + (float) (c - 48);
				compte++;

				c = getchar();

			}

			for (int i = 0; i < compte; i++) {
				val2 /= 10.0;
			}

			val += val2;
			val += ( rand()/(double)RAND_MAX ) * borne - (borne/2.0);

			printf("%f\n", val);

			c = getchar();

		}

	}

	return 0;

}
