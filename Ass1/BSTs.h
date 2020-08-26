#ifndef ASSIGNMENT_1_BST_H
#define ASSIGNMENT_1_BST_H
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "hashtable.h"


#define SIZEofBUFF 50

struct date 
{
   int month;
   int day;
   int year;
};

typedef struct{
	char  	recordID[4];
	char 	patientFirstName[SIZEofBUFF];
	char 	patientLastName[SIZEofBUFF];
	char	diseaseID[SIZEofBUFF];
	char	country[SIZEofBUFF];
	char	entrydate[SIZEofBUFF];
	char	exitdate[SIZEofBUFF];
} MyRecord;

typedef struct TreeNode
{
    MyRecord record;
    struct TreeNode* child_a;
    struct TreeNode* child_b;
} TreeNode;

typedef struct TreeRoot
{
    char itemID[SIZEofBUFF];//disease or country
    TreeNode* rootNode;
} TreeRoot;

void printBucket( Hashtable* hash );

TreeNode* initialize_node(MyRecord *record);
TreeRoot* initialize_root(char *itemID, TreeNode* TreeNode);
unsigned long int check_bucket(char * itemID,
    Bucket bucket, size_t bucket_size);
TreeRoot* find_item( Hashtable* hash , char* itemID);

unsigned long int insertToItemInHashTable(Hashtable* hashtable,MyRecord *record,
        char* key, unsigned long int hashtable_size);

int compare_dates (struct date d1, struct date d2);
TreeNode* insert_BST(TreeNode* node, char* date );

void inorder(struct TreeNode *root) ;
#endif
