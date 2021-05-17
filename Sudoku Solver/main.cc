#include <iostream>
#include "point.h"
#include <stack>
#define N 9

using namespace std;

char board[N][N];
unsigned short int status[N][N]; //array parallel to board[][]
const unsigned short int VALUE_MASK = 0x000f;
stack<point> stack1;

int countBits(unsigned int n) {
  int count = 0;
  
  while (n != 0) {
    count++;
    n &= n - 1;
  }
  
  return count;
}

bool FindBest(void) {

  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      if ((status[i][j] >> 13) == 0) {
         status[i][j] = 0x1ff0;
      }
    }
  }

  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      if ((status[i][j] >> 13) == 1) {
        //masking the cells in same row
        for (int b=0; b<N; b++) {
if ((status[i][b] >> 13) == 0)
            status[i][b] &= ~(1 << ((status[i][j] & 0xf) + 3)); 
        } 

        //masking the cells in same column
        for (int a=0; a<N; a++) {
if ((status[a][j] >> 13) == 0)
            status[a][j] &= ~(1 << ((status[i][j] & 0xf) + 3)); 
        }

        //masking the cells in same block
        int x=3*(i/3),y=3*(j/3);
        for (int a=x; a<=x+2; a++) {
          for (int b=y; b<=y+2; b++) {
if ((status[a][b] >> 13) == 0)
            status[a][b] &= ~(1 << ((status[i][j] & 0xf) + 3));
          }
        }

      }
    }
  }

  int low = 10, i_best, j_best;
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++)  {
      if ((status[i][j] >> 13) == 0) {
        int count = countBits(status[i][j] & 0x1ff0);
          if (count < low) {
          low = count;
          i_best = i;
          j_best = j;
        }
      }
    }
  }
  
  if (low == 10) {
    return false;
  }

  status[i_best][j_best] |= 0x2000; //mark as 'filled'
  stack1.push(point(i_best,j_best));
  return true;
}

void Solve(void) {
  FindBest();
  while (true) {
    int valid_choice = 0;
    for (int m=0; m<9; m++) {
      if ((((status[stack1.top().xValue()][stack1.top().yValue()] & 0x1ff0) >> (m+4)) ) == 1) {
        valid_choice = m+1;
        status[stack1.top().xValue()][stack1.top().yValue()] &= (~(1 << (m+4)));
        status[stack1.top().xValue()][stack1.top().yValue()] &= ~VALUE_MASK;
        status[stack1.top().xValue()][stack1.top().yValue()] |= valid_choice;
      }
    }

    if (valid_choice == 0) {
      status[stack1.top().xValue()][stack1.top().yValue()] = 0; //mark as not filled
      stack1.pop();
      if (stack1.empty()) {
        break;
      }
      continue;
    }
    if (!FindBest())
      break;
  } 
}


  int main () {
  cout << "Input the sudoku: " <<endl;

  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      cin >> board[i][j];
    }
  }

  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      
      if (board[i][j] == '.') {
        status[i][j] = 0x1ff0;
      }
      if (board[i][j] >= 49 & board[i][j] <= 57) {
        status[i][j] = (0x2000 | (board[i][j] - 48));
      }
    }
  }

  Solve();
  cout << "Solution: " << endl;
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      if (board[i][j] == '.') {
        int temp = (status[i][j] & 0xf);
        board[i][j] = temp + 48;
      }
      cout << board[i][j];
      if ((j==2)|(j==5)) {
        cout << " ";
      }
    }
    cout << endl;
    if ((i==2)|(i==5)) {
      cout << endl;
    }
  }

  return 0;
}
