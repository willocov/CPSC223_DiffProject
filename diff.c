//William Covington
//CPSC 223C
//Diff Project
#include <ctype.h>
#include "diff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max(a, b) (a > b ? a : b)
int main(int argc, char* argv[]) 
{
    char first_priority = 0;
    char file1[MAXLENGTH], file2[MAXLENGTH];   // File Buffers
    char* fileLines1[MAXLINES];      // String arrays holding strings pertaining to their files
    char* fileLines2[MAXLINES];
    init();
    if (argc == 1) {
        printf("Error: Not Enough Arguments\n");
        return 1;}
    for (int i = 1; i < argc - 2; i++) {
        char flag[MAXLENGTH]; 
        strcpy(flag , argv[i]); //Copies argument to flag for later comparision
        if (strcmp(flag, "-v") == 0) {
            printf("Diff Version 2.0\n");
            printf("License Number: 420\n");
            printf("Written by: William Covington\n");
            return 0;}
        else if (strcmp(flag, "-q") == 0)   FLAG_BRIEF = 1;
        else if (strcmp(flag, "-i") == 0)   FLAG_IGNORECASE = 1;
        else if (strcmp(flag, "-s") == 0)   FLAG_REPORT = 1;
        else if (strcmp(flag, "-y") == 0) {
            FLAG_SIDEBYSIDE = 1;
            if (first_priority == 0) 
                first_priority = 'y';}
        else if (strcmp(flag, "--left-column") == 0)    FLAG_LEFTCOLUMN = 1;
        else if (strcmp(flag, "--suppress-common-lines") == 0) FLAG_SUPPRESS = 1;
        else if (strcmp(flag, "-c") == 0 || strcmp(flag, "-u")) {printf("ERROR: This function has not been implemented.\n"); return 0;}
        else {printf("Error: Invalid Flag Found.\n"); return 1;}
    }
    if (first_priority == 0) first_priority = 'd';
    strcpy(file2, argv[argc - 2]);
    strcpy(file1, argv[argc - 1]);
    fileLength1 = readFile(fileLines1, file1, file1count);
    fileLength2 = readFile(fileLines2, file2, file2count);
    int table[1000][1000];
    insertIntoTable(table, fileLines1, fileLines2);
    diff(table, fileLines1, fileLines2, fileLength1, fileLength2);
    LineDetails lines_info[MAXLINES * 2];
    int lines_info_length = formatLines(lines_info);
    if(first_priority == 'y') sbsprint(lines_info, fileLines2, fileLines1);
    else if(first_priority == 'd') diff_print(lines_info, lines_info_length, fileLines1, fileLines2);
    freeMemory(fileLines1, file1count);
    freeMemory(fileLines2, file2count);
    return 0;
}

void init(){
    fileLength1 = fileLength2 = FLAG_BRIEF = FLAG_IGNORECASE = FLAG_REPORT = FLAG_SIDEBYSIDE = FLAG_LEFTCOLUMN = FLAG_SUPPRESS = instructionInfoLength = 0;
}

int readFile(char* arr[], char* name, int lineCount)
{
    FILE *file;
    char line[MAXLENGTH];
    int i = 0;
    lineCount = 0;
    file = fopen(name, "r");
    if (file == NULL) {
        printf("ERROR: Can't Open File: %s\n", name);
        exit(1);}
    while (fgets(line, MAXLENGTH, file) != NULL) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';  //Removes newline char from end of string
        if (FLAG_IGNORECASE)
            for (int j = 0; j < (int)strlen(line); j++)
                line[j] = (char)toupper(line[j]);
        arr[i] = malloc(sizeof(line));
        lineCount++;
        strcpy(arr[i], line);
        i++;}
    fclose(file);
    return i;
}

void freeMemory(char* arr[], int lineCount)
{
    for (int i = 0; i < lineCount; i++) {
        if (arr[i] == NULL) return;
        free(arr[i]);}
}

void insertIntoTable(int table[1000][1000], char* arr1[], char* arr2[])
{
    for (int i = 0; i <= fileLength1; i++)  table[0][i] = 0;
    for (int i = 0; i <= fileLength2; i++)  table[i][0] = 0;
    for (int i = 1; i <= fileLength2; i++) {
        for (int j = 1; j <= fileLength1; j++) {
            if (strcmp(arr1[j - 1], arr2[i - 1]) == 0)  table[i][j] = table[i - 1][j - 1] + 1;
            else    table[i][j] = max(table[i - 1][j], table[i][j - 1]);}}
}

