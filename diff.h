#define MAXLINES 1000
#define MAXLENGTH 256
typedef struct instructions {
    int operation;
    int left;
    int right;
    char line[MAXLENGTH];
} instructions;
typedef struct LineDetails {
    int operation;
    int leftStartPos;
    int leftEndPos;
    int rightStartPos;
    int rightEndPos;
    char line[MAXLENGTH];
} LineDetails;
int fileLength1;
int fileLength2;
int FLAG_BRIEF;
int FLAG_IGNORECASE;
int FLAG_REPORT;
int FLAG_SIDEBYSIDE;
int FLAG_LEFTCOLUMN;
int FLAG_SUPPRESS;
int instructionInfoLength;
int file1count, file2count;
instructions instructionInfo[MAXLINES * 2];
LineDetails construct_DiffLines(int op, int ls, int le, int rs, int re);
void diff_print(LineDetails info_arr[], int arr_length, char* arr1[], char* arr2[]);
void sbsprint(LineDetails info_arr[], char* arr1[], char* arr2[]);
void init(void);
void addInstruction(int op, int left, int right, char* line);
int readFile(char* arr[], char* name, int lineCount);
void freeMemory(char* arr[], int linecount);
void insertIntoTable(int table[1000][1000], char* arr1[], char* arr2[]);
void diff(int table[1000][1000], char* arr1[], char* arr2[], int col, int row);
int formatLines(LineDetails lines_arr[]);

