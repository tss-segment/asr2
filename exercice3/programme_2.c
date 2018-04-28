#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/* principe du Hamming[8,4] : */
/** https://en.wikipedia.org/wiki/Hamming_code **/
/* checkez le Hamming[7,4] et l'écriture matricielle */
/* j'implémente le 8,4 matriciel */

/** Convention : si le message n'est pas un multiple de 4, je le remplis */
/* avec des 0 **/


int G[4][8] = {
        {1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 0, 0, 1},
        {0, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 1, 0, 0, 1, 0}
};


int main(int argc, char const *argv[]) {
        /* on lit sur stdin l'entree, 4 bits par 4 bits */
        /* on envoie la multiplication matricielle, et on affiche sur stdout */
        /* le résultat */

        int bits[4];
        int msg[8];
        int bits_lus = 1;
        int ascii_lu;

        while (bits_lus > 0) {

                /* lecture des bits à coder */
                bits_lus = 0;
                while (bits_lus < 4 && (ascii_lu = getchar()) && ascii_lu != EOF) {
                        if (ascii_lu == '0' || ascii_lu == '1') {
                                bits[bits_lus] = ascii_lu - '0';
                                bits_lus++;
                        }
                }

                if (bits_lus < 4) {
                        /* alors ajouter des 0 à la fin */
                        for (size_t i = bits_lus-1; i < 4; i++) {
                                bits[i] = 0;
                        }
                }

                if (bits_lus > 0) {
                        /* produit matriciel */
                        for (size_t j = 0; j < 8; j++) {
                                msg[j] = 0;
                                for (size_t k = 0; k < 4; k++) {
                                        msg[j] += bits[k]*G[k][j];
                                }
                        }

                        /* affichage */
                        for (size_t i = 0; i < 8; i++) {
                                printf("%i", (msg[i]%2));
                        }
                }
        }

        /* \n final */
        printf("\n");

        return 0;
}
