#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** binaire vers ascii . Rappel : les bits de fin du message ne sont **/
/** pas significatifs (on envoie par paquets de 4 mais decode par 7) **/

int main(int argc, char const *argv[]) {
        /* on lit sur stdin l'entree, 7 bits par 7 bits */

        int bits_lus = 1;
        int chiffre_lu;
        int ascii_lu;

        while (bits_lus > 0) {

                /* lecture du ascii à décoder */
                bits_lus = 0;
                while (bits_lus < 7 && (chiffre_lu = getchar() && chiffre_lu != EOF)) {
                        if (chiffre_lu == '0' || chiffre_lu == '1') {
                                ascii_lu |= (chiffre_lu - '0') << (7-bits_lus);
                                /* on a envoyé les bits forts en premier */
                                bits_lus++;
                        }
                }
                if (bits_lus == 7) {
                        printf("%c", ascii_lu);
                }
        }

        /* \n final */
        printf("\n");

        return 0;
}
