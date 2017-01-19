#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include <windows.h>

/**
 * Springerproblem
 * Marc Reinke, Daniel Abel, Alexander Krieg
 **/

/* ************ Nice to have ***********
- Ausgabe in Textdatei
***************************************/


#define MAX_BOARD_SIZE 8 // Sets the max number of fields in the board
#define MAX_COLUMN (MAX_BOARD_SIZE + 64) // used for converting from number to char
#define MAX_BOARD_FIELD (MAX_BOARD_SIZE*MAX_BOARD_SIZE) // maximum fields

/**
 * ANSI_COLORS for marking first and last field.
 **/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define FOREGROUND_WHITE   7

/**
 * print carriage return
 */
#define CRLF printLogOut("\n");

/**
 * Struct Field
 * Represents one field on the chessboard
 * @property row The row index of the board.
 * @property column The column index of the board.
 * @property value The counter value of the knight's steps.
 * @property cReachableFields Number of possible followers.
 * @property reachableFields References to the possible followers.
 * @property lastTry Index of last tried follower.
 * @property desc Column-Row representation of the field in the borad (human readable)
 **/
struct Field {
    int row;
    int column;
    int value;
    int cReachableFields;
    void* reachableFields[8];
    int lastTry;
    char desc[2];
};
typedef struct Field Field;

/**
 * Struct Chessboard
 * Holds all fields in an array.
 **/
struct Chessboard {
    struct Field fields[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};
typedef struct Chessboard Chessboard;

/**
 * Global variable board
 **/
Chessboard board;

FILE *logFile;
/**
 * Reads and verifies the user's input to get the
 * start position.
 * @return Initialized Field with row and column values.
 */
Field getStartPosition() {

    Field retVal;

    printLogOut("Startposition: \n");
    int inputVerifyer = 0;
    char inputCol;
    do{
        printLogOut("Bitte Spalte eingeben (A-%c ; X fuer alle): \n", MAX_COLUMN);
        scanf("%c", &inputCol);
        inputCol = toupper(inputCol);
        if (inputCol == 'X') {
            retVal.column = -1;
            retVal.row = -1;
            return retVal;
        }

        inputVerifyer = (inputCol >= 'A' && inputCol <= MAX_COLUMN);
        if(inputVerifyer == 0){
            printLogOut("Eingabe falsch.\n");
        }
    }while( inputVerifyer == 0);
    retVal.column = inputCol - 65; // Convert char-column to number-columnt, e.q. A to 0

    int inputRow;
    inputVerifyer = 0;
    do{
        printLogOut("Bitte Reihe eingeben (1-%d): ", MAX_BOARD_SIZE);
        scanf("%d", &inputRow);
        inputVerifyer = (inputRow >= 1 && inputRow <= MAX_BOARD_SIZE);
        if(inputVerifyer == 0){
            printLogOut("Eingabe falsch.\n");
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

    /*
    printLogOut("## Nachfolger von %s unsortiert: \n", argField->desc);
    for (int pos = 0; pos < argField->cReachableFields; pos++){
        printLogOut("%s -> mit %d Nachfolgern \n", ((Field *)argField->reachableFields[pos])->desc,((Field *)argField->reachableFields[pos])->cReachableFields);
    }
    printLogOut("##");
    */

    for (int posOut = 0; posOut < argField->cReachableFields - 1; posOut++){
        for (int posIn = 0; posIn < (argField->cReachableFields - posOut - 1); posIn++){
            Field* fieldPointer = argField->reachableFields[posIn];
            Field* fieldPointer2 = argField->reachableFields[posIn+1];
            if (fieldPointer->cReachableFields > fieldPointer2->cReachableFields) {
                void* tmp = argField->reachableFields[posIn];
                argField->reachableFields[posIn] = argField->reachableFields[posIn+1];
                argField->reachableFields[posIn+1] = tmp;
            }
        }
    }

    /*
    printLogOut("## Nachfolger von %s sortiert: \n", argField->desc);
    for (int pos = 0; pos < argField->cReachableFields; pos++){
        printLogOut("%s -> mit %d Nachfolgern \n", ((Field *)argField->reachableFields[pos])->desc,((Field *)argField->reachableFields[pos])->cReachableFields);
    }
    printLogOut("## \n\n");
    */
}

/**
 * Verifies if a specific row and column couple describes a
 * valid field in the board size.
 * @param argColumn The column index to check.
 * @param argRow The row index to check.
 * @return Retuns 1 if the column and row describe a valid field, otherwise 0.
 */
int verifyFeld(int argColumn, int argRow){
    if(argColumn >= 0 && argColumn < MAX_BOARD_SIZE && argRow >= 0 && argRow < MAX_BOARD_SIZE){
        return 1;
    }
    return 0;
}

/**
 * Initializes the reachable followers of one field.
 * @param argField A field pointer to the field whose followers will be initialized.
 */
void initPossibleFollowersForField(Field *argField){
    int counter = 0;
    int newCol;
    int newRow;
    for (int a = 1; a <= 8; a++) {
        if (a <= 4) {
            newRow = 2;
            newCol = 1;
        } else {
            newRow = 1;
            newCol = 2;
        }
        if (a <= 2 || a == 5 || a == 6) {
            newRow = newRow*-1;
        }
        if (a%2==0) {
            newCol = newCol*-1;
        }
        newCol = argField->column + newCol;
        newRow = argField->row + newRow;
        if(verifyFeld(newCol, newRow) != 0){
            argField->reachableFields[counter] = &board.fields[newRow][newCol];
            counter += 1;
        }
    }
    argField->cReachableFields = counter;
}

/**
 * Initializes the reachable followers of one field.
 * @param argField A field pointer to the field whose followers will be initialized.

void initPossibleFollowersForField(Field *argField){
    int counter = 0;
    int startRow = argField->row;
    int startCol = argField->column;

    int newCol = startCol + 2;
    int newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->reachableFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->reachableFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow + 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->reachableFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 2;
    newRow = startRow - 1;
    if(verifyFeld(newCol, newRow) != 0){
        argField->reachableFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->reachableFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol + 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->reachableFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow + 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->reachableFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }
    newCol = startCol - 1;
    newRow = startRow - 2;
    if(verifyFeld(newCol, newRow) != 0){
        argField->reachableFields[counter] = &board.fields[newRow][newCol];
        counter += 1;
    }

    argField->cReachableFields = counter;

}
 */
/**
 * Initializes the global variable 'board' and it's fields.
 */
void initBoard(){

    // initalize fields
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
    // initialize rechable followers
    for (int row = 0; row < MAX_BOARD_SIZE; row++){
        for(int col = 0; col < MAX_BOARD_SIZE; col++){
            Field *fieldPointer = &board.fields[row][col];
            initPossibleFollowersForField(fieldPointer);
        }
    }
    // sort reachable followers
    for (int row = 0; row < MAX_BOARD_SIZE; row++){
        for(int col = 0; col < MAX_BOARD_SIZE; col++){
            Field *fieldPointer = &board.fields[row][col];
            sortPossibleFollowerArray(fieldPointer);
        }
    }
}

/**
 * Prints the letters of the chessboard.
 */
void printBoardHead(){
    printLogOut("   ");
    for(int h = 65; h < 65 + MAX_BOARD_SIZE; h++){
        printLogOut(" %c", h);
        if(h != MAX_BOARD_SIZE - 1){
            printLogOut("|");
        }
    }
    CRLF
}

/**
 * Prints the global variable 'board' with the knight's steps.
 * -1 => The knight did not reach the field yet.
 */
void printBoard(){
    printBoardHead();
    for (int row = 0; row < MAX_BOARD_SIZE; row++){
        printLogOut("%d |", row + 1);
        for(int col = 0; col < MAX_BOARD_SIZE; col++){
            // print startfield green and last field red
            if(board.fields[row][col].value == 0){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN );
                printLogOut("%02d", board.fields[row][col].value);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_WHITE);
                printLogOut("|");
            }else if(board.fields[row][col].value == MAX_BOARD_FIELD - 1){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED );
                printLogOut("%02d", board.fields[row][col].value);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_WHITE);
                printLogOut("|");
            }else{
                printLogOut("%02d|", board.fields[row][col].value);
            }
        }
        printLogOut("  %d", row + 1);
        CRLF
    }
    printBoardHead();
}


