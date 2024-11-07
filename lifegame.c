#define board_size 40 //Globally defined variable that I use for the rest of the code

void initialize_board(char arg[board_size][board_size]);
void boarder(char arg[][board_size]);
void add_cell(char arg[][board_size], int, int);
void remove_cell(char arg[][board_size], int,int);
void update_board(char arg[][board_size], char temp_arg[][board_size]);
void print_board(char arg[][board_size]);
int count_neighbors(char arg[][board_size],int,int);
void itterate_board(char arg[][board_size]);
void game_mode(FILE *fp); //Create all my funcition prototypes


//Bryan Escalante
//playlife.c

#include <stdio.h>
#include <string.h>


#include "lifefunc.h"

int main(int argc, char *argv[]) {//argc and argv to see if user wants to include file
    
    if (argc == 1){
        game_mode(NULL);//If there is no file play the game with user input
    } else if (argc == 2){
        char fileName[30];//If there is a file store the name and try to open it
        strcpy(fileName,argv[1]);//If file doesn't exist return error message
        FILE *fp;
        fp = fopen(fileName,"r");
        if (!fp){
            printf("Can't open the %s file\n",fileName);
            return 1;
        }
        game_mode(fp);//Play the game with the instructions from the file
    } else {
        printf("Too many arguements.\n");//Print if input was invalid
    }
    
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Include all the libraries I will use within the code
#include "lifefunc.h"  // Include the header with board_size


void initialize_board(char board[board_size][board_size]){ //Function to initalize boarder
    for(int i = 0; i < board_size;i++){
        for(int j = 0; j < board_size;j++){
            board[i][j] = ' '; //Go through each index in the board and set them to empty
        }
    }
}

void boarder(char board[][board_size]) { //Function to print board along with borderer
    for (int i = 0; i < board_size + 2; i++) {
        printf("- "); //Create the top boarder of the board + 2 to make sure their is no empty space
    }
    printf("\n"); //New line for the actual board

    for (int i = 0; i < board_size; i++) {
        printf("|"); //Left side boarder
        for (int j = 0; j < board_size; j++) {
            printf("%c ", board[i][j]); //Print out all the spaces from the acutal board
        }
        printf(" |"); //Right side of the boarder
        printf("\n"); //New line for bottom boarder
    }

    for (int i = 0; i < board_size + 2; i++) {
        printf("- ");//Bottom boarder
    }
    printf("\n"); //New line for readability
}

// Function to add cell at specified position
void add_cell(char board[][board_size], int row, int col) {
    if (row >= 0 && row < board_size && col >= 0 && col < board_size) { //Make sure it is in bounds
        if (board[row][col] == ' ') { //Check if it is already empty 
            board[row][col] = 'x'; 
            printf("Added cell at (%d, %d)\n", row, col); //Let user know cell was added
        } else {
            printf("Cell already alive at (%d, %d): current value = '%c'\n", row, col, board[row][col]);//Tell user if there was already a cell
        }
    } else {
        printf("Invalid position: (%d, %d)\n", row, col); //Let user know if it was out of bounds and can't be placed
    }
}

// Function to remove cell at specified position
void remove_cell(char board[][board_size], int row, int col) {
    if (row >= 0 && row < board_size && col >= 0 && col < board_size) { //Make sure it is in bounds
        if (board[row][col] == 'x') { //Check if the cell is alive
            board[row][col] = ' '; 
            printf("Removed cell at (%d, %d)\n", row, col);//Let user know it is removed
        } else {
            printf("Cell is already dead at (%d, %d): current value = '%c'\n", row, col, board[row][col]);//Let user know if it was already dead
        }
    } else {
        printf("Invalid position: (%d, %d)\n", row, col);//Let user know if position was invalid
    }
}

void update_board(char arg[][board_size],char temp_arg[][board_size]){ //Update the acutal board with the temp_board
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            arg[i][j] = temp_arg[i][j];//Loop through each index and assign temp_board to board for each one
        }
    }
}

