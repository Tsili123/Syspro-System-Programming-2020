#include "treestructure.h"
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
		//printf("wraia\n");
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

void inorder13(TreeNode *root,char *ids,MyRecord****ptr,int ***size){ 

	if(root==NULL){
		return;
	}
	inorder13(root->child_a,ids,ptr,size); 
		
	if(root->data!=NULL && strcmp(((MyRecord*)root->data)->recordID,ids)==0){
	//	if((***size)>1)
		//	ptr=realloc(ptr,***size);
		//**ptr[(***size)-1]=(MyRecord*)root->data;
		//(***size)++;
		//printf("\n");
		printf("%s %s %s\n", ((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->entrydate);
	}
			
	inorder13(root->child_b,ids,ptr,size); 

}


void inorder45(TreeNode *root,char * date_1,char* date_2,char *disease){ 
	if(root==NULL){
		return;
	}
	
	if(strcmp(((MyRecord*)root->data)->entrydate,date_1) > 0)
		inorder45(root->child_a,date_1,date_2,disease); 
		
	if(strcmp(((MyRecord*)root->data)->entrydate,date_1) >= 0 && strcmp(((MyRecord*)root->data)->entrydate,date_2) <= 0 && strcmp(((MyRecord*)root->data)->diseaseID,disease) == 0){
		ct++;
		//printf("wraia\n");
		//printf("%s %s %s\n", ((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->entrydate);
	}
			
	
	if(strcmp(((MyRecord*)root->data)->entrydate,date_2) < 0)
		inorder45(root->child_b,date_1,date_2,disease); 
}

void inorder5(TreeNode *root,char * date_1,char* date_2,char *disease){ 
	if(root==NULL){
		return;
	}
	
	if(strcmp(((MyRecord*)root->data)->exitdate,date_1) > 0)
		inorder5(root->child_a,date_1,date_2,disease); 
		
	if(strcmp(((MyRecord*)root->data)->exitdate,date_1) >= 0 && strcmp(((MyRecord*)root->data)->exitdate,date_2) <= 0  && strcmp(((MyRecord*)root->data)->diseaseID,disease) == 0){
		ct++;
		//printf("wraia\n");
		//printf("%s %s %s\n", ((MyRecord*)root->data)->country, ((MyRecord*)root->data)->diseaseID , ((MyRecord*)root->data)->entrydate);
	}
			
	
	if(strcmp(((MyRecord*)root->data)->exitdate,date_2) < 0)
		inorder5(root->child_b,date_1,date_2,disease); 
}

