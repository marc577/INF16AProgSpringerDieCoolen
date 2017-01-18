#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


/* ************ Nice to have ***********
- Ausgabe in Textdatei
- Degubmodus
- vairable Brettgroesse
    ->MAX_BOARD_SIZE mit Pointer machen, damit der Wert zur Laufzeit eingelesen werden kann
    ->Problem, wenn die Variable keine Konstante ist, kann kein Array initialisiert werden
- Defines uppercase only
***************************************/

/**
 * Global variable MAX_BOARD_SIZE
 * Sets the max number of fields in the board
 **/
#define MAX_BOARD_SIZE 8
#define MAX_COLUMN (MAX_BOARD_SIZE + 64)
#define MAX_BOARD_FIELD (MAX_BOARD_SIZE*MAX_BOARD_SIZE)

/**
 * Global variable ANSI_COLOR
 * Mark first and last field.
 **/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/**
 * Struct Field
 * Represents one field in the chessboard
 * @property row The row index of the board.
 * @property column The column index in the board.
 * @property value The couter value of the knight's steps.
 * @property cPossMoves Number of possible follower fields.
 * @property pFields References to the possible follower fields.
 * @property lastTry Index of last tried follower.
 * @property desc Column-Row representation of the field in the borad (human readable)
 **/
struct Field {
    int row;
    int column;
    int value;
    int cPossMoves;
    void* pFields[8];
    int lastTry ;
    char desc[2];
};
typedef struct Field Field;

/**
 * Struct Chessboard
 * Holds a number of fields.
 **/
struct Chessboard {
    struct Field fields[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
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
        printf("Bitte Spalte eingeben (A-%c ; X fuer alle): \n", MAX_COLUMN);
        scanf("%c", &inputCol);
        inputCol = toupper(inputCol);
        if (inputCol == 'X') {
            retVal.column = -1;
            retVal.row = -1;
            return retVal;
        }

        inputVerifyer = (inputCol >= 'A' && inputCol <= MAX_COLUMN);
        if(inputVerifyer == 0){
            printf("Eingabe falsch.\n");
        }
    }while( inputVerifyer == 0);
    retVal.column = inputCol - 65;

    int inputRow;
    inputVerifyer = 0;
    do{
        printf("Bitte Reihe eingeben (1-%d): ", MAX_BOARD_SIZE);
        scanf("%d", &inputRow);
        inputVerifyer = (inputRow >= 1 && inputRow <= MAX_BOARD_SIZE);
        if(inputVerifyer == 0){
            printf("Eingabe falsch.\n");
        }
    }while( inputVerifyer == 0 );
    retVal.row = inputRow - 1;
    retVal.desc[0] = retVal.column + 65;
    retVal.desc[1] = (retVal.row + 1) +'0';
    return retVal;
}




/**
 * Sorts (Bubble Sort) the possible followers array
 * for one specific field from the board. The Array will be ordered
 * ascending by the number of the possible followers from the field's followers.
 * @param The field reference (callbyreference) which followers will be sort.
 */
void sortPossibleFollowerArray(Field *argField) {
    /*printf("## Nachfolger von %s unsortiert: \n", argField->desc);
    for (int pos = 0; pos < argField->cPossMoves; pos++){
        printf("%s -> mit %d Nachfolgern \n", ((Field *)argField->pFields[pos])->desc,((Field *)argField->pFields[pos])->cPossMoves);
    }
    printf("##");*/

    for (int posOut = 0; posOut < argField->cPossMoves - 1; posOut++){
        for (int posIn = 0; posIn < (argField->cPossMoves - posOut - 1); posIn++){
            Field* fieldPointer = argField->pFields[posIn];
            Field* fieldPointer2 = argField->pFields[posIn+1];
            if (fieldPointer->cPossMoves > fieldPointer2->cPossMoves) {
                void* tmp = argField->pFields[posIn];
                argField->pFields[posIn] = argField->pFields[posIn+1];
                argField->pFields[posIn+1] = tmp;
            }

        }
    }

    /*printf("## Nachfolger von %s sortiert: \n", argField->desc);
    for (int pos = 0; pos < argField->cPossMoves; pos++){
        printf("%s -> mit %d Nachfolgern \n", ((Field *)argField->pFields[pos])->desc,((Field *)argField->pFields[pos])->cPossMoves);
    }
    printf("## \n\n");*/

}

