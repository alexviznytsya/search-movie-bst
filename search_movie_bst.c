/*search_movie_bst.c*/

//
// Searching for movies in a binary search tree.
//
// Alex Viznytsya
// MacOS Sierra, Xcode 8.2.1
// Spring 2017
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int BSTKey;
typedef char* BSTValue;

typedef struct BSTNode
{
    BSTKey    Key;
    BSTValue  Value;
    struct BSTNode  *Left;
    struct BSTNode  *Right;
} BSTNode;

typedef struct BST
{
    BSTNode *Root;
    int      Count;
} BST;


//
// GetFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
//
char *GetFileName(void) {
    
    char filename[512];
    int  fnsize = sizeof(filename) / sizeof(filename[0]);
    
    // input filename from the keyboard:
    fgets(filename, fnsize, stdin);
    
    // strip EOL char(s):
    filename[strcspn(filename, "\r\n")] = '\0';
    
    // make sure filename exists and can be opened:
    FILE *infile = fopen(filename, "r");
    if (infile == NULL)
    {
        printf("**Error: unable to open '%s'\n\n", filename);
        exit(-1);
    }
    
    fclose(infile);
    
    // duplicate and return filename:
    char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
    strcpy(s, filename);
    
    return s;
}


//
// BSTCreate: dynamically creates and returns an empty
// binary search tree:
//
BST *BSTCreate()
{
    BST *tree;
    
    tree = (BST *)malloc(sizeof(BST));
    tree->Root  = NULL;
    tree->Count = 0;
    
    return tree;
}


//
// BSTCompareKeys: compares key1 and key2, returning
//   value < 0 if key1 <  key2
//   0         if key1 == key2
//   value > 0 if key1 >  key2
//
int BSTCompareKeys(BSTKey key1, BSTKey key2)
{
    if (key1 < key2)
        return -1;
    else if (key1 == key2)
        return 0;
    else
        return 1;
}


//
// BSTInsert: inserts the given (key, value) pair into the binary search
// tree.  Returns true (non-zero) if the insert was successful, returns
// false (0) if the given key is already in the tree -- in which case the
// given (key, value) pair is not inserted.
//
int BSTInsert(BST *tree, BSTKey key, BSTValue value) {
    
    BSTNode *temp = (BSTNode *)malloc(sizeof(BSTNode));
    temp->Key = key;
    temp->Value = value;
    temp->Left = 0;
    temp->Right = 0;
    
    BSTNode *currentBSTNode = NULL;
    
    if (tree->Root == 0) {
        tree->Root = temp;
        tree->Count++;
        return 1;
    } else {
        currentBSTNode = tree->Root;
        while (currentBSTNode != 0) {
            if (BSTCompareKeys(key, currentBSTNode->Key) < 0) {
                if(currentBSTNode->Left == 0) {
                    currentBSTNode->Left = temp;
                    tree->Count++;
                    return 1;
                } else {
                    currentBSTNode = currentBSTNode->Left;
                }
            } else if (BSTCompareKeys(key, currentBSTNode->Key) == 0){
                return 0;
            } else {
                if (currentBSTNode->Right == 0) {
                    currentBSTNode->Right = temp;
                    tree->Count++;
                    return 1;
                } else {
                    currentBSTNode = currentBSTNode->Right;
                }
            }
        }
    }
    
    return 0;
}


//
// PopulateBSTFromCSVFile: read CSV file records and store each record into
// BST.
//
void PopulateBSTFromCSVFile(BST *tree, char *fileName) {
    
    char line[255];
    int  lineSize = sizeof(line) / sizeof(line[0]);
    int  key = 0;
    char *value = NULL;
    char *tempValue = NULL;
    
    FILE *pFile = fopen(fileName, "r");
    fgets(line, lineSize, pFile);
    
    // reads all CSV file lines and store records into BST:
    while(fgets(line, lineSize, pFile) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        key = atoi(strtok(line, ","));
        tempValue = strtok(NULL, ",");
        value = (char *)malloc(strlen(tempValue) * sizeof(char) + 1);
        strcpy(value, tempValue);
        BSTInsert(tree, key, value);
    }
    
    return;
}


//
// BSTSearch: searches the BST for a node containing the same key.
// It prints visiting nodes and message if movie was or wasn't found.
//
void BSTSearch(BST *tree, int key) {
    
    BSTNode *currentNode = tree->Root;
    
    // traverse BST to find node with the same key:
    while (currentNode != 0) {
        printf(">>Visiting %d: '%s'\n", currentNode->Key, currentNode->Value);
        if (BSTCompareKeys(key, currentNode->Key) < 0) {
            currentNode = currentNode->Left;
        } else if (BSTCompareKeys(key, currentNode->Key) == 0) {
            printf("Movie %d: '%s'\n", currentNode->Key, currentNode->Value);
            return;
        } else {
            currentNode = currentNode->Right;
        }
    }
    
    printf("Movie %d: not found\n", key);
    return;
}

int main(void) {
    
    int key = 0;
    
    // get movies filename from the user/stdin:
    char *MoviesFileName = GetFileName();
    
    // create new BST:
    BST * tree = BSTCreate();
    
    // insert records from csv file to BST:
    PopulateBSTFromCSVFile(tree, MoviesFileName);
    
    scanf("%d", &key);
    
    // search movie by id:
    BSTSearch(tree, key);
    
    return 0;
}
