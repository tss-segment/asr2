#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Je vais colorer la sortie pour indiquer les erreurs/corrections */
/* https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c */
#ifdef USE_COLOR
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE  "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN  "\x1b[36m"
#define RESET "\x1b[0m"
#else
#define RED     ""
#define GREEN   ""
#define YELLOW  ""
#define BLUE    ""
#define MAGENTA ""
#define CYAN    ""
#define RESET   ""
#endif

/* code couleur : vert si pas d'erreur, jaune si corrigé, rouge si double erreur */

/** Hamming(8,4) corrrige 1 erreur, et détecte 2 **/
/** Dans le cadre de l'exo, je mettrai juste un avertissement si on détecte
   deux erreurs **/

int H[4][8] = {
        {1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1}
};


void decodage(int bits[8], int syndrome[4]) {
        /* on a fait la multiplication de matrice pour decoder */
        /* si son resultat est nul (pour les trois premiers), c'est bon */
        /* si un des trois premeirs elements du produit est non-nul : */
        /* -si le bit de parité confirme un nombre impair d'erreur, on corrige */
        /*    (oui, on risque la triple erreur) */
        /* -si le bit de parité confirme un nombre pair d'erreur, on devrait */
        /* demande rune retransmission. Ici on écrira juste en rouge */

        /* nb : si le syndrome est (0,0,0,1), on suppose que c'est le bit de parite qui est faux */

        int parite = syndrome[3];

        int est_nul = !(syndrome[0] | syndrome[1] | syndrome[2]);

        if (est_nul) {
                /* alors tout va pour le mieux dans le meilleur des mondes */
                /* donc on écrit en GREEN */
                printf(GREEN "%d" RESET, bits[2]);
                printf(GREEN "%d" RESET, bits[4]);
                printf(GREEN "%d" RESET, bits[5]);
                printf(GREEN "%d" RESET, bits[6]);
        } else if (!est_nul && parite ) {
                /* alors on corrige 1 erreur */
                int position = 0;
                for (size_t i = 0; i < 3; i++) {
                        position |= syndrome[i] << i;
                }
                bits[position] ^= 1;

                /* on a fait une correction, donc YELLOW */
                printf(YELLOW "%d" RESET, bits[2]);
                printf(YELLOW "%d" RESET, bits[4]);
                printf(YELLOW "%d" RESET, bits[5]);
                printf(YELLOW "%d" RESET, bits[6]);
        } else {
                /* alors on a un nombre pair d'erreur, et on panique */
                /* mais dans le cadre de ce projet, on va juste mettre du RED */
                printf(RED "%d" RESET, bits[2]);
                printf(RED "%d" RESET, bits[4]);
                printf(RED "%d" RESET, bits[5]);
                printf(RED "%d" RESET, bits[6]);
        }

        return;
}


int main(void) {
        /* capte l'entree sur stdin, et decode hamming */
        /** https://fr.wikipedia.org/wiki/Code_de_Hamming_(7,4)#Code_de_Hamming_(8,4) **/

        int bits[8];
        int syndrome[4];
        int bits_lus = 1;
        int chiffre_lu;

        while (bits_lus > 0) {

                /* lecture des bits à décoder */
                bits_lus = 0;
                while (bits_lus < 8 && (chiffre_lu = getchar()) != EOF) {
                        if (chiffre_lu == '0' || chiffre_lu == '1') {
                                bits[bits_lus] = chiffre_lu - '0';
                                bits_lus++;
                        }
                }

                if (bits_lus > 0 && bits_lus < 8) {
                        fprintf(stderr, RED "\nAttention : nombre incorrect "
                                "de bits reçu (%d), erreur probable !" RESET,
                                bits_lus);
                        break;
                }

                /* produit matriciel */
                for (size_t i = 0; i < 4; i++) {
                        syndrome[i] = 0;
                        for (size_t k = 0; k < 8; k++) {
                                syndrome[i] ^= bits[k]*H[i][k];
                        }
                }

                /* decodage du syndrome et du message */
                decodage(bits, syndrome);
        }

        /* \n final */
        printf("\n");

        return 0;
}
