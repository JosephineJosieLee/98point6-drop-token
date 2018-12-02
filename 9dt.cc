// Josie Lee

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define NUM_COLS 4
#define BOARD_SIZE 16

// Attempts to put a token in the board at the given col
// if successful, col is added to the given list of successful puts
// Returns the current state of the game (1 if won, 2 if draw, 0 otherwise)
int putToken(int board[NUM_COLS][NUM_COLS], vector<int> &puts, int col);

// Prints the given list of columns where tokens have been successfully put to
// in chronological order
void getPuts(vector<int> puts);

// Prints the state of the given board
void printBoard(int board[NUM_COLS][NUM_COLS]);

// Prints the rules of the game
void howToPlay();

// Will check the given board to see if the token added at position (row, col) results in a win
// Assumes that no one has won before this token was added
bool isWin(int board[NUM_COLS][NUM_COLS], int col, int row);


int main(int argc, char **argv) {
  int board[NUM_COLS][NUM_COLS]{0}; // state of the board
  vector<int> puts;                 // successful puts
  int state;                        // state of the game:
                                    //  1, if won | 2, if draw | 0, otherwise
  string command;                   // current user command
  int col;                          // the column to put to

  while(1) {
    getline (cin, command);
    
    if (command == "EXIT") {
      return EXIT_SUCCESS;

    } else if (command == "GET") {
      getPuts(puts);

    } else if (command == "BOARD") {
      printBoard(board);

    } else if (command == "HELP") {
      howToPlay();

    } else if (command.substr (0,3) == "PUT") {
      // check to make sure the number is valid
      if ((sscanf(command.substr (3).data(), "%d", &col) != 1) ||
          (command.substr (3) != (" " + to_string(col))) ||
          (col < 1) ||(col > 4)) {
        cout << "PUT NEEDS VALID COLUMN NUMBER" << endl;
        cout << "(TYPE \"HELP\" FOR RULES)" << endl;

      // check to make sure game is not over
      } else if (state == 1) {
        cout << "GAME OVER : WON BY PLAYER " << 2 - (puts.size() % 2) << endl;
      } else if (state == 2) {
        cout << "GAME OVER : DRAW" << endl;

      // go ahead with the put
      } else {  
         state = putToken(board, puts, col);
      }

    } else { // Input was not a valid command
      cout << "INVALID COMMAND : TYPE \"HELP\" FOR RULES" << endl;
    }
  }
}

void getPuts(vector<int> puts) {
  for (unsigned int i = 0; i < puts.size(); i++) {
    cout << puts[i] << endl;
  }
}

void printBoard(int board[NUM_COLS][NUM_COLS]) {
  for (int i = 0; i < NUM_COLS; i++) {
    cout << "|";
    for (int j = 0; j < NUM_COLS; j++) {
      cout << " " << board[i][j];
    }
    cout << endl;
  }
  cout << "+--------" << endl;
  cout << "  1 2 3 4" << endl;
}

int putToken(int board[NUM_COLS][NUM_COLS], vector<int> &puts, int col) {
  // make sure there is room in the column
  if (board[0][col - 1] != 0) {
    // if the top row is full, then the whole column is full
    cout << "ERROR" << endl;
    return 0;
  } 

  // decide which player's turn it is
  int player = puts.size() % 2 + 1;
  
  // assign bottom-most available spot in col to player
  int row;
  for (row = NUM_COLS - 1; row >= 0; row--) {
    if (board[row][col - 1] == 0) {
      board[row][col - 1] = player;
      break;
    }
  }

  // add column number to list of successful puts
  puts.push_back(col);

  // print proper output and return proper state
  if (isWin(board, col - 1, row)) {
    // we have a winner!
    cout << "WIN" << endl;
    return 1;

  } else if (puts.size() == BOARD_SIZE) {
    // board is full, and no one has won
    cout << "DRAW" << endl;
    return 2;

  } else {
    // the game continues
    cout << "OK" << endl;
    return 0;
  } 
}

bool isWin(int board[NUM_COLS][NUM_COLS], int col, int row) {
  // check if the row has 4 in a row
  for (int i = 0; i < NUM_COLS; i++) {
    if (board[row][col] != board[row][i]) {
      break;
    }
    if (i == NUM_COLS - 1) {
      return true;
    }
  }

  // check if the column has 4 in a row, if possible
  if (row == 0) {
    // the last inserted token was in the top row,
    // so the column is full and may have 4 in a row
    for (int i = 1; i < NUM_COLS; i++) {
      if (board[row][col] != board[i][col]) {
        break;
      }
      if (i == NUM_COLS - 1) {
        return true;
      }
    }
  }

  //check both diagonals if they have 4 in a row, if possible
  // the "/" diagonal :
  if (row == col) {
    // the token is on the diagonal, so it might have 4 in a row
    for (int i = 0; i < NUM_COLS; i++) {
      if (board[row][col] != board[i][i]) {
        break;
      }
      if (i == NUM_COLS - 1) {
        return true;
      }
    }
  }

  // the "\" diagonal :
  // (note all of the positions in this diagonal have indicies that sum to 3)
  if ( row + col == NUM_COLS - 1) {
    // the token is on the diagonal, so it might have 4 in a row
    for (int i = 0; i < NUM_COLS; i++) {
      if (board[row][col] != board[(NUM_COLS - 1) - i][i]) {
        break;
      }
      if (i == NUM_COLS - 1) {
        return true;
      }
    }
  }

  // there are no instances of 4 in a row caused by the last token
  return false;
}

void howToPlay() {
  cout << "Commands :" << endl;
  cout << " - PUT <column> : puts a token in the specified column (1,2,3,or 4)" << endl;
  cout << "   -- outputs :" << endl;
  cout << "        OK   : token successfully added to board, " << endl;
  cout << "                no one has won & board is not full" << endl;
  cout << "        WIN  : token successfully added to board," << endl;
  cout << "                resulting in current player winning" << endl;
  cout << "        DRAW : token successfully added to board," << endl; 
  cout << "                resulting in a full board where no one has won" << endl;
  cout << "        ERROR: token not successfully added to board, the column is already full" << endl  << endl;
  cout << " - GET : gets list of columns that have been successfully put to" << endl << endl;
  cout << " - BOARD : shows the current state of the board where..." << endl;
  cout << "           -- 0 is an unfilled space" << endl;
  cout << "           -- 1 is player 1's token" << endl;
  cout << "           -- 2 is player 2's token" << endl << endl;
  cout << " - EXIT : ends the game" << endl;
}
