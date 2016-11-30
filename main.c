#include <stdio.h>
#include <stdlib.h>

/* Nice to have
- Wenn falsche Zeile oder Spalte, dann nochmal fragen
- Ausgabe in Textdatei
- Degubmodus

*/

struct MoeglicheFelder {

};

struct Feld {
    int zeile;
    int spalte;
    int value;
    int anzMoeglichkeiten;
    struct Feld* moeglicheFelder[8];
    int letzterVersuch;
};

struct Schachbrett {
    struct Feld felder[8][8];

};

struct Feld getStartPosition() {
    struct Feld retVal;
    int iRow, iCol;
    printf("Es wird nun die Startposition abgefragt!\n");
    printf("Bitte Zeile eingeben (1-8): ");
    scanf("%i", &iRow);
    printf("\n");
    if (iRow < 1 || iRow > 8) {
        printf("Ungueltige Reihe. Programm wird beendet !\n");
    } else {
        iRow--;
    }
    retVal.zeile = iRow;
    printf("Bitte Spalte eingeben (1-8): ");
    scanf("%i", &iCol);
    printf("\n");
    if (iCol < 1 || iCol > 8) {
        printf("Ungueltige Spalte. Programm wird beendet !\n");
    } else {
        iCol--;
    }
    retVal.spalte = iCol;
    retVal.value = 0;
    return retVal;
}

struct Schachbrett initSchachbrett(){
    struct Schachbrett brett;
    for (int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            struct Feld feld;
            feld.zeile = row;
            feld.spalte = row;
            feld.value = -1;
            feld.letzterVersuch = 0;
            brett.felder[row][col] = feld;
        }
    }

    for (int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            struct Feld feld = brett.felder[row][col];
            feld.moeglicheFelder = getMoeglicheFelderOfFeld(&feld);
        }
    }


    return brett;
}

struct MoeglicheFelder getMoeglicheFelderOfFeld(struct Feld *argFeld){

}

void brettAusgeben(struct Schachbrett argBrett){
    for (int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
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
void addCellToBrett(struct Feld argFeld, struct Schachbrett *argBrett) {
    printf("## Wert der Startposition: %d", argFeld.value);
    (*argBrett).felder[argFeld.zeile][argFeld.spalte] = argFeld;
}

int main()
{
    // Startprosition auswŠhlen
    printf("\n\n############################\nEs geht los\n############################\n");
    struct Feld startFeld = getStartPosition();
    printf("Zeile: %d, Spalte: %d Posiion: %d\n",startFeld.zeile, startFeld.spalte, startFeld.value);
    // Schachfeld initialisieren
    struct Schachbrett brett = initSchachbrett();
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
