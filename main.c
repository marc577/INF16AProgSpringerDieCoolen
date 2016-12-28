#include <stdio.h>
#include <stdlib.h>

/* *********** Nice to have ***********
- Wenn falsche Zeile oder Spalte, dann nochmal fragen
- Ausgabe in Textdatei
- Degubmodus

***************************************/
const int boardMaxLength = 8;
// boardMaxLength mit Pointer machen, damit der Wert zur Laufzeit eingelesen werden kann
// Problem, wenn die Variable keine Konstante ist, kann kein Array initialisiert werden

/* so etwa:
const int boardMaxLength = 8;
int *p1 = &boardMaxLength;
*pi = 40
*/

struct Feld {
    int zeile;
    int spalte;
    int value;
    int anzMoeglichkeiten;
    //int moeglicheFelderPositions[boardMaxLength][boardMaxLength];
    void* mFelder[8];
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
    printf("Es wird nun die Startposition abgefragt!!!!!!\n");
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

int verifyFeld(int spalte, int zeile){
    if(spalte >= 0 && spalte < boardMaxLength && zeile >= 0 && zeile < boardMaxLength){
        return 1;
    }
    return 0;
}

void sortMFelder(Feld *argFeld, Schachbrett *brett) {
    //Feld *mFelderPointer = argFeld->mFelder;

    //void* mFelder = argFeld->mFelder;
    printf("### VORHER ################################################\n");

    for (int pos = 0; pos < argFeld->anzMoeglichkeiten; pos++){
        printf("%d\n", ((Feld *)argFeld->mFelder[pos])->anzMoeglichkeiten);
    }
    //printf("XXX:%d", mFelder[0]);
    for (int posOut = 0; posOut < argFeld->anzMoeglichkeiten; posOut++){
        Feld* feldPointer = argFeld->mFelder[posOut];

        for (int posIn = posOut; posIn < argFeld->anzMoeglichkeiten; posIn++){
            Feld* feldPointer2 = argFeld->mFelder[posIn];
            if (feldPointer->anzMoeglichkeiten > feldPointer2->anzMoeglichkeiten) {
                void* tmp = argFeld->mFelder[posIn];
                argFeld->mFelder[posIn] = argFeld->mFelder[posOut];
                argFeld->mFelder[posOut] = tmp;
            }

        }
    }


    printf("### NACHHER *********************************************\n");

    for (int pos = 0; pos < argFeld->anzMoeglichkeiten; pos++){
        printf("%d\n", ((Feld *)argFeld->mFelder[pos])->anzMoeglichkeiten);
    }

    printf("### ENDE ################################################\n");

    //argFeld->mFelder = mFelder;
}

void initMoeglicheFelderOfFeld(Feld *argFeld, Schachbrett *brett){
    int counter = 0;
    int startRow = argFeld->zeile;
    int startCol = argFeld->spalte;

    int newCol = startCol + 3;
    int newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &brett->felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 3;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &brett->felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 3;
    newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &brett->felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 3;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &brett->felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow - 3;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &brett->felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow + 3;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &brett->felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow + 3;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &brett->felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow - 3;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &brett->felder[newRow][newCol];
        counter += 1;
    }

    argFeld->anzMoeglichkeiten = counter;

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

    /*long feldPointer = &brett.felder[2][2];
    ((Feld *)feldPointer)->prio = 42;
    printf("FELD Prio: %d\n", brett.felder[2][2].prio);
    printf("FELD POINTER: %d\n", feldPointer);
    printf("FELD POINTER: %p\n", feldPointer);
    printf("FELD Brett Pointer: %p\n", &brett.felder[2][2]);
    printf("FELD Prio2: %d\n", ((Feld *)feldPointer)->prio);*/

    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            Feld *feldPointer = &brett.felder[row][col];
            initMoeglicheFelderOfFeld(feldPointer, &brett);

        }
    }
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            Feld *feldPointer = &brett.felder[row][col];
            sortMFelder(feldPointer, &brett);

        }
    }


    return brett;
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
void addCellToBrett(Feld argFeld, Schachbrett *argBrett, int counter) {
    printf("\n## Bin beim laufen\n");
    counter = counter +1;
    int posI = 0;
    if (argFeld.value < -99) {
        printf("\n## Wert der Startposition: %d\n", argFeld.value);
        (*argBrett).felder[argFeld.zeile][argFeld.spalte] = argFeld;
    } else if(argFeld.value >= 64) {
        printf("\n## Wert der letzten Position: %d\n", argFeld.value);
    } else {
        //for (int posI = 0; posI < argFeld.anzMoeglichkeiten; posI++)
        printf("\n## Wert der aktuellen Position: %d\n", argFeld.value);
        argFeld.value = counter;
        for (posI = 0; posI <= 1; posI++) {
            argFeld.letzterVersuch = posI;
            int newRow = ((Feld *)argFeld.mFelder[posI])->zeile;
            int newCol = ((Feld *)argFeld.mFelder[posI])->spalte;
            Feld* newFeld = argFeld.mFelder[posI];
            (*argBrett).felder[newRow][newCol] = *newFeld;
            addCellToBrett(*newFeld, (Schachbrett*) &argBrett, counter);
        }
    }
}

void initFeldGroesse() {
    // Optional: Feldgroesse einlesen
    printf("Bitte die Feldgroesse eingeben (z.B. 8 fuer 8x8):");
    //boardMaxLength = scnaf("%i", &boardMaxLength);
}

int main()
{
    printf("\n\n############################\nEs geht los\n############################\n");
    Feld startFeld = getStartPosition();
    printf("Zeile: %d, Spalte: %d Posiion: %d\n",startFeld.zeile, startFeld.spalte, startFeld.value);
    Schachbrett brett = initSchachbrett();
    printf("\n\n############################\nAusgabe Brett nach initialisieren\n############################\n");
    brettAusgeben(brett);
    printf("\n## Bin beim laufen Main\n");
    addCellToBrett(startFeld, &brett, 0);
    // Schachfeld ausgeben
    printf("\n\n############################\nAusgabe nachm laufen\n############################\n");
    brettAusgeben(brett);

    return 0;
}
