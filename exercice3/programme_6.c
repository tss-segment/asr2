#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** programme 5 nous renvoie un BPSK correct, on le passe en bit **/

int main(void) {
        /* lis l'entree avec fscanf, et renvoie un signal débruité */

        int a, b;

        while ( (fscanf(stdin, "%d.0\n%d.0\n", &a, &b) != EOF) ) {
                if (a==1 && b==0) {
                        printf("%i", 1);
                } else if (a==-1 && b==0) {
                        printf("%i", 0);
                } else {
                        fprintf(stderr, "ERROR_WHILE_PARSING_BPSK");
                }
        }

        printf("\n");
        return 0;
}
