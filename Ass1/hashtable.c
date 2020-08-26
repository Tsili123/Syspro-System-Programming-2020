#include "hashtable.h"
int count =0;
extern int ct;
unsigned long sdbm(const char *str){
    unsigned long hash = 0;
    int c;
 
    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;
 
    return hash;
}

Hashtable *initialize_hashtable(int hashtable_size, int bucket_size) {
	int i;
    // allocate memory for hashtable struct
    Hashtable *ht = malloc(sizeof(Hashtable));
    ht->hashtable_size = hashtable_size;
    ht->bucket_size = bucket_size;
    
    // allocate memory for array of lists
    ht->array = malloc(sizeof(ll *) * hashtable_size);
    
    // allocate memory for each list of the table
    for (i = 0; i < hashtable_size; i++) {
        ht->array[i] = initialize_list();
    }

    return ht;
}

Bucket *initialize_bucket(int size){
	int i;
	//size for the structure of bucket is given as argument
    // allocate memory for bucket structure 
    Bucket *bucket = malloc(sizeof(Bucket));
    bucket->entry_num = (size - sizeof(Bucket) - 8) / sizeof(Pairs);
    bucket->data = malloc( bucket->entry_num * sizeof(Pairs) );
    
    for (i = 0; i < bucket->entry_num; i++) {
        bucket->data[i] = NULL;
      //  bucket->data[i]->rootptr = initialize_tree();
    }
    
    return bucket;
}

void *insert_entry_to_ht(Hashtable **h_table, int position, char *datas ,void * elem, int (*compare_dates)(struct date,struct date)){
    ll_node *bucket_node;
    int i;
    
    //  add one bucket if there is none in this position
    if((*h_table)->array[position]->start == NULL){
        Bucket *bucket = initialize_bucket((*h_table)->bucket_size);
        bucket_node = add_ll_node(&((*h_table)->array[position]), bucket, sizeof(Bucket));
    }else{
        bucket_node = (*h_table)->array[position]->start;
    }

    // search list of buckets to find space 
    while(bucket_node != NULL){
        // cast data  to access the Bucket 
        Bucket *bucket = (Bucket*)bucket_node->data;
        
        // search bucket array for an empty space to insert new data
        for(i = 0; i < bucket->entry_num; i++){
        	//if its not empty and its the same key
        	if(bucket->data[i] != NULL && strcmp(bucket->data[i]->entry,datas) == 0){
        		
        		
				//insert to bst
				insert_bst(bucket->data[i]->rootptr->root,elem,compare_dates);
				count++;
        		printf("%d\n",count);
        		//printf("%s\n",bucket->data[i]->entry);
        		return NULL;
			}else if(bucket->data[i] == NULL){//empty place found and the item was not in the bucket
                bucket->data[i] = malloc(sizeof(Pairs));
                //insert string to entry
                bucket->data[i]->entry=datas;
                bucket->data[i]->rootptr = initialize_tree();
                bucket->data[i]->rootptr->root = set_bst_node(elem);
                //insert to bst
                //insert_bst(bucket->data[i]->rootptr->root,elem,compare_dates);
                
                //printf("MPIKE %s %d %d\n",data,i,position);
                
				
                return bucket->data[i];
            }
        }
        bucket_node = bucket_node->next;
    }
    
    // if no space was found, add a new bucket in the list and add theere the data 
    Bucket *bucket = initialize_bucket((*h_table)->bucket_size);
    bucket->data[0] = malloc(sizeof(Pairs));
    //copy entry
    bucket->data[0]->entry= datas;
    //insert to bst
    bucket->data[0]->rootptr = initialize_tree();
    bucket->data[0]->rootptr->root = set_bst_node(elem);
    insert_bst(bucket->data[0]->rootptr->root,elem,compare_dates);
    
    //memcpy(bucket->data[i], data, x);
    
    //add the bucket that was created to the list
    bucket_node = add_ll_node(&((*h_table)->array[position]), bucket, sizeof(Bucket));
    //free(bucket);
    return ((Bucket *)bucket_node->data)->data[0];
}


