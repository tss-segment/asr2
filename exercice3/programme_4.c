#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <bits/types.h>

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Error : missing argument\n");
		return 1;
	}

	/* https://www.guyrutenberg.com/2007/09/03/seeding-srand/ */
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);

	double borne = 2 * atof(argv[1]);
	double val;

	while(scanf("%lf", &val) != EOF)
	{
		val += (rand() / (double)RAND_MAX - 0.5) * borne;
		printf("%.1f\n", val);
	}

	return 0;
}
