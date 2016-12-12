#include <stdio.h>
#include <stdlib.h>

/* *********** Nice to have ***********
- Wenn falsche Zeile oder Spalte, dann nochmal fragen
- Ausgabe in Textdatei
- Degubmodus

***************************************/
int boardMaxLength = 8;

struct Feld {
    int zeile;
    int spalte;
    int value;
    int anzMoeglichkeiten;
    int moeglicheFelderPositions[boardMaxLength][boardMaxLength];
    int letzterVersuch;
    int prio;
};
typedef struct Feld Feld;

struct Schachbrett {
    struct Feld felder[boardMaxLength][boardMaxLength];

};
typedef struct Schachbrett Schachbrett;

Feld getStartPosition() {
    Feld retVal;
    int iRow, iCol;
    printf("Es wird nun die Startposition abgefragt!\n");
    printf("Bitte Zeile eingeben (1-8): ");
    scanf("%i", &iRow);
    printf("\n");
    if (iRow < 1 || iRow > boardMaxLength) {
        printf("Ungueltige Reihe. Programm wird beendet !\n");
    } else {
        iRow--;
    }
    retVal.zeile = iRow;
    printf("Bitte Spalte eingeben (1-8): ");
    scanf("%i", &iCol);
    printf("\n");
    if (iCol < 1 || iCol > boardMaxLength) {
        printf("Ungueltige Spalte. Programm wird beendet !\n");
    } else {
        iCol--;
    }
    retVal.spalte = iCol;
    retVal.value = 0;
    return retVal;
}

Schachbrett initSchachbrett(){
    Schachbrett brett;
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            Feld feld;
            feld.zeile = row;
            feld.spalte = col;
            feld.value = -1;
            feld.letzterVersuch = 0;
            if((col == 1 && row == 1) || (col == boardMaxLength-1 && row == boardMaxLength-1) || (col == boardMaxLength-1 && row == 1) || (col == 1 && row == boardMaxLength-1)){
                feld.prio = 1;
            }
            else if((col == 3 && row == 2) || (col == boardMaxLength-3 && row == boardMaxLength-2) || (col == boardMaxLength-3 && row == 2) ||
                     (col == 3 && row == boardMaxLength-2) || (col == 2 && row == 3) || (col == boardMaxLength-2 && row == boardMaxLength-3) ||
                    (col == boardMaxLength-2 && row == 3) || (col == 2 && row == boardMaxLength-3)){
                feld.prio = 2;
            }
            else{
                feld.prio = 4;
            }
            brett.felder[row][col] = feld;
        }
    }
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            Feld feld = brett.felder[row][col];
            feld.moeglicheFelderPositions = getMoeglicheFelderOfFeld(&feld);
        }
    }
    return brett;
}

int felder[][] getMoeglicheFelderOfFeld(Feld *argFeld){
    int felder[boardMaxLength][boardMaxLength];
    felder[0][0] = -1;
}

void brettAusgeben(Schachbrett argBrett){
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            printf("%02d|", argBrett.felder[row][col].value);
        }
        printf("\n");
    }
}
/*
struct Schachfeld addCellToBrett(struct Celle argCell, struct Schachfeld argBrett) {
    printf("## Wert der Startposition: %d", argCell.value);
    argBrett.felder[argCell.zeile][argCell.spalte] = argCell;
    return argBrett;
}
*/
void addCellToBrett(Feld argFeld, Schachbrett *argBrett) {
    printf("## Wert der Startposition: %d", argFeld.value);
    (*argBrett).felder[argFeld.zeile][argFeld.spalte] = argFeld;
}

void initFeldGroesse() {
    // Optional: Feldgroesse einlesen
    boardMaxLength = 8;
    printf("Bitte die Feldgroesse eingeben (z.B. 8 fuer 8x8):");
    boardMaxLength = scnaf("%i", &boardMaxLength);
}

int main()
{
    // Startprosition auswŠhlen
    printf("\n\n############################\nEs geht los\n############################\n");
    Feld startFeld = getStartPosition();
    printf("Zeile: %d, Spalte: %d Posiion: %d\n",startFeld.zeile, startFeld.spalte, startFeld.value);
    // Schachfeld initialisieren
    Schachbrett brett = initSchachbrett();
    // Schachfeld ausgeben
    printf("\n\n############################\nAusgabe\n############################\n");
    brettAusgeben(brett);
    // Startzelle hinzufügen
    //brett = addCellToBrett(startCell, &brett);
    addCellToBrett(startFeld, &brett);
    // Schachfeld ausgeben
    printf("\n\n############################\nAusgabe\n############################\n");
    brettAusgeben(brett);

    return 0;
}
