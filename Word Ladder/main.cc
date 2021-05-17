#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <queue>

using namespace std;

struct Word {
    int ptr = -1; //ptr = NULL
    string word;
    list<int> ptrList;

    string get_word() {return word;}
    void addToList(int x) {ptrList.push_back(x);}
};

Word wordArray[5757]; //array of Word objects

int hammingDist(string str1, string str2);

void genLadder(Word w1, Word w2); //to generate ladder 

int getIndex(Word w); //to get index of a word in wordArray[]


int main() {

    ifstream file("sgb-words.txt");
  // check if the file is open
    if(!file) {
        cout << "Couldn't read the file!\n";
        exit(1);
    }

  //if file is open,
  //read words from file into objects.word
    int i = 0;
    while(file){
        if(file.eof()) break;
        file >> wordArray[i].word;
        i++;
    }
    file.close();
    
  //check the word pairs with hamming distance 1
  //add one's index into another's ptrList
    for(int j=0; j<5757; j++) {
        for(int k=j+1; k<5757; k++) {
            if(hammingDist(wordArray[j].get_word(),wordArray[k].get_word()) == 1) {
                wordArray[j].addToList(k);
                wordArray[k].addToList(j); 
            }
        }
    }

  //read the input words
    string str1, str2;
    cout << "Enter two words:\n";
    cin >> str1 >> str2;

  //check if both the input words are in the list
  //if yes, generate the ladder
    int flag = 0;
    for (int c = 0; c < 5757; c++) {
        if(wordArray[c].get_word() == str1) {
            for(int d = 0; d < 5757; d++) {
                if(wordArray[d].get_word() == str2) {
                    flag++;
                    Word word1 = wordArray[c];
                    Word word2 = wordArray[d];
                    genLadder(word1, word2);
                    break;
                }
            }
         }  
    }

    if(flag == 0) cout << "\nError: word/s not in list!" << endl;

}


int hammingDist(string str1, string str2) {
    int hd = 0;
    for (int i=0; i<5; i++) {
        if(str1[i] != str2[i])
            hd++;
    }
    return hd;
}


int getIndex(Word w) {
    int index;
    for(index = 0; index < 5757; index++) {
        if(w.get_word() == wordArray[index].get_word())
        break;
    }
    return index;
}


void genLadder(Word w1, Word w2) {
    list<Word> S;
    queue<Word> Q;

    S.clear();
    Q.push(w2);

    Word w;
    while(!Q.empty()) {
      //dequeue Q into w
        w = Q.front(); 
        Q.pop();
    
      //for each word v in w.ptrList,
        for(list<int>::iterator iter = w.ptrList.begin(); iter != w.ptrList.end(); iter++) {
            if(wordArray[*iter].ptr == -1 && wordArray[*iter].get_word() != w2.get_word()) {
              //set index of w to v.ptr
                wordArray[*iter].ptr = getIndex(w); 
              //enqueue v in Q
                Q.push(wordArray[*iter]); 
            }
        }
    }

  //set the ptr value of wordArray[d] to its copy w1
    int d = getIndex(w1);
    w1.ptr = wordArray[d].ptr;

    if(w1.ptr != -1) {
        S.push_back(w1);
        w.ptr = w1.ptr;
        
        while(w.ptr != -1) {
          //append w = wordArray[w.ptr] to S
            S.push_back(wordArray[w.ptr]);
          //set w to the word w.ptr points to 
            w = wordArray[w.ptr];
        }
    }

  //check if S is empty or not
  //if not, print the elements in order
    if(S.empty())
        cout << "\nNo ladder exist!\n";
    else {
        cout << "\nLadder: " << w1.get_word() << "-" << w2.get_word() << endl;
        while(!S.empty()) {
            cout << S.front().get_word() << endl;
            S.pop_front();
        }
    }

}