void diff(int table[1000][1000], char* arr1[], char* arr2[], int col, int row)
{ 
    int differentBool = 0;
    int sameBool = 1;
    while(row > 0 || col > 0) {
        if (row > 0 && col > 0 && strcmp(arr1[col - 1], arr2[row - 1]) == 0) {
            col--;
            row--;}
        else if (col > 0 && row == 0) {
            addInstruction('a', row, col, arr1[col - 1]);
            col--;
            differentBool = 1;
            sameBool = 0;}
        else if (col > 0 && table[row][col - 1] > table[row - 1][col]) {
            addInstruction('a', row, col, arr1[col - 1]);
            col--;
            differentBool = 1;
            sameBool = 0;}
        else if (col > 0 && table[row][col - 1] == table[row - 1][col]) {
            addInstruction('c', row, col, arr1[col - 1]);
            col--;
            row--;
            differentBool = 1;
            sameBool = 0;}
        else if (row > 0 && col == 0) {
            addInstruction('d', row, col + 1, arr2[row - 1]);
            row--;
            differentBool = 1;
            sameBool = 0;}
        else if (row > 0 && table[row][col - 1] < table[row - 1][col]) {
            addInstruction('d', row, col + 1, arr2[row - 1]);
            row--;
            differentBool = 1;
            sameBool = 0;}}
    if (FLAG_BRIEF) {
        if (differentBool) {
            printf("The files are different\n");
            exit(0);}}
    if (sameBool && FLAG_REPORT) {
        printf("The files are the same\n");
        exit(0);}
    instructions tempArr[MAXLINES * 2];
    for (int i = 0; i < instructionInfoLength; i++) tempArr[i] = instructionInfo[i];
    int arr_counter = 0;
    for (int i = instructionInfoLength - 1; i >= 0; i--) instructionInfo[arr_counter++] = tempArr[i];
}

void addInstruction(int op, int left, int right, char* line) 
{
    instructions newInstruction;
    strcpy(newInstruction.line, line);
    newInstruction.left = left;
    newInstruction.right = right;
    newInstruction.operation = op;
    instructionInfo[instructionInfoLength++] = newInstruction;
}

LineDetails DiffLines(int op, int ls, int le, int rs, int re) {
    LineDetails newDetails;
    newDetails.operation = op;
    newDetails.leftStartPos = ls;
    newDetails.leftEndPos = le;
    newDetails.rightStartPos = rs;
    newDetails.rightEndPos = re;
    return newDetails;
}

int formatLines(LineDetails lines_arr[])
{
    int lastOperation = 0, leftStartPosition = 0, leftEndPosition = 0, rightStartPosition = 0, rightEndPosition = 0, length = 0;
    for (int i = 0; i < instructionInfoLength; i++) {
        instructions tempInstructions = instructionInfo[i];
        if (lastOperation == 0 || lastOperation != tempInstructions.operation || (leftEndPosition + 1 != tempInstructions.left && tempInstructions.operation != 'a') || (rightEndPosition + 1 != tempInstructions.right && tempInstructions.operation != 'd')) {
            if (lastOperation != 0) {
                if (leftEndPosition - leftStartPosition > 0 ) {;}
                else leftEndPosition = -1;
                if (rightEndPosition - rightStartPosition > 0) lines_arr[length++] = DiffLines(lastOperation, leftStartPosition, leftEndPosition, rightStartPosition, rightEndPosition);
                else{
                    rightEndPosition = -1;
                    lines_arr[length++] = DiffLines(lastOperation, leftStartPosition, leftEndPosition, rightStartPosition, rightEndPosition);}}
            leftStartPosition = tempInstructions.left;
            rightStartPosition = tempInstructions.right;
            leftEndPosition = tempInstructions.left;
            rightEndPosition = tempInstructions.right;}
        else {
            leftEndPosition = tempInstructions.left;
            rightEndPosition = tempInstructions.right;}
        lastOperation = tempInstructions.operation;
        if (i == instructionInfoLength - 1) {
            if (leftEndPosition - leftStartPosition > 0) {;}
            else
                leftEndPosition = -1;
                if (rightEndPosition - rightStartPosition > 0) lines_arr[length++] = DiffLines(lastOperation, leftStartPosition, leftEndPosition, rightStartPosition, rightEndPosition);
                else {
                    rightEndPosition = -1;
                    lines_arr[length++] = DiffLines(lastOperation, leftStartPosition, leftEndPosition, rightStartPosition, rightEndPosition);}}}
    return length;
}

