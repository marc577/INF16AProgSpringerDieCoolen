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
    int letzterVersuch ;
    int prio;
};
typedef struct Feld Feld;

struct Schachbrett {
    struct Feld felder[boardMaxLength][boardMaxLength];
};
typedef struct Schachbrett Schachbrett;

Schachbrett schachbrett;
int walkCounter = 0;

/* Startfedld Eingabe return Startfeld*/
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

void sortMFelder(Feld *argFeld) {
    //Feld *mFelderPointer = argFeld->mFelder;

    //void* mFelder = argFeld->mFelder;
    printf("### VORHER ## %d, %d ##############################################\n", argFeld->zeile, argFeld->spalte);

    for (int pos = 0; pos < argFeld->anzMoeglichkeiten; pos++){
        printf("%d, (%d|%d)\n", ((Feld *)argFeld->mFelder[pos])->anzMoeglichkeiten,((Feld *)argFeld->mFelder[pos])->zeile, ((Feld *)argFeld->mFelder[pos])->spalte);
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


    printf("### NACHHER ## %d, %d ##############################################\n", argFeld->zeile, argFeld->spalte);

    for (int pos = 0; pos < argFeld->anzMoeglichkeiten; pos++){
        printf("%d, (%d|%d)\n", ((Feld *)argFeld->mFelder[pos])->anzMoeglichkeiten,((Feld *)argFeld->mFelder[pos])->zeile, ((Feld *)argFeld->mFelder[pos])->spalte);
    }

    printf("### ENDE ################################################\n");

    //argFeld->mFelder = mFelder;
}

void initMoeglicheFelderOfFeld(Feld *argFeld){
    int counter = 0;
    int startRow = argFeld->zeile;
    int startCol = argFeld->spalte;

    int newCol = startCol + 2;
    int newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.felder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.felder[newRow][newCol];
        counter += 1;
    }

    argFeld->anzMoeglichkeiten = counter;

}

void initSchachbrett(){
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
            schachbrett.felder[row][col] = feld;
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
            Feld *feldPointer = &schachbrett.felder[row][col];
            initMoeglicheFelderOfFeld(feldPointer);

        }
    }
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            Feld *feldPointer = &schachbrett.felder[row][col];
            sortMFelder(feldPointer);

        }
    }
}

void brettAusgeben(){
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            printf("%02d|", schachbrett.felder[row][col].value);
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
/*void addCellToBrett(Feld argFeld, int counter) {
    printf("\n## Bin beim laufen\n");
    counter = counter +1;
    int posI = 0;
    if (argFeld.value < -99) {
        printf("\n## Wert der Startposition: %d\n", argFeld.value);
        schachbrett.felder[argFeld.zeile][argFeld.spalte] = argFeld;
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
            schachbrett.felder[newRow][newCol] = *newFeld;
            addCellToBrett(*newFeld, counter);
        }
    }
}*/

void walk(Feld startFeld){
    //schachbrett.felder[startFeld.zeile][startFeld.spalte] = startFeld;
    Feld* cFeld = &(schachbrett.felder[startFeld.zeile][startFeld.spalte]);
    //printf("WalkCounter: %d", walkCounter);
    //printf("WalkCounter: %d", cFeld.anzMoeglichkeiten);
    for(int feldPos= 0 ; feldPos < 38; feldPos++){
        /*if(walkCounter == -1){
            break;
        }*/
        cFeld->value = walkCounter;
        walkCounter += 1;

        int foundNextFeld = -1;
        Feld* nextFeld;
        do{
            if(cFeld->letzterVersuch >= cFeld->anzMoeglichkeiten){
                break;
            }
            void* feldAdress = cFeld->mFelder[cFeld->letzterVersuch];
            nextFeld = (Feld *)feldAdress;
            if(nextFeld->value == -1){
                foundNextFeld = 1;
            }else{
                cFeld->letzterVersuch += 1;
            }
        }while(foundNextFeld == -1);
        if(foundNextFeld == -1){
            printf("Du musst zuŸck gehen");
        }else{
            cFeld = nextFeld;
        }

        //nextFeld->value = walkCounter;
        //printf("Irwas Gemacht %d, %d", nextFeld->zeile, nextFeld->spalte);

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
    initSchachbrett();
    printf("\n\n############################\nAusgabe Brett nach initialisieren\n############################\n");
    brettAusgeben();
    printf("\n## Bin beim laufen Main\n");
    //addCellToBrett(startFeld, 0);
    walk(startFeld);
    // Schachfeld ausgeben
    printf("\n\n############################\nAusgabe nachm laufen\n############################\n");
    brettAusgeben();

    return 0;
}