/**
 * Verifies if a specific row and column couple describes a
 * valid field for the board size.
 * @param column The column index to check.
 * @param row The row index to check.
 * @return Retuns 1 if the column and row describe a valid field, otherwise 0
 */
int verifyFeld(int column, int row){
    if(column >= 0 && column < MAX_BOARD_SIZE && row >= 0 && row < MAX_BOARD_SIZE){
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
    int startRow = argField->row;
    int startCol = argField->column;

    int newCol = startCol + 2;
    int newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->pFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->pFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->pFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->pFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->pFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->pFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->pFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->pFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }

    argField->cPossMoves = counter;

}

/**
 * Initializes the global variable 'board' and its fields.
 */
void initBoard(){
    for (int row = 0; row < MAX_BOARD_SIZE; row++){
        for(int col = 0; col < MAX_BOARD_SIZE; col++){
            Field field;
            field.row = row;
            field.column = col;
            field.value = -1;
            field.lastTry = 0;
            field.desc[0] = field.column + 65;
            field.desc[1] = (field.row + 1) +'0';
            board.fields[row][col] = field;
        }
    }

    for (int row = 0; row < MAX_BOARD_SIZE; row++){
        for(int col = 0; col < MAX_BOARD_SIZE; col++){
            Field *fieldPointer = &board.fields[row][col];
            initPossibleFollowersForField(fieldPointer);
        }
    }
    for (int row = 0; row < MAX_BOARD_SIZE; row++){
        for(int col = 0; col < MAX_BOARD_SIZE; col++){
            Field *fieldPointer = &board.fields[row][col];
            sortPossibleFollowerArray(fieldPointer);
        }
    }
}

/**
 * Prints the column for the board.
 */
