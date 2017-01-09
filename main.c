#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ************ Nice to have ***********
- Wenn falsche Zeile oder Spalte, dann nochmal fragen
- Ausgabe in Textdatei
- Degubmodus
***************************************/
const int boardMaxLength = 8;
// boardMaxLength mit Pointer machen, damit der Wert zur Laufzeit eingelesen werden kann
// Problem, wenn die Variable keine Konstante ist, kann kein Array initialisiert werden

/**
 * Struct Feld
 **/
struct Feld {
    int zeile;
    int spalte;
    int value;
    int anzMoeglichkeiten;
    void* mFelder[boardMaxLength];
    int letzterVersuch ;
    int prio;
};
typedef struct Feld Feld;

/**
 * Struct Schachbrett
 **/
struct Schachbrett {
    struct Feld felder[boardMaxLength][boardMaxLength];
};
typedef struct Schachbrett Schachbrett;

/**
 * Globale Variablen
 * Struct Schachbrett
 **/
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

/* Sort nach Bubble Sort*/
void sortMFelder(Feld *argFeld) {
    //Feld *mFelderPointer = argFeld->mFelder;

    //void* mFelder = argFeld->mFelder;
    printf("### VORHER ## %d, %d ##############################################\n", argFeld->zeile, argFeld->spalte);

    for (int pos = 0; pos < argFeld->anzMoeglichkeiten; pos++){
        printf("%d, (%d|%d)\n", ((Feld *)argFeld->mFelder[pos])->anzMoeglichkeiten,((Feld *)argFeld->mFelder[pos])->zeile, ((Feld *)argFeld->mFelder[pos])->spalte);
    }
    //printf("XXX:%d", mFelder[0]);
    for (int posOut = 0; posOut < argFeld->anzMoeglichkeiten - 1; posOut++){
        for (int posIn = 0; posIn < (argFeld->anzMoeglichkeiten - posOut - 1); posIn++){
            Feld* feldPointer = argFeld->mFelder[posIn];
            Feld* feldPointer2 = argFeld->mFelder[posIn+1];
            if (feldPointer->anzMoeglichkeiten > feldPointer2->anzMoeglichkeiten) {
                void* tmp = argFeld->mFelder[posIn];
                argFeld->mFelder[posIn] = argFeld->mFelder[posIn+1];
                argFeld->mFelder[posIn+1] = tmp;
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


Feld* getFeldAtCounter() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (schachbrett.felder[i][j].value == walkCounter) {
                return (Feld*) &(schachbrett.felder[i][j]);
            }
        }
    }
    //return (Feld*) &(schachbrett.felder[0][0]);
    return NULL;
}

/** Stopwatch **/
double start_t_millis, end_t_millis;
double get_time_millis(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + (t.tv_usec / 1000000.0)) * 1000.0;
}
void startStopwatch(){
    start_t_millis = get_time_millis();
}
void stopStopwatch(){
    end_t_millis = get_time_millis();
}
void printStopwatch(){
    printf("Dauer: %lf Millisekunden. \n", end_t_millis - start_t_millis);
}

void walk(Feld startFeld){
    startStopwatch();
    Feld* cFeld = &(schachbrett.felder[startFeld.zeile][startFeld.spalte]);
     // start walk
    for(int feldPos= 0 ; feldPos < 64; feldPos++){
        int foundNextFeld = -1;
        Feld* nextFeld;
        do{
            if(cFeld->letzterVersuch >= cFeld->anzMoeglichkeiten){
                cFeld->letzterVersuch = 0;
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
            cFeld->value = -1;
            walkCounter--;
            cFeld = getFeldAtCounter();
            cFeld->letzterVersuch += 1;
            if(cFeld == NULL){
                printf("Kein Weg gefunden!\n");
                break;
            }
            feldPos = walkCounter;
            //printf("Keine Moeglichkeit mehr.Gehe eins zurŸck!\n");

        }else{
            cFeld->value = walkCounter;
            walkCounter += 1;
            cFeld = nextFeld;
            if (walkCounter == 63) {
                cFeld->value = walkCounter;
            }
        }
        //printf("Aktuelle Feldposition: %d\n", feldPos);
    }
    // end walk
    stopStopwatch();
}



void initFeldGroesse() {
    // Optional: Feldgroesse einlesen
    printf("Bitte die Feldgroesse eingeben (z.B. 8 fuer 8x8):");
    //boardMaxLength = scnaf("%i", &boardMaxLength);
}

int main()
{
    printf("\n##### Springerproblem #####\n");

    Feld startFeld = getStartPosition();
    printf("Zeile: %d, Spalte: %d Posiion: %d\n",startFeld.zeile, startFeld.spalte, startFeld.value);

    initSchachbrett();
    printf("\n\nSchachbrett initialisiert: \n");
    // Schachfeld nach der Initialisierung
    brettAusgeben();

    printf("\n\nSpringer laeuft ...");
    walk(startFeld);
    printf("\n... laufen abgeschlossen. ");
    printStopwatch();
    printf("\n");
    // Schachfeld nach dem laufen
    brettAusgeben();

    return 0;
}
