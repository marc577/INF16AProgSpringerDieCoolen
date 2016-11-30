#include <stdio.h>
#include <stdlib.h>

/* *********** Nice to have ***********
- Wenn falsche Zeile oder Spalte, dann nochmal fragen
- Ausgabe in Textdatei
- Degubmodus

***************************************/

struct MoeglicheFelder {

};
typedef struct MoeglicheFelder MoeglicheFelder;

struct Feld {
    int zeile;
    int spalte;
    int value;
    int anzMoeglichkeiten;
    struct Feld* moeglicheFelder[8];
    int letzterVersuch;
};
typedef struct Feld Feld;

struct Schachbrett {
    struct Feld felder[8][8];

};
typedef struct Schachbrett Schachbrett;

Feld getStartPosition() {
    Feld retVal;
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

Schachbrett initSchachbrett(){
    Schachbrett brett;
    for (int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            Feld feld;
            feld.zeile = row;
            feld.spalte = row;
            feld.value = -1;
            feld.letzterVersuch = 0;
            brett.felder[row][col] = feld;
        }
    }
/*
    for (int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            Feld feld = brett.felder[row][col];
            feld.moeglicheFelder = getMoeglicheFelderOfFeld(&feld);
        }
    }
*/
    return brett;
}
/*
MoeglicheFelder getMoeglicheFelderOfFeld(Feld *argFeld){

}
*/
void brettAusgeben(Schachbrett argBrett){
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
void addCellToBrett(Feld argFeld, Schachbrett *argBrett) {
    printf("## Wert der Startposition: %d", argFeld.value);
    (*argBrett).felder[argFeld.zeile][argFeld.spalte] = argFeld;
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
