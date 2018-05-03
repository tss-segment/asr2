#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Je vais colorer la sortie pour indiquer les erreurs/corrections */
/* https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
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

        short int correction = 0; /* stocke si les trois premiers bits sont non nuls */
        short int est_nul = 1;
        for (size_t i = 0; i < 3; i++) {
                est_nul &= (syndrome[i] == 0);
                correction |= (syndrome[i] = 1);
        }
        est_nul &= (syndrome[4] == 0);

        if (est_nul) {
                /* alors tout va pour le mieux dans le meilleur des mondes */
                /* donc on écrit en GREEN */
                printf(ANSI_COLOR_GREEN   "%c"   ANSI_COLOR_RESET, bits[2]);
                printf(ANSI_COLOR_GREEN   "%c"   ANSI_COLOR_RESET, bits[4]);
                printf(ANSI_COLOR_GREEN   "%c"   ANSI_COLOR_RESET, bits[5]);
                printf(ANSI_COLOR_GREEN   "%c"   ANSI_COLOR_RESET, bits[6]);
        } else if (correction && syndrome[4]) {
                /* alors on corrige 1 erreur */
                int position = 0;
                for (size_t i = 0; i < 3; i++) {
                        position |= syndrome[i] << i;
                }
                bits[position] ^= 1;

                /* on a fait une correction, donc YELLOW */
                printf(ANSI_COLOR_YELLOW   "%c"   ANSI_COLOR_RESET, bits[2]);
                printf(ANSI_COLOR_YELLOW   "%c"   ANSI_COLOR_RESET, bits[4]);
                printf(ANSI_COLOR_YELLOW   "%c"   ANSI_COLOR_RESET, bits[5]);
                printf(ANSI_COLOR_YELLOW   "%c"   ANSI_COLOR_RESET, bits[6]);
        } else {
                /* alors on a un nombre pair d'erreur, et on panique */
                /* mais dans le cadre de ce projet, on va juste mettre du RED */
                printf(ANSI_COLOR_RED   "%c"   ANSI_COLOR_RESET, bits[2]);
                printf(ANSI_COLOR_RED   "%c"   ANSI_COLOR_RESET, bits[4]);
                printf(ANSI_COLOR_RED   "%c"   ANSI_COLOR_RESET, bits[5]);
                printf(ANSI_COLOR_RED   "%c"   ANSI_COLOR_RESET, bits[6]);
        }

        return;
}


int main(int argc, char const *argv[]) {
        /* capte l'entree sur stdin, et decode hamming */
        /** https://fr.wikipedia.org/wiki/Code_de_Hamming_(7,4)#Code_de_Hamming_(8,4) **/

        int bits[8];
        int syndrome[4];
        int bits_lus = 1;
        int chiffre_lu;

        while (bits_lus > 0) {

                /* lecture des bits à décoder */
                bits_lus = 0;
                while (bits_lus < 8 && (chiffre_lu = getchar() && chiffre_lu != EOF)) {
                        if (chiffre_lu == '0' || chiffre_lu == '1') {
                                bits[bits_lus] = chiffre_lu - '0';
                                bits_lus++;
                        }
                }

                if (bits_lus < 8) {
                        fprintf(stderr, ANSI_COLOR_RED "\nAttention : nombre"
                                "incorrect de bits reçu, erreur probable !"
                                ANSI_COLOR_RESET);
                }

                if (bits_lus == 8) {
                        /* produit matriciel */
                        for (size_t i = 0; i < 4; i++) {
                                syndrome[i] = 0;
                                for (size_t k = 0; k < 8; k++) {
                                        syndrome[i] += bits[k]*H[i][k];
                                }
                        }

                        /* decodage du syndrome et du message */
                        decodage(bits, syndrome);
                }
        }

        /* \n final */
        printf("\n");

        return 0;
}
