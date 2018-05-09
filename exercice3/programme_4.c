#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Error : missing argument\n");
		return 1;
	}

	srand(time(NULL));

	double borne = 2 * atof(argv[1]);
	double val;

	while(scanf("%lf", &val) != EOF)
	{
		val += (rand() / (double)RAND_MAX - 0.5) * borne;
		printf("%.1f\n", val);
	}

	return 0;
}