int count_neighbors(char board[][board_size], int row, int col){//Function to find neighbors for an indinvidual cell
    int neighbors = 0;//Coutner to be returned at the end

    for(int i = row - 1; i <= row + 1; i++){//The double nested loops will go through a 3 by 3 grid
        for(int j = col - 1; j <= col + 1;j++){ 
            if(i == row && j ==col){ //If it is the middle point then it is the cell we are comparing to so don't count it
                continue;
            }
            if(i >= 0 && i < board_size && j >= 0 && j < board_size){//Check to make sure it doesn't check cells outside of bounds
                if(board[i][j] == 'x'){//Check to see if the cell is alive
                    neighbors++;//If all conditions are met then add one to neighbors
                }
            }
        }
    }

    return neighbors; //Return the count to be used later on
}


void itterate_board(char board[][board_size]){
    int neighbors = 0;//Variable to hold value from count_neighbors
    char temp_board[board_size][board_size];//Intalize a temp_board that will be updated and then set to normal board

    for(int i = 0; i < board_size;i++){
        for(int j = 0; j < board_size;j++){
            temp_board[i][j] = board[i][j];//Go through each index from the original board and set it to temp
        }
    }
    
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            neighbors = count_neighbors(board, i ,j); //Go through each cell one by one and count the amount of neighbors each cell has from the oringal board

            if(neighbors == 3 && temp_board[i][j] == ' '){ //If the cell is dead and has 3 neighbors then make it alive on temp
                temp_board[i][j] = 'x';
            } else if (temp_board[i][j] == 'x' && (neighbors != 2 && neighbors != 3)) {//If it is alive and has <2 or >3 neighbors kill it
                temp_board[i][j] = ' ';
              } 
        }
    }

    for(int i = 0; i < board_size;i++){
        for(int j = 0; j < board_size;j++){
            board[i][j] = temp_board[i][j];//Finally get all the new cells from temp_board.
            //Do this to make sure the game rules are followed and new cells added don't count until next itteration
        }
    }
}

void game_mode(FILE *fp){//Function that will decide which game mode was chosen
    char board[board_size][board_size];//Both board and temp board will be used for both versions of the game
    char temp_board[board_size][board_size];  
    char userInput[10];
    char letter;
    int row, col;//Userinput, letter, row, col will all be used for both versions as well
    initialize_board(temp_board);
    initialize_board(board); //Initalize both boards to 40x40 with empty spaces
    

    while (1) {
        if (fp == NULL){ //If the filepointer is empty then start asking the user for input
            printf("Enter an input (a for add, r for remove, n for advance, p for play, q for quit): ");
            if(fgets(userInput,sizeof(userInput),stdin) == NULL){
                break; //If userinput is null or their is some error reading standard input then break
            }
         } else {
            
            if(fgets(userInput,sizeof(userInput),fp) == NULL){
                    break; //If the fp is at the end or error reading then break
                }
            }   

            if (sscanf(userInput, "%c %d %d", &letter, &row, &col) == 3) {//Scan the line to see if there are 3 inputs 
                if (letter == 'a') {//If the first letter is a along with 2 valid inputs it will add the cell to that point
                    add_cell(temp_board, row, col);
                    boarder(temp_board);
                } else if (letter == 'r') {
                    remove_cell(temp_board, row, col);//If the first letter is r with 2 valid inputs it will remove the cell from that point
                    boarder(temp_board);
                }
            } else if (sscanf(userInput, "%c", &letter) == 1) {//Check if there is only 1 input
                if (letter == 'n') { //If letter is n then update the board to itterate the next round of the game
                    itterate_board(temp_board);
                    update_board(board, temp_board);
                    boarder(board);
                } else if (letter == 'q'){//If letter is q quit the game
                    printf("Okay ba bye!");
                    break;
                } else if (letter == 'p'){//If p then play the game without stopping
                    while(1){
                        itterate_board(temp_board);
                        update_board(board, temp_board);
                        boarder(board);
                        usleep(200);
                    }
                }
            } else {
                printf("Invalid input\n");//If they formatted wrong or just invalid input let user know
            }

        update_board(board, temp_board);
        printf("\n");
    }
}