void diff_print(LineDetails info_arr[], int info_arr_length, char* arr1[], char* arr2[])
{
    for (int i = 0; i < info_arr_length; i++) {
        LineDetails tempDetail = info_arr[i];
        if (tempDetail.leftEndPos != -1)    printf("%i,%i%c", tempDetail.leftStartPos, tempDetail.leftEndPos, tempDetail.operation);
        else    printf("%i%c", tempDetail.leftStartPos, tempDetail.operation);
        if (tempDetail.rightEndPos != -1)   printf("%i,%i\n", tempDetail.rightStartPos, tempDetail.rightEndPos);
        else    printf("%i\n", tempDetail.rightStartPos);
        if (tempDetail.rightEndPos == -1)   tempDetail.rightEndPos = tempDetail.rightStartPos;
        if (tempDetail.leftEndPos == -1)    tempDetail.leftEndPos = tempDetail.leftStartPos;
        switch ((char)tempDetail.operation) {
            case 'a':
                if ((!FLAG_SUPPRESS && i < fileLength2) && i != 0) printf("> %s\n", arr1[i]);
                for (int j = tempDetail.rightStartPos; j <= tempDetail.rightEndPos; j++)
                    printf("> %s\n", arr1[j - 1]);
                if ((!FLAG_SUPPRESS && i < fileLength2) && i != 0) printf("> %s\n", arr1[i]);
                break;
            case 'd':   
                for (int j = tempDetail.leftStartPos; j <= tempDetail.leftEndPos; j++)
                    printf("< %s\n", arr2[j - 1]);
                break;
            case 'c':
                for (int j = tempDetail.leftStartPos; j <= tempDetail.leftEndPos; j++) printf("< %s\n", arr2[j - 1]);
                printf("---\n");
                for (int j = tempDetail.rightStartPos; j <= tempDetail.rightEndPos; j++) printf("> %s\n", arr1[j - 1]);
                break;}}
}

void sbsprint(LineDetails info_arr[], char* arr1[], char* arr2[])
{
    int longest_length = max(fileLength1, fileLength2);
    int counter = 0;
    int first_run = 1;
    LineDetails temp = info_arr[counter];
    for (int i = 0; i < longest_length - 1; i++) {
        if (i + 1 == temp.leftStartPos || first_run == 1) {
            if (temp.rightEndPos == -1) temp.rightEndPos = temp.rightStartPos;
            if (temp.leftEndPos == -1) temp.leftEndPos = temp.leftStartPos;
            int holder1 = temp.rightStartPos;
            switch ((char)temp.operation) {
                case 'a':   
                    if ((!FLAG_SUPPRESS && i < fileLength2) && i != 0) {
                        if (FLAG_LEFTCOLUMN) printf("%-32s%c\n", arr1[i], '(');
                        else printf("%-34s%s\n", arr1[i], arr1[i]);}
                    for (int j = temp.rightStartPos; j <= temp.rightEndPos; j++)
                        printf("\t\t\t\t> %s\n", arr2[j - 1]);
                    if ((!FLAG_SUPPRESS && i < fileLength2) && i == 0) {
                        if (FLAG_LEFTCOLUMN) printf("%-32s%c\n", arr1[i], '(');
                        else printf("%-34s%s\n", arr1[i], arr1[i]);}
                    break;
                case 'd':   
                    for (int j = temp.leftStartPos; j <= temp.leftEndPos; j++) {
                        printf("%-32s<\n", arr1[j - 1]);
                        i++;}
                    break;
                case 'c':
                    for (int j = temp.leftStartPos; j <= temp.leftEndPos; j++) {
                        printf("%-32s| %s\n", arr1[j - 1], arr2[holder1 - 1]);
                        holder1++;
                        i++;}
                    break;}
            temp = info_arr[++counter];
            first_run = 0;}
        else {
            if ((!FLAG_SUPPRESS && i < fileLength2)) {
                if (FLAG_LEFTCOLUMN) printf("%-32s%c\n", arr1[i], '(');
                else printf("%-34s%s\n", arr1[i], arr1[i]);}}}
}
//this shit bangs, yo!
