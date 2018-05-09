#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/* Objectif : débruiter le signal */
/** Pour ce faire : je sais que le signal est soit (1,0) soit (-1,0) **/
/** Je prends la norme 1 comme distance, et je choisis celui le plus proche **/
/* Pourquoi N1 ? Je sais pas, il faudra faire des tests pour savoir si N2 ou */
/* Nsup ne seraient pas meilleures */

int main(int argc, char const *argv[]) {
        /* lis l'entree avec getchar, et renvoie un signal débruité */
        double d_bit_0, d_bit_1;
        double a, b;

        while ( (fscanf(stdin, "%lf\n%lf\n", &a, &b) != EOF) ) {
              d_bit_0 = fabs(-1.0-a);
              d_bit_1 = fabs(1-a);
              if (d_bit_0 < d_bit_1) {
                puts("-1.0\n0.0");
              } else {
                puts("1.0\n0.0");
              }
        }

        return 0;
}
