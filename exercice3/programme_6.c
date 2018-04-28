#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** programme 5 nous renvoie un BPSK correct, on le passe en bit **/

int main(int argc, char const *argv[]) {
        /* lis l'entree avec fscanf, et renvoie un signal débruité */

        float a, b;

        while ( (fscanf(stdin, "%f\n%f\n", &a, &b) != EOF) ) {
                if (a==1.0 && b==0.0) {
                        printf("%i", 1);
                } else if (a==-1.0 && b==0.0) {
                        printf("%i\n", -1);
                } else {
                        fprintf(stderr, "ERROR_WHILE_PARSING_BPSK");
                }
        }

        printf("\n");
        return 0;
}