/**
 * Searches for a field with a specific knight step(=>field.value) in board.
 * @param argPosition The knight's step value to search for.
 * @return Returns a field pointer to the searched field. If no field was found returns NULL.
 */
Field* getFieldAt(int argPosition) {
    for (int i = 0; i < MAX_BOARD_SIZE; i++) {
        for (int j = 0; j < MAX_BOARD_SIZE; j++) {
            if (board.fields[i][j].value == argPosition) {
                return (Field*) &(board.fields[i][j]);
            }
        }
    }
    return NULL;
}

/** für Aufgabenteil c
 * Starts the knight's walk and searches for a way where the knight
 * can jump from the last position to the startfield back again.
 * @param argStartField The field from which the walk starts.
 */
double walkAndGoToStartPos(Field argStartField){
    int walkCounter = 0;
    SYSTEMTIME t;
    SYSTEMTIME t1;
    GetSystemTime(&t);
    Field* cField = &(board.fields[argStartField.row][argStartField.column]);
     // start walk
    for(int fieldPos= 0 ; fieldPos < MAX_BOARD_FIELD; fieldPos++){
        int foundnextField = -1;
        Field* nextField;
        do{
            // Check if the cField has no more reachableFields; if true cancel the walk for the current field.
            if(cField->lastTry >= cField->cReachableFields){
                cField->lastTry = 0;
                break;
            }

            // Knight walks to the next field.
            void* fieldAdress = cField->reachableFields[cField->lastTry];
            nextField = (Field *)fieldAdress;
            if(nextField->value == -1){
                foundnextField = 1;
            }else{
                cField->lastTry += 1;
            }
        }while(foundnextField == -1);

        // If no reachable field was found for that field, move one field back.
        if(foundnextField == -1){
            cField->value = -1;
            walkCounter--;
            // Exitcondition
            if(walkCounter < 0){
                printLogOut("Kein Weg gefunden!\nFuer dieses Startfeld gibt es keine Loesung.\n");
                break;
            }
            cField = getFieldAt(walkCounter);
            cField->lastTry += 1;
            fieldPos = walkCounter;
        }else{
            // If reachable field was found for that field, move to next field.
            cField->value = walkCounter;
            walkCounter += 1;
            cField = nextField;
            // If knight reached the last empty field of board.
            if (walkCounter == MAX_BOARD_FIELD - 1) {
                cField->value = walkCounter;
                int isStartFieldReachable = -1;
                // Check if startfield is reachable.
                for (int posI = cField->lastTry; posI < cField->cReachableFields; posI++) {
                    void* fieldAdress = cField->reachableFields[posI];
                    nextField = (Field *)fieldAdress;
                    if(nextField->value == 0){
                        isStartFieldReachable = 1;
                    }
                }
                // If startfield isn't reachable move one field back.
                if (isStartFieldReachable == -1) {
                    cField->value = -1;
                    walkCounter--;
                    cField = getFieldAt(walkCounter);
                    cField->lastTry += 1;
                    fieldPos = walkCounter;
                }
            }
        }
    } //end for(int fieldPos= 0 ; fieldPos < MAX_BOARD_FIELD; fieldPos++)
    GetSystemTime(&t1);
    double elapsedMs=((t1.wMilliseconds)-(t.wMilliseconds));
    if ((t1.wSecond-t.wSecond)!= 0){
        if ((t1.wMinute-t.wMinute)!= 0){
            if ((t1.wHour-t.wHour)!= 0){
                    elapsedMs = elapsedMs + ((((t1.wHour-t.wHour)*1000)*60)*60);
            }
            elapsedMs = elapsedMs + (((t1.wMinute-t.wMinute)*1000)*60);
        }
        elapsedMs = elapsedMs + ((t1.wSecond-t.wSecond)*1000);
    }
    return elapsedMs;
}