void printBoardHead(){
    char head[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T'};
    printf("   ");
    for(int h = 0; h < MAX_BOARD_SIZE; h++){
        printf(" %c", head[h]);
        if(h != MAX_BOARD_SIZE - 1){
            printf("|");
        }
    }
    printf("\n");
}

/**
 * Prints the global variable 'board' with the knight's step
 * -1 => The knight did not reached the field yet.
 */
void printBoard(){
    printBoardHead();
    for (int row = 0; row < MAX_BOARD_SIZE; row++){
        printf("%d |", row + 1);
        for(int col = 0; col < MAX_BOARD_SIZE; col++){
            if(board.fields[row][col].value == 0){
                printf(ANSI_COLOR_GREEN "%02d" ANSI_COLOR_RESET "|", board.fields[row][col].value);
            }else if(board.fields[row][col].value == MAX_BOARD_FIELD - 1){
                printf(ANSI_COLOR_RED "%02d" ANSI_COLOR_RESET "|", board.fields[row][col].value);
            }else{
                printf("%02d|", board.fields[row][col].value);
            }
        }
        printf("  %d", row + 1);
        printf("\n");
    }
    printBoardHead();
}


/**
 * Searches for a field with a specific knight step in board.
 * @param position The knight's step value to search for.
 * @return Returns a field pointer to the searched field. If no field was found returns NULL.
 */
Field* getFieldAt(int position) {
    for (int i = 0; i < MAX_BOARD_SIZE; i++) {
        for (int j = 0; j < MAX_BOARD_SIZE; j++) {
            if (board.fields[i][j].value == position) {
                return (Field*) &(board.fields[i][j]);
            }
        }
    }
    return NULL;
}

/**
 * Starts the knights walk and searches for a way where the knight
 * can jump to the startfield again.
 * @param startField The field from which the walk begins.
 */
void walkAndGoToStartPos(Field startField){
    int walkCounter = 0;
    Field* cField = &(board.fields[startField.row][startField.column]);
     // start walk
    for(int fieldPos= 0 ; fieldPos < MAX_BOARD_FIELD; fieldPos++){
        int foundnextField = -1;
        Field* nextField;
        do{
            if(cField->lastTry >= cField->cPossMoves){
                cField->lastTry = 0;
                break;
            }
            void* fieldAdress = cField->pFields[cField->lastTry];
            nextField = (Field *)fieldAdress;
            if(nextField->value == -1){
                foundnextField = 1;
            }else{
                cField->lastTry += 1;
            }
        }while(foundnextField == -1);
        if(foundnextField == -1){
            cField->value = -1;
            walkCounter--;
            if(walkCounter < 0){
                printf("Kein Weg gefunden!\n");
                break;
            }
            cField = getFieldAt(walkCounter);
            cField->lastTry += 1;
            fieldPos = walkCounter;
            //printf("Keine Moeglichkeit mehr.Gehe eins zurŸck!\n");

        }else{

            cField->value = walkCounter;
            walkCounter += 1;
            cField = nextField;
            if (walkCounter == MAX_BOARD_FIELD - 1) {
                cField->value = walkCounter;
                int boStartFieldFounded = -1;
                for (int posI = cField->lastTry; posI < cField->cPossMoves; posI++) {
                    void* fieldAdress = cField->pFields[posI];
                    nextField = (Field *)fieldAdress;
                    if(nextField->value == 0){
                        boStartFieldFounded = 1;
                    }
                }
                if (boStartFieldFounded == -1) {
                    cField->value = -1;
                    walkCounter--;
                    if(walkCounter < 0){
                        printf("Kein Weg gefunden!\n");
                        break;
                    }
                    cField = getFieldAt(walkCounter);
                    cField->lastTry += 1;
                    fieldPos = walkCounter;
                }
            }
        }
        //printf("Aktuelle Feldposition: %d\n", fieldPos);
    }
    // end walk
}

/**
 * Starts the knights walk.
 * @param startField The field from which the walk begins.
 */
/*void walk(Field startField){
    int walkCounter = 0;

    Field* cField = &(board.fields[startField.row][startField.column]);
     // start walk
    //return;
    for(int fieldPos= 0 ; fieldPos < MAX_BOARD_FIELD; fieldPos++){
        int foundnextField = -1;
        Field* nextField;
        do{
            if(cField->lastTry >= cField->cPossMoves){
                cField->lastTry = 0;
                break;
            }

            void* fieldAdress = cField->pFields[cField->lastTry];
            nextField = (Field *)fieldAdress;
            if(nextField->value == -1){
                foundnextField = 1;
            }else{
                cField->lastTry += 1;
            }
        }while(foundnextField == -1);
        if(foundnextField == -1){
            cField->value = -1;
            walkCounter--;
            //printf("WalkCounter = %d \n", walkCounter);
            if(walkCounter < 0){
                printf("Kein Weg gefunden!\n");
                break;
            }

            // keine Möglichkeiten mehr -> gehe zurueck!
            cField = getFieldAt(walkCounter);
            cField->lastTry += 1;
            fieldPos = walkCounter;
        }else{
            cField->value = walkCounter;
            walkCounter += 1;
            cField = nextField;
            if (walkCounter == (MAX_BOARD_FIELD - 1)) {
                cField->value = walkCounter;
            }
        }
        //printf("Aktuelle Feldposition: %d\n", fieldPos);
    }
    // end walk
}*/

void prettyPrintedWalk(Field argField) {
    printf("Springer laeuft von %c%c aus los...\n", argField.desc[0], argField.desc[1]);
    walkAndGoToStartPos(argField);
    printf("\n");
    printBoard();
    printf("\n... laufen abgeschlossen.");
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
    if (startField.row == -1) {
        for (int rowI = 0; rowI < MAX_BOARD_SIZE; rowI++) {
            for (int colI = 0; colI < MAX_BOARD_SIZE; colI++) {
                Field startField;
                startField.column = colI;
                startField.row = rowI;
                startField.desc[0] = startField.column + 65;
                startField.desc[1] = (startField.row + 1) +'0';
                prettyPrintedWalk(startField);
                if(!(rowI == MAX_BOARD_SIZE - 1 && colI == MAX_BOARD_SIZE - 1)){
                    initBoard();
                    printf("\n\nSchachbrett neu initialisiert\n");
                }
            }
        }
    } else {
        prettyPrintedWalk(startField);
    }
    return 0;
}

