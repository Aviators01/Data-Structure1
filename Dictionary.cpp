/*
Timothy Queva
CS2010 Lab7
November 11, 2020
*/

/*
This file uses code from the assignment page as a template in accordance with assignment requirements.
This file holds the functions and data structures necessary in order for another file (main.cpp-needs
to be compiled first of course) to count and store entered words from keyboard or file. File features
include structs, binary search tree, and singularly linked list.
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>

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
	int maxEntries; /* maximum number of entries allowed; this is an artificial limit */
                    /* linked lists can be as big as you want. This limit ensures that   */
                    /* this code tries to behave like the previous ones */

	int numWords;	//number of words in the dictionary
	ENTRY *Words;	//pointer to the entries in binary tree
	ENTRY *wordLen;	//pointer to entries in thread
} DICTION;


//Prototypes
ENTRY *LocateWord(DICTION&,WORD);
BOOL FullDictionary(DICTION&);
BOOL InsertWord(DICTION&,WORD);
WORD GetNextWord(void);
void DumpDictionary(DICTION&);
void rebuildList(DICTION&,ENTRY *);
void FreqSorted(DICTION&);
void TreeTrav(ENTRY*);


BOOL InsertWord(DICTION &d, WORD word){
	//adds word to dictionary (tree and thread ), if word can't be added returns false else returns true
	if(!FullDictionary(d)){
		ENTRY *ptr=LocateWord(d,word);
		if(d.numWords == 0){	//This build the first node in the dictionary
			d.numWords++;
			d.Words=new ENTRY{{1,word},0,0,0,0};
			d.wordLen = d.Words;
		}
		else if(ptr == NULL){
			ptr = d.Words;
			//Traverses the tree until the correct node destination
			while((*ptr).leftChild != NULL || (*ptr).rightChild != NULL){
				if(word < (*ptr).e.w && (*ptr).leftChild != NULL){
					ptr=(*ptr).leftChild;
				}
				else if(word >= (*ptr).e.w && (*ptr).rightChild != NULL){
					ptr=ptr -> rightChild;
				}
				
				//Breaks loop once at correct null branch
				if(word < (*ptr).e.w && (*ptr).leftChild == NULL) break;
				else if(word >= (*ptr).e.w && (*ptr).rightChild == NULL) break;
			}
			
			/*Adds word/node to correct branch (left/right child), increments word count to 1,
			and adds node to doubly-linked list*/
			if(word < (*ptr).e.w){
				(*ptr).leftChild = new ENTRY{{1,word},0,0,0,0};
				ptr=(*ptr).leftChild;
				(*ptr).next=d.wordLen;
				(*d.wordLen).prev=ptr;
				d.wordLen=ptr;
				d.numWords++;
			}
			else{
				(*ptr).rightChild = new ENTRY{{1,word},0,0,0,0};
				ptr=(*ptr).rightChild;
				(*ptr).next=d.wordLen;
				(*d.wordLen).prev=ptr;
				d.wordLen=ptr;
				d.numWords++;
			}
		}
		else{		//Technically, this is redundant because of main's safeguard against adding a word already in the dictionary.
			(*ptr).e.count++;
			rebuildList(d,ptr);
		}
		
		return true;
	}
	return false;
}

void DumpDictionary(DICTION &d){
	cout << endl;
	//display the contents of dictionary in sorted order as well as dumping thread elements
	cout << string("/------------\\ \n")	//Heading for user
	+ ("| Dictionary |\n")
	+ ("\\------------/ \n \n");
	
	//Displays dictionary sorted alphabetically
	cout << "Sorted alphabetically:" << endl;
	cout << string("Word		Frequency\n")
	+ ("-------------------------") << endl;
	TreeTrav(d.Words);
	cout << "\n\n";
	
	//Displays dictionary sorted by word frequency
	cout << "Sorted by frequency (least-most)" << endl;
	cout << string("Word		Frequency\n")
	+ ("-------------------------") << endl;
	FreqSorted(d);
}

//This displays list of words sorted alphabetically
void TreeTrav(ENTRY *ptr){
	if((*ptr).leftChild != NULL) TreeTrav((*ptr).leftChild);
	cout << setw(15) << left << (*ptr).e.w << "	" << setw(8) << right << (*ptr).e.count << endl;
	if((*ptr).rightChild != NULL) TreeTrav((*ptr).rightChild);
}

//This displays list of words sorted by frequency
void FreqSorted(DICTION &d){
	ENTRY *tmp =d.wordLen;
	while(tmp != NULL){
		cout << setw(15) << left << (*tmp).e.w << "	" << setw(8) << right << (*tmp).e.count << endl;
		if((*tmp).next != nullptr) tmp = (*tmp).next;
		else break;
	}
}

//This gets the next word from file or keyboard input
WORD GetNextWord(void){
	char ch;
	WORD w = "";
	BOOL cbuilding = false;
	
	while(cin.good()){
		ch=cin.get();
		ch=tolower(ch);
		//Adds each alpha character to string
		if(isalpha(ch)){
			w.push_back(ch);
			cbuilding=true;		//this line enables else condition to executes once we finished building word
		}
		//else if code section executes once non-alphabet character is encountered
		else if(cbuilding){
			return w;
		}
	}
	return w;
}

//If dictionary is full, return true, else false
BOOL FullDictionary(DICTION &d){
	if(d.maxEntries == d.numWords) return true;
	return false;
}

void rebuildList(DICTION &d, ENTRY *p){
	/* Rebuild doubly-link list (least-most order). Function called after word found and
	frequency count incremented. p points to node whose frequency was incremented.
	*/
	
	//Rebuilds list only if p's node is not at end of list and list>1
	if(d.numWords>1 && (*p).next != NULL){
		//Rebuilds list only if p node's count is greater than next node's count
		if((*p).e.count > (*(*p).next).e.count){
			//if-else statement cuts node out of list.
			if((*p).prev == NULL){	//Is node is at head of list?
				d.wordLen=(*p).next;
				(*d.wordLen).prev = nullptr;
				(*p).next = nullptr;
			}
			else{
				(*(*p).prev).next = (*p).next;
				(*(*p).next).prev = (*p).prev;
				(*p).next = nullptr;
				(*p).prev = nullptr;
			}
			
			//Sets tmp to d.wordLen's pointer
			ENTRY *tmp;
			tmp = d.wordLen;
			
			//Advances tmp until a node's count equals p node's count or until list end
			while((*tmp).e.count < (*p).e.count && (*tmp).next != NULL){
				tmp=(*tmp).next;
			}
			
			//Deals with situation if node is at list end and word count is less than p's word count
			if((*tmp).next == NULL && (*tmp).e.count < (*p).e.count){
				(*tmp).next = p;
				(*p).prev=tmp;
			}
			//Deals with situation where node is not at list end
			else{
				(*p).prev=(*tmp).prev;
				(*(*tmp).prev).next = p;
				(*tmp).prev=p;
				(*p).next=tmp;
			}
		}
	}
	
}

ENTRY *LocateWord(DICTION &d, WORD word){
	//will determine if dictionary contains word. if found, returns pointer to entry else returns  nullptr
	ENTRY *tmp=d.wordLen;
	if(tmp != nullptr){	
		while((*tmp).e.w != word && (*tmp).next != NULL) tmp=(*tmp).next;
		if((*tmp).e.w == word) return tmp;
	}
	
	//Returns nullptr if word can't be found in dictionary
	tmp=nullptr;
	return tmp;
}