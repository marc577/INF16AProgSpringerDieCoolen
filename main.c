#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


/* ************ Nice to have ***********
- Wenn falsche Zeile oder Spalte, dann nochmal fragen
- Ausgabe in Textdatei
- Degubmodus
- vairable Brettgroesse
***************************************/
const int boardMaxLength = 8;
// boardMaxLength mit Pointer machen, damit der Wert zur Laufzeit eingelesen werden kann
// Problem, wenn die Variable keine Konstante ist, kann kein Array initialisiert werden

/**
 * Struct Feld
 **/
struct Field {
    int zeile;
    int spalte;
    int value;
    int anzMoeglichkeiten;
    void* mFelder[boardMaxLength];
    int letzterVersuch ;
    char desc[2];
};
typedef struct Field Field;

/**
 * Struct Schachbrett
 **/
struct Schachbrett {
    struct Field fielder[boardMaxLength][boardMaxLength];
};
typedef struct Schachbrett Schachbrett;

/**
 * Globale Variablen
 * Struct Schachbrett
 **/
Schachbrett schachbrett;



/**
 * Reads and valids the user's input to get the
 * start position.
 * @return Inited Field with row and column values.
 */
Field getStartPosition() {

    Field retVal;

    printf("Startposition: \n");
    int inputVerifyer = 0;
    char inputCol;
    do{
        printf("Bitte Spalte eingeben (A-H): ");
        scanf("%c", &inputCol);
        inputCol = toupper(inputCol);
        inputVerifyer = (inputCol >= 'A' && inputCol <= 'H');
        if(inputVerifyer == 0){
            printf("Eingabe falsch.\n");
        }
    }while( inputVerifyer == 0);
    retVal.spalte = inputCol - 65;

    int inputRow;
    inputVerifyer = 0;
    do{
        printf("Bitte Reihe eingeben (1-%d): ", boardMaxLength);
        scanf("%d", &inputRow);
        inputVerifyer = (inputRow >= 1 && inputRow <= boardMaxLength);
        if(inputVerifyer == 0){
            printf("Eingabe falsch.\n");
        }
    }while( inputVerifyer == 0 );
    retVal.zeile = inputRow - 1;

    return retVal;
}




/**
 * Sorts (Bubble Sort) the possible followers array
 * for one specific field from the board. The Array will be ordered
 * ascending by the number of the possible followers from the field's followers.
 * @param The field reference (callbyreference) which followers will be sort.
 */
void sortPossibleFollowerArray(Field *argField) {

    printf("## Nachfolger von %s unsortiert: \n", argField->desc);
    for (int pos = 0; pos < argField->anzMoeglichkeiten; pos++){
        printf("%s -> mit %d Nachfolgern \n", ((Field *)argField->mFelder[pos])->desc,((Field *)argField->mFelder[pos])->anzMoeglichkeiten);
    }
    printf("##");

    for (int posOut = 0; posOut < argField->anzMoeglichkeiten - 1; posOut++){
        for (int posIn = 0; posIn < (argField->anzMoeglichkeiten - posOut - 1); posIn++){
            Field* fieldPointer = argField->mFelder[posIn];
            Field* fieldPointer2 = argField->mFelder[posIn+1];
            if (fieldPointer->anzMoeglichkeiten > fieldPointer2->anzMoeglichkeiten) {
                void* tmp = argField->mFelder[posIn];
                argField->mFelder[posIn] = argField->mFelder[posIn+1];
                argField->mFelder[posIn+1] = tmp;
            }

        }
    }

    printf("## Nachfolger von %s sortiert: \n", argField->desc);
    for (int pos = 0; pos < argField->anzMoeglichkeiten; pos++){
        printf("%s -> mit %d Nachfolgern \n", ((Field *)argField->mFelder[pos])->desc,((Field *)argField->mFelder[pos])->anzMoeglichkeiten);
    }
    printf("## \n\n");

}

