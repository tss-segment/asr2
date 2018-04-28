#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* float*float (I,Q) : module = amplitude; arg = phase */

int main(int argc, char const *argv[]) {
        /* On fixe l'amplitude à 1 et la fréquence à 10 */
        /* Le temps incrémente de 1 à chaque nouveau complexe envoyé */
        /* Donc l'argument en 2*pi*f*t est toujours multiple de 2*pi */
        /** Donc : si le bit est pair, on envoie 0.0, -1.0 (z = -i)  **/
        /** Sinon, on envoie  1.0, 0.0 (z = 1) **/

        int ascii = 1;

        while ( (ascii = getchar()) && ascii != EOF) {
                if (ascii == '0' ) {
                        /* le ascii de 0 est pair, celui de 1 impair */
                        printf("%f\n%f\n", 0.0, -1.0);
                } else if (ascii == '1') {
                        printf("%f\n%f\n", 1.0, 0.0);
                }
                /* else : ignore */
        }
        return 0;
}