void *search_hashtable(Hashtable **ht, int ps, char *disease) {
	int i;
   	ll_node *bucket_nd = (*ht)->array[ps]->start;
    while (bucket_nd != NULL){
        Bucket *buckt = (Bucket *)bucket_nd->data;
        for (i = 0; i < buckt->entry_num; i++) {
            if (buckt->data[i] != NULL) {
                if (strcmp(buckt->data[i]->entry, disease) == 0) {
                    return buckt->data[i];
                }
            }
        }
        bucket_nd = bucket_nd->next;
    }
    return NULL;//return pointer to pair structure
}



void print_hashtable(Hashtable *h_table){
	int i,j,counts=0,ctr=0;
    for(i = 0; i < h_table->hashtable_size; i++){
        printf("Hashtable entry %d: \n", i);
        ll_node *entry_cur = h_table->array[i]->start;//traverse every entry
        while (entry_cur!= NULL) {
            //printf("In bucket %d: \n", counts);
            Bucket *bucket_cur = (Bucket*)entry_cur->data;
            for (j = 0; j < bucket_cur->entry_num; j++) {
                if (bucket_cur->data[j] == NULL) {
                    break;
                }
                // call print function given as argument
                printf("In bucket %d : %s : ",counts,bucket_cur->data[j]->entry);
              	ctr=inorder(bucket_cur->data[j]->rootptr->root);
              	printf("%d\n",ctr);
              	ctr=0;
            }
            entry_cur = entry_cur->next;
            counts++;
            printf("node count : %d\n",counts-1);
        }
        counts=0;
    }
}

void print_hashtable2(Hashtable *h_table,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date)){
	int i,j,counts=0;
    for(i = 0; i < h_table->hashtable_size; i++){
        printf("Hashtable entry %d: \n", i);
        ll_node *entry_cur = h_table->array[i]->start;//traverse every entry
        while (entry_cur!= NULL) {
            //printf("In bucket %d: \n", counts);
            Bucket *bucket_cur = (Bucket*)entry_cur->data;
            for (j = 0; j < bucket_cur->entry_num; j++) {
                if (bucket_cur->data[j] == NULL) {
                    break;
                }
                // call print function given as argument
                printf("In bucket %d : %s : ",counts,bucket_cur->data[j]->entry);
              	inorder2(bucket_cur->data[j]->rootptr->root,date_1,date_2,compare_dates);
              	printf("%d\n",ct);//print number of patients that are in range
              	ct=0;
            }
            entry_cur = entry_cur->next;
            counts++;
            printf("node count : %d\n",counts-1);
        }
        counts=0;//initialize node count after we exit bucket list
    }
}


void print_hashtable3(Hashtable *h_table){
	int i,j,counts=0,ctr=0;
    for(i = 0; i < h_table->hashtable_size; i++){
        printf("Hashtable entry %d: \n", i);
        ll_node *entry_cur = h_table->array[i]->start;//traverse every entry
        while (entry_cur!= NULL) {
            //printf("In bucket %d: \n", counts);
            Bucket *bucket_cur = (Bucket*)entry_cur->data;
            for (j = 0; j < bucket_cur->entry_num; j++) {
                if (bucket_cur->data[j] == NULL) {
                    break;
                }
                
              	inorder4(bucket_cur->data[j]->rootptr->root);
              	 printf("In bucket %d : %s : %d patients\n",counts,bucket_cur->data[j]->entry,ct);
              	ct=0;
            }
            entry_cur = entry_cur->next;
            counts++;
            printf("node count : %d\n",counts-1);
        }
        counts=0;
    }
}


void print_hashtable4(Hashtable *h_table,char *disease,int ps){
        Pairs *ptr;
        if((ptr=search_hashtable(&h_table,ps,disease))!=NULL){
        	
        	inorder4(ptr->rootptr->root);
	        printf("Hashtable entry %d found %s : %d patients\n", ps,ptr->entry,ct);
				        
		}
        	
        ct=0;
}


