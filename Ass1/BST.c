#include "BST.h"


void printBucket( Hashtable* hash ){
    int i,j,index=0;    //Traverse every bucket
    Bucket bucket = hash->buckets[0];
    
    for(j=0;j<hash->size;j++){
    	for(i = 0; i < (hash->bucket_size / sizeof(void*)) - 2; i++){
	    		/*do{
	    	    //	printf("%s bucket:  of the nth array element of buckets ", ((TreeRoot*) bucket)->itemID); //,index,j
			        bucket = (Bucket)bucket[(hash->bucket_size / sizeof(void*)) - 1];
			        index++;
			    }while(bucket != NULL);*/
			    printf("%s bucket:  of the nth array element of buckets ", ((TreeRoot*) bucket)->itemID);
			}	
	}
    
    
    
}

// Create a leaf  node:
TreeNode* initialize_node(MyRecord *record){
    TreeNode* treenode = malloc(sizeof(TreeNode));
    strcpy(treenode->record.country,record->country);
    strcpy(treenode->record.diseaseID,record->diseaseID);
    strcpy(treenode->record.entrydate,record->entrydate);
    strcpy(treenode->record.exitdate,record->exitdate);
    strcpy(treenode->record.patientFirstName,record->patientFirstName);
    strcpy(treenode->record.patientLastName,record->patientLastName);
    strcpy(treenode->record.recordID,record->recordID);
    treenode->child_a = NULL;
    treenode->child_b = NULL;
    return treenode;
}

TreeRoot* initialize_root(char *itemID, TreeNode* treenode){
    TreeRoot* treeroot = malloc(sizeof(TreeRoot));
    strcpy(treeroot->itemID,itemID);
    treeroot->rootNode = treenode;
    return treeroot;
}

// Return position if the item is found in bucket or -1 if it's not
unsigned long int check_bucket(char * itemID,
    Bucket bucket, size_t bucket_size){
    int found = -1,i;
    for(i = 0; i < (bucket_size / sizeof(void*)) - 2; i++)
    {
        if(bucket[i] == NULL)
            break;
        else{
        //	if(((TreeRoot*)(bucket[i]))!=NULL && (((TreeRoot*)(bucket[i]))->itemID)!=NULL && itemID !=NULL )
      //  printf("%s \n",((TreeRoot*)(bucket[i]))->itemID);
        //printf("%s  \n",itemID);
	            if (strcmp(((TreeRoot*)(bucket[i]))->itemID ,itemID) == 0){
	                found = i;
	                break;
	            }
        }
    }
    return found;
}


// Return the item if found in the hashtable:
TreeRoot* find_item( Hashtable* hash , char* itemID){
    unsigned long int index = hash_sdbm(itemID, hash->size);
    Bucket bucket = hash->buckets[index];
    int found = -1;
    
    do{
        found = check_bucket(itemID, bucket, hash->bucket_size);
        if(found != -1)
            break;
        bucket = (Bucket)bucket[(hash->bucket_size / sizeof(void*)) - 1];
    }while(bucket != NULL);

    if(found != -1){
    	printf("OK %d \n",found);
        return ((TreeRoot*)(bucket[found]));
    }
    else
        return NULL;
}


// Add a new bitcoin to the bitcoin hash table.
// Return 0 if this can be done, -1 if it's already in the hash table:
unsigned long int insertToItemInHashTable(Hashtable* hashtable,MyRecord *record,
        char* key, unsigned long int hashtable_size){
        	
    unsigned long int index = hash_sdbm(key, hashtable_size);
	TreeNode *node;
	TreeRoot *root;

    // Go through buckets and see if a list exists:
    size_t bucket_size = hashtable->bucket_size;
    Bucket bucket_to_insert = hashtable->buckets[index];
    int found = -1;
    
    do{
        found = check_bucket(key, bucket_to_insert, bucket_size);
        if(found != -1)
            break;
        bucket_to_insert = (Bucket)bucket_to_insert[(bucket_size / sizeof(void*)) - 1];
    }while(bucket_to_insert != NULL);

	//insert to BST
	if(found!=-1){
		TreeRoot *treeroot = (TreeRoot*) bucket_to_insert[found];
		insert_BST( treeroot->rootNode,key);
	}
	
    // If  is not found, we can add it to the hash table:
    if(found == -1){
        bucket_to_insert = hashtable->buckets[index];
        while(bucket_to_insert[(bucket_size / sizeof(void*)) - 1] != NULL)
            bucket_to_insert = (Bucket)bucket_to_insert[(bucket_size / sizeof(void*)) - 1];
            
        node = initialize_node(record);
        root = initialize_root(key,node);
        
        insert_to_bucket(bucket_to_insert,  bucket_size , root );
        return 0;
    }

    // If  is already in a bucket, then
    else{
        return -1;
  }

}


int compare_dates (struct date d1, struct date d2){
    if (d1.year < d2.year)
       return -1;

    else if (d1.year > d2.year)
       return 1;

    if (d1.year == d2.year){
         if (d1.month<d2.month)
              return -1;
         else if (d1.month>d2.month)
              return 1;
         else if (d1.day<d2.day)
              return -1;
         else if(d1.day>d2.day)
              return 1;
         else
              return 0;
    }
}

struct date parse_date(char *date){
	struct date dt;
	char *p1,*p2, prefix;
	int counter=0;
	prefix = '-';
    p1 = date;
    
	while( (p2 = strchr( p1, prefix )) != NULL ){
	    *p2 = '\0';
	    counter++;
	    if(counter==1){
	    	dt.day=atoi(p1);
		//	printf("%d\n",dt.date);	
		}
	    else if(counter==2){
	    	dt.month=atoi(p1);
	    //	printf("%d\n",dt.month);
		}
	    p1++;
	    p2++;
	    p1 = p2;
	}
	    	dt.year=atoi(p1);
			//printf("%d\n",dt.year);	
	
	return dt;
}



/* A utility function to insert a new node with given key in BST */
TreeNode* insert_BST(TreeNode* node, char* date ) { 
    struct date dt1,dt2;
	/* If the tree is empty, return a new node */
    if (node == NULL) return NULL; 
  
  	//parse dates
  	dt1=parse_date(node->record.entrydate);
  	dt2=parse_date(date);
  	
    /* Otherwise, recur down the tree */
    if(compare_dates(dt1,dt2) < 0)
	    node->child_a = insert_BST(node->child_a, date); 
    else if (compare_dates(dt1,dt2) > 0) 
        node->child_b = insert_BST(node->child_b, date);    
  
  	/* return the (unchanged) node pointer */
    return node; 
} 

// A utility function to do inorder traversal of BST 
void inorder(struct TreeNode *root) 
{ 
    if (root != NULL) 
    { 
        inorder(root->child_a); 
        printf("%s \n", root->record.exitdate); 
        inorder(root->child_b); 
    } 
} 
