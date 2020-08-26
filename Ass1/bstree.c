#include "bstree.h"
int ct=0;


TreeRoot *initialize_tree(void){
    TreeRoot *Tree = malloc(sizeof(TreeNode));
    Tree->root = NULL;

    return Tree;
}

TreeNode *set_bst_node(void *data){
    TreeNode *new_node = malloc(sizeof(TreeNode));
    new_node->data = data;
    new_node->child_a = NULL;
    new_node->child_b = NULL;
    
    return new_node;
}

// Function to insert a new node with given key in BST 
TreeNode* insert_bst(TreeNode* node, void* data , int (*compare_dates)(struct date,struct date)){ 

	
	// If the tree is empty, return 
    if (node == NULL){
    //	printf("MPIKE\n");
		node =  set_bst_node(data); 	
		return node;
	} 

    struct date dt1,dt2;
    //MyRecord* data_d =(MyRecord*)data;
    dt1=((MyRecord*)node->data)->dt;
	dt2=((MyRecord*)data)->dt;
	
//	printf("dt 1 %d %d %d\n",dt1.day,dt1.month,dt1.year);
//	printf("dt 2 %d %d %d\n",dt2.day,dt2.month,dt2.year);
	
    // Otherwise, recurse  the tree upside down
    if(compare_dates(dt2,dt1) <= 0){
    //	printf("MPIKE1\n");
    	node->child_a = insert_bst(node->child_a, data, compare_dates); 
	}
	    
    else if (compare_dates(dt2,dt1) > 0){
    	 //   	printf("MPIKE2\n");

    	node->child_b = insert_bst(node->child_b, data ,compare_dates);    
	} 
        
    //  printf("%d\n",count);

  	// return the (unchanged) node ptr
   
    return node;
} 

// inorder traversal of bst 
int inorder(TreeNode *root){ 
	int ctr = 1;
    if (root != NULL){ 
        ctr+=inorder(root->child_a); 
        //printf("%s %s %s %s %s %s\n",((MyRecord*)root->data)->recordID, ((MyRecord*)root->data)->patientFirstName,\
		((MyRecord*)root->data)->patientLastName,((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->exitdate);
        //printf("%s %s %s\n", ((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->entrydate); 
        //printf("MPIKE\n");
        ctr+=inorder(root->child_b); 
    } else{
    	return 0;
	}
	return ctr;
	
} 

void inorder2(TreeNode *root,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date)){ 
	if(root==NULL){
		return;
	}
	
	if(compare_dates(((MyRecord*)root->data)->dt,date_1) > 0)
		inorder2(root->child_a,date_1,date_2,compare_dates); 
		
	if(compare_dates(((MyRecord*)root->data)->dt,date_1) >= 0 && compare_dates(((MyRecord*)root->data)->dt,date_2) <= 0 ){
		ct++;
		//printf("%s %s %s\n", ((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->entrydate);
	}
			
	
	if(compare_dates(((MyRecord*)root->data)->dt,date_2) < 0)
		inorder2(root->child_b,date_1,date_2,compare_dates); 
}

void inorder3(TreeNode *root,char *disease,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date)){ 
	if(root==NULL){
		return;
	}
	
	
	inorder3(root->child_a,disease,date_1,date_2,compare_dates); 
		
	if(compare_dates(((MyRecord*)root->data)->dt,date_1) >= 0 && compare_dates(((MyRecord*)root->data)->dt,date_2) <= 0 && strcmp(((MyRecord*)root->data)->diseaseID,disease)==0){
		ct++;
		//printf("%s %s %s\n", ((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->entrydate);
	}
			
		inorder3(root->child_b,disease,date_1,date_2,compare_dates); 
}


void inorder4(TreeNode *root){ 
	if(root==NULL){
		return;
	}
	
	
	inorder4(root->child_a); 
		
	if(strcmp(((MyRecord*)root->data)->exitdate,"-")==0){
		ct++;
		printf("%s %s %s %s %s %s\n",((MyRecord*)root->data)->recordID, ((MyRecord*)root->data)->patientFirstName,
		((MyRecord*)root->data)->patientLastName,((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->exitdate);
	}
	
	inorder4(root->child_b); 
}




// C function to search a given key in a given BST 
TreeNode * search_bst(TreeNode * root, char* key,char* exitdate) 
{ 
   	if(root==NULL){
		return NULL;
	}
	
	
	search_bst(root->child_a,key,exitdate); 
		
	if(strcmp(((MyRecord*)root->data)->recordID,key)==0){
		
		//printf("%s %s %s %s %s %s\n",((MyRecord*)root->data)->recordID, ((MyRecord*)root->data)->patientFirstName,
		//((MyRecord*)root->data)->patientLastName,((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->exitdate);
		printf("ok\n");
		strcpy(((MyRecord*)root->data)->exitdate,exitdate);
		// printf("Hashtable entry %d found %s : \n",i,buckt->data[i]->entry);
		printf("%s\n",((MyRecord*)root->data)->exitdate);
	}
	
	search_bst(root->child_b,key,exitdate); 
	
	
}


// inorder traversal of bst 
int inorderx(TreeNode *root){ 
	int ctr = 1;
    if (root != NULL){ 
        ctr+=inorderx(root->child_a); 
        printf("%s %s %s %s %s %s\n",((MyRecord*)root->data)->recordID, ((MyRecord*)root->data)->patientFirstName,
		((MyRecord*)root->data)->patientLastName,((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->exitdate);
        //printf("%s %s %s\n", ((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->entrydate); 
        //printf("MPIKE\n");
        ctr+=inorderx(root->child_b); 
    } else{
    	return 0;
	}
	return ctr;
	
} 


void delete_bstree(TreeNode *node){
    if(node == NULL){
        return;
    }
    
    delete_bstree(node->child_a);
    //free(node->data);
    delete_bstree(node->child_b);

    free(node->data);
    
    free(node);
}