int verifyFeld(int spalte, int zeile){
    if(spalte >= 0 && spalte < boardMaxLength && zeile >= 0 && zeile < boardMaxLength){
        return 1;
    }
    return 0;
}
void initMoeglicheFelderOfFeld(Field *argFeld){
    int counter = 0;
    int startRow = argFeld->zeile;
    int startCol = argFeld->spalte;

    int newCol = startCol + 2;
    int newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.fielder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.fielder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.fielder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.fielder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.fielder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.fielder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.fielder[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argFeld->mFelder[counter] = &schachbrett.fielder[newRow][newCol];
        counter += 1;
    }

    argFeld->anzMoeglichkeiten = counter;

}

void initBoard(){
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            Field field;
            field.zeile = row;
            field.spalte = col;
            field.value = -1;
            field.letzterVersuch = 0;

            //sprintf(field.desc, "%c%d", field.spalte + 65, field.zeile + 1);
            field.desc[0] = field.spalte + 65;
            field.desc[1] = (field.zeile + 1) +'0';
            schachbrett.fielder[row][col] = field;
        }
    }

    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            Field *fieldPointer = &schachbrett.fielder[row][col];
            initMoeglicheFelderOfFeld(fieldPointer);
        }
    }
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            Field *fieldPointer = &schachbrett.fielder[row][col];
            sortPossibleFollowerArray(fieldPointer);
        }
    }
}

void printBoard(){
    for (int row = 0; row < boardMaxLength; row++){
        for(int col = 0; col < boardMaxLength; col++){
            printf("%02d|", schachbrett.fielder[row][col].value);
        }
        printf("\n");
    }
}


Field* getFeldAt(int position) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (schachbrett.fielder[i][j].value == position) {
                return (Field*) &(schachbrett.fielder[i][j]);
            }
        }
    }
    //return (Feld*) &(schachbrett.fielder[0][0]);
    return NULL;
}

/** Stopwatch **/
double start_t_millis, end_t_millis;
double get_time_millis(){
    double ms; // millis to return
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    ms = spec.tv_nsec / 1.0e6; // convert nanoseconds to milliseconds
    return ms;
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

void walk(Field startFeld){
    startStopwatch();
    int walkCounter = 0;
    Field* cFeld = &(schachbrett.fielder[startFeld.zeile][startFeld.spalte]);
     // start walk
    for(int fieldPos= 0 ; fieldPos < 64; fieldPos++){
        int foundNextFeld = -1;
        Field* nextFeld;
        do{
            if(cFeld->letzterVersuch >= cFeld->anzMoeglichkeiten){
                cFeld->letzterVersuch = 0;
                break;
            }
            void* fieldAdress = cFeld->mFelder[cFeld->letzterVersuch];
            nextFeld = (Field *)fieldAdress;
            if(nextFeld->value == -1){
                foundNextFeld = 1;
            }else{
                cFeld->letzterVersuch += 1;
            }
        }while(foundNextFeld == -1);
        if(foundNextFeld == -1){
            cFeld->value = -1;
            walkCounter--;
            cFeld = getFeldAt(walkCounter);
            cFeld->letzterVersuch += 1;
            if(cFeld == NULL){
                printf("Kein Weg gefunden!\n");
                break;
            }
            fieldPos = walkCounter;
            //printf("Keine Moeglichkeit mehr.Gehe eins zurŸck!\n");

        }else{

            cFeld->value = walkCounter;
            walkCounter += 1;
            cFeld = nextFeld;
            if (walkCounter == 63) {
                cFeld->value = walkCounter;
            }
        }
        //printf("Aktuelle Feldposition: %d\n", fieldPos);
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

    Field startField = getStartPosition();

    initBoard();
    printf("\n\nSchachbrett initialisiert: \n");
    // Schachfield nach der Initialisierung
    printBoard();

    printf("\n\nSpringer laeuft von %c%d aus los...", startField.spalte+65, startField.zeile+1);
    walk(startField);
    printf("\n... laufen abgeschlossen. ");
    printStopwatch();
    printf("\n");
    // Schachfield nach dem laufen
    printBoard();

    return 0;
}
