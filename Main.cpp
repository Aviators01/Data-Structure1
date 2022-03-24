/*
Timothy Queva
CS2010 Lab7
November 11, 2020
*/

/*
This program counts words in a file or from keyboard entry and stores them in a linked list and
binary search tree. These data structures are found in the Dictionary.cpp file. Code for this file
was taken from the assignment page and used as a template in accordance with assignment requirements.
*/

#include <iostream>

using namespace std;

typedef bool BOOL;
typedef string WORD;    

//Structures describing word entry in the dictionary
typedef struct Pair {
	int count;                  /* frequency count for a particular word */
	WORD w;                     /* the word itself */
} Pair;

typedef struct entry {
	Pair e;
	struct entry *leftChild;    /* left sub-tree */
	struct entry *rightChild;   /* right sub-tree */
	struct entry *next;         /* next sibling */
	struct entry *prev;       /* previous sibling */
} ENTRY;

//Structure describing dictionary
typedef struct diction {
	int maxEntries;     /* maximum number of entries allowed; this is an artificial limit */
                        /* linked lists can be as big as you want. This limit ensures that   */
                        /* this code tries to behave like the previous ones */

	int numWords;       /* number of words in the dictionary */
	ENTRY *Words;       /* pointer to the entries in binary tree */
	ENTRY *wordLen;     /* pointer to entries in thread */
} DICTION;

//Prototypes
ENTRY *LocateWord(DICTION&,WORD);
BOOL FullDictionary(DICTION&);
BOOL InsertWord(DICTION&,WORD);
WORD GetNextWord(void);
void DumpDictionary(DICTION&);
void rebuildList(DICTION&,ENTRY *);

const int MAX = 100;
/*
  note that these are global variables so that they are already initialized to 0
  do not write your functions so that they use these directly. All input variables MUST
  be passed as parameters!!!!
*/
DICTION dictionary={MAX,0,0,0};  /* your dictionary */
WORD word;

int main (void){
	ENTRY *pos;
	
	while(1){
		word = GetNextWord();
		
		if(word.empty()){
			DumpDictionary(dictionary);
			break;
		}
		if((pos = LocateWord(dictionary,word)) != nullptr){ 
			(pos->e).count++;
			rebuildList(dictionary,pos);
		}
		else if (!InsertWord(dictionary,word)) cout << "Dictionary full: " << "\""<< word << "\""<< " cannot be added\n";
	}
	
	/*
	BONUS TASK: Write additional code that will, after dumping the dictionary,
	ask the user for a word. Your code will then search the dictionary for that word.
	If it is found, your program will delete the word from dictionary ( don't forget
	to free your memory!!!). This will repeat until the user says no more word deletion.
	At this point, your program will dump the modified dictionary.
	
	while(word != "no" || word != "n"){
		cout << "What word would you like found and deleted from the library?" << endl;
		cout << "If you do not wish to remove/further remove words from the dictionary, type \"no\"" << endl;
		cout << "Response: ";
		cin >> word;
		word.tolower();
		pos = LocateWord(dictionary,word);
		removeWord(dictionary,pos);
	}
	*/
    return 0;
}