void print_hash_country(Hashtable *h_table,char *disease,int ps,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date)){
        Pairs *ptr;
        if((ptr=search_hashtable(&h_table,ps,disease))!=NULL){
        	
	        printf("Hashtable entry %d found %s : \n", ps,ptr->entry);
	        
	        inorder2(ptr->rootptr->root,date_1,date_2,compare_dates);
		}
        	
        printf("patients %d \n",ct);
        ct=0;
}

void print_hash_country2(Hashtable *h_table,char *country,char *disease,int ps,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date)){
        Pairs *ptr;
        if((ptr=search_hashtable(&h_table,ps,country))!=NULL){
        	//printf("mpike\n");
	        printf("Hashtable entry %d found %s : %s\n", ps,ptr->entry,disease);
	        
	        inorder3(ptr->rootptr->root,disease,date_1,date_2,compare_dates);
		}
        	
        printf("patients %d \n",ct);
        ct=0;
}
/*
void print_all(Hashtable *h_table){
        int i,j;
        for(i=0;i<h_table->hashtable_size;i++){
        	ll_node *bucket_nd = h_table->array[i]->start;
		    while (bucket_nd != NULL){
		        Bucket *buckt = (Bucket *)bucket_nd->data;
		        for (j = 0; j< buckt->entry_num; j++) {
		            //if (buckt->data[j] != NULL) {
		            if (buckt->data[j] == NULL) {
	                    break;
	                }
		            	//inorder5(buckt->data[j]->rootptr->root);
		            	printf("mpike\n");
		        }
		        bucket_nd = bucket_nd->next;
		    }
		}
}*/
void print_hs(Hashtable *h_table){
	int i,j,counts=0,ctr=0;
    for(i = 0; i < h_table->hashtable_size; i++){
        printf("Hashtable entry %d: \n", i);
        ll_node *entry_cur = h_table->array[i]->start;//traverse every entry
        while (entry_cur!= NULL) {
            //printf("In bucket %d: \n", counts);
            Bucket *bucket_cur = (Bucket*)entry_cur->data;
            for (j = 0; j < bucket_cur->entry_num; j++) {
                if (bucket_cur->data[j] == NULL) {
                    break;
                }
                // call print function given as argument
                printf("In bucket %d : %s : ",counts,bucket_cur->data[j]->entry);
              	ctr=inorderx(bucket_cur->data[j]->rootptr->root);
              	printf("%d\n",ctr);
              	ctr=0;
            }
            entry_cur = entry_cur->next;
            counts++;
            printf("node count : %d\n",counts-1);
        }
        counts=0;
    }
}

void print_exitdate(Hashtable *h_table,char *recordid,char *exitdate){
        int i,j;
        //TreeNode *ptr=NULL;
        for(i=0;i<h_table->hashtable_size;i++){
        	ll_node *bucket_nd = h_table->array[i]->start;
		    while (bucket_nd != NULL){
		        Bucket *buckt = (Bucket *)bucket_nd->data;
		        for (j = 0; j< buckt->entry_num; j++) {
		            if (buckt->data[j] != NULL) {
		            	search_bst(buckt->data[j]->rootptr->root,recordid,exitdate);
		            }
		        }
		        bucket_nd = bucket_nd->next;
		    }
    
        	//TreeNode * search_bst(TreeNode * root, char* key) 
		}
}

void delete_ht(Hashtable **ht){
	int i,j;
    for (i = 0; i < (*ht)->hashtable_size; i++){
        ll_node *cur = (*ht)->array[i]->start;
        ll_node *next;

        while(cur!= NULL){
            next = cur->next;
            // delete bucket data
            Bucket *bucket = (Bucket *)cur->data;
            for(j = 0; j < bucket->entry_num; j++){
                if(bucket->data[j] != NULL){
                		//free tree .Element in array  of pairs* contains pointer to that.
                		
                		//delete_bstree(bucket->data[j]->rootptr->root);
                		
						//free pointer to str key
                		free(bucket->data[j]->entry);
                		//free pointer to pair
                        free(bucket->data[j]);
                }
            }
            // delete bucket
            free(bucket->data);
            free(bucket);
            // delete list node
            free(cur);
            cur = next;
        }
        free((*ht)->array[i]);
    }
    free((*ht)->array);
    free(*ht);
}
