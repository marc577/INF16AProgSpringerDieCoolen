#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


/* ************ Nice to have ***********
- Ausgabe in Textdatei
- Degubmodus
- vairable Brettgroesse
    ->maxBoardSize mit Pointer machen, damit der Wert zur Laufzeit eingelesen werden kann
    ->Problem, wenn die Variable keine Konstante ist, kann kein Array initialisiert werden
***************************************/

/**
 * Global variable maxBoardSize
 * Sets the max number of fields in the board
 **/
//const int maxBoardSize = 8;
#define maxBoardSize 8

/**
 * Struct Field
 * Represents one field in the chessboard
 * @property row The row index of the board.
 * @property column The column index in the board.
 * @property value The couter value of the knight's steps.
 * @property anzMoeglichkeiten
 * @property mFelder
 * @property letzterVersuch
 * @property desc Column-Row representation of the field in the borad (human readable)
 **/
struct Field {
    int zeile;
    int spalte;
    int value;
    int anzMoeglichkeiten;
    void* mFelder[maxBoardSize];
    int letzterVersuch ;
    char desc[2];
};
typedef struct Field Field;

/**
 * Struct Chessboard
 * Holds a number of fields.
 **/
struct Chessboard {
    struct Field fields[maxBoardSize][maxBoardSize];
};
typedef struct Chessboard Chessboard;

/**
 * Global variable board
 **/
Chessboard board;




/**
 * Reads and verifies the user's input to get the
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
        printf("Bitte Reihe eingeben (1-%d): ", maxBoardSize);
        scanf("%d", &inputRow);
        inputVerifyer = (inputRow >= 1 && inputRow <= maxBoardSize);
        if(inputVerifyer == 0){
            printf("Eingabe falsch.\n");
        }
    }while( inputVerifyer == 0 );
    retVal.zeile = inputRow - 1;
    retVal.desc[0] = retVal.spalte + 65;
    retVal.desc[1] = (retVal.zeile + 1) +'0';
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

/**
 * Verifies if a specific row and column couple describes a
 * valid field for the board size.
 * @param column The column index to check.
 * @param row The row index to check.
 * @return Retuns 1 if the column and row describe a valid field, otherwise 0
 */
int verifyFeld(int column, int row){
    if(column >= 0 && column < maxBoardSize && row >= 0 && row < maxBoardSize){
        return 1;
    }
    return 0;
}


/**
 * Initializes the possible followers of one field.
 * @param argField A field pointer to the who's followers will be init.
 */
void initPossibleFollowersForField(Field *argField){
    int counter = 0;
    int startRow = argField->zeile;
    int startCol = argField->spalte;

    int newCol = startCol + 2;
    int newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->mFelder[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->mFelder[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->mFelder[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->mFelder[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->mFelder[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->mFelder[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->mFelder[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->mFelder[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }

    argField->anzMoeglichkeiten = counter;

}

/**
 * Initializes the global variable 'board' and its fields.
 */
void initBoard(){
    for (int row = 0; row < maxBoardSize; row++){
        for(int col = 0; col < maxBoardSize; col++){
            Field field;
            field.zeile = row;
            field.spalte = col;
            field.value = -1;
            field.letzterVersuch = 0;
            field.desc[0] = field.spalte + 65;
            field.desc[1] = (field.zeile + 1) +'0';
            board.fields[row][col] = field;
        }
    }

    for (int row = 0; row < maxBoardSize; row++){
        for(int col = 0; col < maxBoardSize; col++){
            Field *fieldPointer = &board.fields[row][col];
            initPossibleFollowersForField(fieldPointer);
        }
    }
    for (int row = 0; row < maxBoardSize; row++){
        for(int col = 0; col < maxBoardSize; col++){
            Field *fieldPointer = &board.fields[row][col];
            sortPossibleFollowerArray(fieldPointer);
        }
    }
}

/**
 * Prints the global variable 'board' with the knight's step
 * -1 => The knight did not reched the field yet.
 */
void printBoard(){
    for (int row = 0; row < maxBoardSize; row++){
        for(int col = 0; col < maxBoardSize; col++){
            printf("%02d|", board.fields[row][col].value);
        }
        printf("\n");
    }
}

/**
 * Searches for a field with a specific knight step in board.
 * @param position The knight's step value to search for.
 * @return Returns a field pointer to the searched field. If no field was found returns NULL.
 */
Field* getFieldAt(int position) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.fields[i][j].value == position) {
                return (Field*) &(board.fields[i][j]);
            }
        }
    }
    return NULL;
}

/** Global stopwatch variables **/
double start_t_millis, end_t_millis;

/**
 * Returns the time in millis from the clock time.
 */
double get_time_millis(){
    //double ms; // millis to return
    //struct timespec spec;
    //clock_gettime(CLOCK_MONOTONIC, &spec);
    //ms = spec.tv_nsec / 1.0e6; // convert nanoseconds to milliseconds
    //return ms;
    return 1.0;
}

/**
 * Saves the current time in millis in the global variable 'start_t_millis'.
 * (calls get_time_millis)
 */
void startStopwatch(){
    start_t_millis = get_time_millis();
}

/**
 * Saves the current time in millis in the global variable 'end_t_millis'.
 * (calls get_time_millis)
 */
void stopStopwatch(){
    end_t_millis = get_time_millis();
}

/**
 * Prints the elapsed time between the call 'startStopwatch'
 * and 'stopStopwatch' in millis.
 */
void printStopwatch(){
    printf("Dauer: %lf Millisekunden. \n", end_t_millis - start_t_millis);
}

/**
 * Starts the knights walk.
 * @param startField The field from which the walk begins.
 */
void walk(Field startField){
    startStopwatch();
    int walkCounter = 0;
    Field* cFeld = &(board.fields[startField.zeile][startField.spalte]);
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
            cFeld = getFieldAt(walkCounter);
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

/**
 * Starts the programm.
 */
int main()
{
    printf("\n##### Springerproblem #####\n");

    initBoard();
    printf("\n\nSchachbrett initialisiert: \n\n");
    printBoard();

    Field startField = getStartPosition();

    printf("\n\nSpringer laeuft von %s aus los...", startField.desc);
    walk(startField);
    printf("\n... laufen abgeschlossen. ");
    printStopwatch();
    printf("\n");
    // Schachfield nach dem laufen
    printBoard();

    return 0;
}
