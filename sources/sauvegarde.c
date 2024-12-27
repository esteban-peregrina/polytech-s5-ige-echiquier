#include ".././headers/sauvegarde.h"

void save_echiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], Piece* joueurNoir[16], char* directoryname) {
    mkdir(directoryname, 0777);
    chdir(directoryname);

    FILE* descriptor = fopen("echiquier", "w");

    // Sauvegarder l'état de l'échiquier
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Case* currentCase = Echiquier[y][x];
            if (currentCase->piece) {
                Piece* piece = currentCase->piece;
                fprintf(descriptor, "%d %d %d %d\n", 
                        piece->role, 
                        piece->couleur, 
                        piece->x, 
                        piece->y);
            } else {
                fprintf(descriptor, "-1 -1 -1 -1\n"); // Case vide
            }
        }
    }

    fclose(descriptor);

    // Sauvegarder les joueurs
    FILE* joueurBlancFile = fopen("joueurBlanc", "w");
    for (int i = 0; i < 16; i++) {
        Piece* piece = joueurBlanc[i];
        if (piece) {
            fprintf(joueurBlancFile, "%d %d %d %d\n", piece->role, piece->couleur, piece->x, piece->y);
        }
    }
    fclose(joueurBlancFile);

    FILE* joueurNoirFile = fopen("joueurNoir", "w");
    for (int i = 0; i < 16; i++) {
        Piece* piece = joueurNoir[i];
        if (piece) {
            fprintf(joueurNoirFile, "%d %d %d %d\n", piece->role, piece->couleur, piece->x, piece->y);
        }
    }
    fclose(joueurNoirFile);

    printf("Successfully saved game to \"%s\"\n", directoryname);
    chdir("-");
}