/** für Aufgabenteil b
 * Starts the knights walk.
 * @param argStartField The field from which the walk begins.
 */
/*void walk(Field argStartField){
    int walkCounter = 0;

    Field* cField = &(board.fields[argStartField.row][argStartField.column]);
     // start walk
    //return;
    for(int fieldPos= 0 ; fieldPos < MAX_BOARD_FIELD; fieldPos++){
        int foundnextField = -1;
        Field* nextField;
        do{
            if(cField->lastTry >= cField->cReachableFields){
                cField->lastTry = 0;
                break;
            }

            void* fieldAdress = cField->reachableFields[cField->lastTry];
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
            //printLogOut("WalkCounter = %d \n", walkCounter);
            if(walkCounter < 0){
                printLogOut("Kein Weg gefunden!\n");
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
        //printLogOut("Aktuelle Feldposition: %d\n", fieldPos);
    }
    // end walk
}*/

/**
 * Starts the walk from a field.
 * @param argField The field where the walk begins.
 */
void startWalkFromField(Field argField) {
    printLogOut("Springer laeuft von %c%c aus los...\n", argField.desc[0], argField.desc[1]);
    double walkTime= walkAndGoToStartPos(argField);
    CRLF
    printBoard();
    printLogOut("\n... laufen innerhalb von %0.0lf Millisekunden abgeschlossen.", walkTime);
}

/**
 * Starts logging content to file
 */
void startLogging() {
    logFile = fopen("Springer.txt", "w+");
}

/**
 * writes to Logfile and STDOUT
*/
void printLogOut(const char *restrict msg, ...)
{
    va_list args;

    va_start(args, msg);
    vfprintf(logFile, msg, args);
    va_end(args);
    va_start(args, msg);
    vfprintf(stdout, msg, args);
    va_end(args);
}

/**
 * Starts logging content to file
 */
void stopLogging() {
    fclose(logFile);
}

int startTimestamp(){
    time_t now;
    struct tm *tm;

    now = time(0);
    if ((tm = localtime (&now)) == NULL) {
        printLogOut("Zeit konnte nicht initialisiert werden!\n");
        return 1;
    }
    printf ("%04d-%02d-%02d %02d:%02d:%02d\n",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec);
    return 0;
}

/**
 * Starts the programm.
 */
int main()
{
    startLogging();
    printLogOut("\n##### Springerproblem #####\n");
    initBoard();
    printLogOut("\n\nSchachbrett initialisiert: \n\n");
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
                startWalkFromField(startField);
                if(!(rowI == MAX_BOARD_SIZE - 1 && colI == MAX_BOARD_SIZE - 1)){
                    initBoard();
                    printLogOut("\n\nSchachbrett neu initialisiert\n");
                }
            }
        }
    } else {
        startWalkFromField(startField);
    }
    stopLogging();
    return 0;
}

