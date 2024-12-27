#include ".././headers/sauvegarde.h"

void save_echiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], Piece* joueurNoir[16], char* directoryname) {
    char currentDir[1024];
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("Failed to get current working directory");
        return;
    }

    // Create the directory if it doesn't exist
    mkdir(directoryname, 0777);
    
    // Change to the specified directory
    if (chdir(directoryname) != 0) {
        perror("Failed to change directory");
        return;
    }

    FILE* descriptor = fopen("echiquier", "w");

    // Save the state of the chessboard
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
                fprintf(descriptor, "-1 -1 -1 -1\n"); // Empty case
            }
        }
    }

    fclose(descriptor);

    // Save the players (pieces)
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

    // Return to the original directory
    if (chdir(currentDir) != 0) {
        perror("Failed to return to the original directory");
    }
}

void load_echiquier(Case* Echiquier[8][8], Piece* joueurBlanc[16], Piece* joueurNoir[16], char* directoryname) {
    char currentDir[1024];
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("Failed to get current working directory");
        return;
    }

    // Change to the directory containing the saved game
    if (chdir(directoryname) != 0) {
        perror("Failed to change directory");
        return;
    }

    FILE* descriptor = fopen("echiquier", "r");
    if (!descriptor) {
        perror("Failed to open echiquier file");
        return;
    }

    // Load the state of the chessboard
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int role, couleur, posX, posY;
            fscanf(descriptor, "%d %d %d %d\n", &role, &couleur, &posX, &posY);

            Case* currentCase = creationCase(x, y);
            if (role != -1) {
                Piece* piece = creationPiece((Role)role, couleur);
                piece->x = posX;
                piece->y = posY;
                currentCase->piece = piece;
            }
            Echiquier[y][x] = currentCase;
        }
    }

    fclose(descriptor);

    // Load the players
    FILE* joueurBlancFile = fopen("joueurBlanc", "r");
    for (int i = 0; i < 16; i++) {
        int role, couleur, x, y;
        fscanf(joueurBlancFile, "%d %d %d %d\n", &role, &couleur, &x, &y);
        joueurBlanc[i] = creationPiece((Role)role, couleur);
        joueurBlanc[i]->x = x;
        joueurBlanc[i]->y = y;
    }
    fclose(joueurBlancFile);

    FILE* joueurNoirFile = fopen("joueurNoir", "r");
    for (int i = 0; i < 16; i++) {
        int role, couleur, x, y;
        fscanf(joueurNoirFile, "%d %d %d %d\n", &role, &couleur, &x, &y);
        joueurNoir[i] = creationPiece((Role)role, couleur);
        joueurNoir[i]->x = x;
        joueurNoir[i]->y = y;
    }
    fclose(joueurNoirFile);

    printf("Successfully loaded game from \"%s\"\n", directoryname);

    // Return to the original directory
    if (chdir(currentDir) != 0) {
        perror("Failed to return to the original directory");
    }
}
